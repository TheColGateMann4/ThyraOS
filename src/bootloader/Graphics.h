#pragma once
#include "includes/cpp_includes.h"
#include "includes/efi_includes.h"

class Graphics
{
	struct DisplayModeInformations
	{
		UINT32 index;
		UINT32 width;
		UINT32 height;
	};
public:
	Graphics(EFI_SYSTEM_TABLE* SystemTable);

public:
	void Draw() const;

private:
	DisplayModeInformations GetBestDisplayMode() const;

private:
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL* m_pixelBuffer = nullptr; 
	EFI_GRAPHICS_OUTPUT_PROTOCOL* m_gop = nullptr;

	DisplayModeInformations m_displayModeInformations = {};
};
