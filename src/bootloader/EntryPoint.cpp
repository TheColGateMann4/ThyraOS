#include "includes/efi_includes.h"

#include "Graphics.h"

extern "C" int _fltused = 0;

extern "C"
EFI_STATUS efi_main(
		EFI_HANDLE        ImageHandle,
		EFI_SYSTEM_TABLE* SystemTable
		)
{
	InitializeLib(ImageHandle, SystemTable);

	AsciiPrint((CHAR8*)"Hello from ThyraOS!\n");

	Graphics graphics(SystemTable);

	while (true) 
	{
		__asm__ __volatile__("hlt");
	}

	return EFI_SUCCESS;
}
