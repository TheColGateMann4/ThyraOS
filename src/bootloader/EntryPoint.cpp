#include <efi.h>
#include <efilib.h>

constexpr int a()
{
	return 1;
}

extern "C" int _fltused = 0;

extern "C"
EFI_STATUS efi_main(
    EFI_HANDLE        ImageHandle,
    EFI_SYSTEM_TABLE* SystemTable
)
{
   InitializeLib(ImageHandle, SystemTable);

    AsciiPrint((CHAR8*)"Hello from ThyraOS!\n");
    AsciiPrint((CHAR8*)"Hello from ThyraOS!\n");
    AsciiPrint((CHAR8*)"Hello from ThyraOS!\n");

    while (true) 
    {
	    __asm__ __volatile__("hlt");
    }

    return EFI_SUCCESS;
}
