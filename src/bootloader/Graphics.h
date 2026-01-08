#pragma once
#include "includes/efi_includes.h"

class Graphics
{
public:
	Graphics(EFI_SYSTEM_TABLE* SystemTable);

public:
	void Draw() const;

private:
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL* m_pixelBuffer = nullptr; 
	EFI_GRAPHICS_OUTPUT_PROTOCOL* m_gop = nullptr;

	UINT32 m_screenWidth;
	UINT32 m_screenHeight;
};
