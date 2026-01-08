#include "Graphics.h"

Graphics::Graphics(EFI_SYSTEM_TABLE* SystemTable)
{

	EFI_GUID graphicsOutputProtocolGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	SystemTable->BootServices->LocateProtocol(
		&graphicsOutputProtocolGuid,
		nullptr,
		(void**)&m_gop
	);

	EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE* gopMode = m_gop->Mode;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* gopInformation = gopMode->Info;

	m_screenWidth = gopInformation->HorizontalResolution;
	m_screenHeight = gopInformation->VerticalResolution;
	UINT32 maxMode = gopMode->MaxMode;


	EFI_GRAPHICS_PIXEL_FORMAT pixelFormat = gopInformation->PixelFormat;
	EFI_PIXEL_BITMASK pixelBitmask = gopInformation->PixelInformation;          
	
	AsciiPrint((CHAR8*)"Screen Resolution is %dx%d\n", m_screenWidth, m_screenHeight);


	void* allocatedMemory = AllocatePool(m_screenWidth * m_screenHeight * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
	m_pixelBuffer = static_cast<EFI_GRAPHICS_OUTPUT_BLT_PIXEL*>(allocatedMemory);

	for(int i = 0;i < maxMode; i++)
	{
		EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* modeInformation = nullptr; 
		UINTN sizeOfModeInformation = 0;

		EFI_STATUS result = m_gop->QueryMode(
			m_gop,
			i,
			&sizeOfModeInformation,
			&modeInformation
		);

		if(result != EFI_SUCCESS)
		{
			AsciiPrint((CHAR8*)"Failed to get mode number %d. Error was %d\n", i, result);

			continue;
		}

		UINT32 expectedSizeOfModelInformation = sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);

		if(sizeOfModeInformation != expectedSizeOfModelInformation )
		{
			AsciiPrint((CHAR8*)"size of model information was %d instead of %d\n", sizeOfModeInformation, expectedSizeOfModelInformation);

			continue;
		}

		AsciiPrint((CHAR8*)"Found Mode %d. Version %d. Resolution %dx%d. PixelFormat %d. PixelsPerScanLine %d\n", 
			i,
			modeInformation->Version,
			modeInformation->HorizontalResolution,
			modeInformation->VerticalResolution,
			static_cast<UINT32>(modeInformation->PixelFormat),
			modeInformation->PixelsPerScanLine
		);

		FreePool(modeInformation);
	}

	// EFI_STATUS result = m_gop->SetMode(
	// 	m_gop,
	// 	1
	// );
}


void Graphics::Draw() const
{
	EFI_STATUS result = m_gop->Blt(
		m_gop,
		m_pixelBuffer,
		EfiBltBufferToVideo,
		0,
		0,
		0,
		0,
		static_cast<UINTN>(m_screenWidth),
		static_cast<UINTN>(m_screenHeight),
		0
	);
}
