#include "Graphics.h"

Graphics::Graphics(EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_GUID graphicsOutputProtocolGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	SystemTable->BootServices->LocateProtocol(
		&graphicsOutputProtocolGuid,
		nullptr,
		(void**)&m_gop
	);


	m_displayModeInformations = GetBestDisplayMode();


	EFI_STATUS result = m_gop->SetMode(
		m_gop,
		m_displayModeInformations.index
	);

	AsciiPrint((CHAR8*)"Selected Mode %d with resolution %dx%d\n", m_displayModeInformations.index, m_displayModeInformations.width, m_displayModeInformations.height);

	void* allocatedMemory = AllocatePool(m_displayModeInformations.width * m_displayModeInformations.width * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
	m_pixelBuffer = static_cast<EFI_GRAPHICS_OUTPUT_BLT_PIXEL*>(allocatedMemory);
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
		static_cast<UINTN>(m_displayModeInformations.width),
		static_cast<UINTN>(m_displayModeInformations.height),
		0
	);
}

Graphics::DisplayModeInformations Graphics::GetBestDisplayMode() const
{
	EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE* defaultMode = m_gop->Mode;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* gopInformation = defaultMode->Info;

	UINT32 maxMode = defaultMode->MaxMode;

	DisplayModeInformations bestDisplayOptions = {
		.index = 0,
		.width = gopInformation->HorizontalResolution,
		.height = gopInformation->VerticalResolution
	};

	float nativeAspectRatio = static_cast<float>(bestDisplayOptions.width) / static_cast<float>(bestDisplayOptions.height);
	float epsilon = 1e-4f;

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

		if(sizeOfModeInformation < expectedSizeOfModelInformation)
		{
			AsciiPrint((CHAR8*)"Model information was too small. %d instead of %d\n", sizeOfModeInformation, expectedSizeOfModelInformation);

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

		float currentAspectRatio = static_cast<float>(modeInformation->HorizontalResolution) / static_cast<float>(modeInformation->VerticalResolution);

		if(modeInformation->HorizontalResolution > bestDisplayOptions.width  && abs(currentAspectRatio - nativeAspectRatio) < epsilon) 
		{
			bestDisplayOptions.index = i;
			bestDisplayOptions.width = modeInformation->HorizontalResolution;
			bestDisplayOptions.height = modeInformation->VerticalResolution;

			nativeAspectRatio = static_cast<float>(bestDisplayOptions.width) / static_cast<float>(bestDisplayOptions.height);
		}

		FreePool(modeInformation);
	}

	return bestDisplayOptions;
}
