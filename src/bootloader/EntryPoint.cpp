#include <efi.h>
#include <efilib.h>

extern "C" int _fltused = 0;

extern "C"
EFI_STATUS efi_main(
		EFI_HANDLE        ImageHandle,
		EFI_SYSTEM_TABLE* SystemTable
		)
{
	InitializeLib(ImageHandle, SystemTable);

	AsciiPrint((CHAR8*)"Hello from ThyraOS!\n");

	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop = nullptr;

	EFI_GUID graphicsOutputProtocolGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	SystemTable->BootServices->LocateProtocol(
		&graphicsOutputProtocolGuid,
		nullptr,
		(void**)&gop
	);

	EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE* gopMode = gop->Mode;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* gopInformation = gopMode->Info;

	UINT32 horizontalResolution = gopInformation->HorizontalResolution;
	UINT32 verticalResolution = gopInformation->VerticalResolution;


	AsciiPrint((CHAR8*)"Screen Resolution is %dx%d", horizontalResolution, verticalResolution);

	while (true) 
	{
		__asm__ __volatile__("hlt");
	}

	return EFI_SUCCESS;
}
