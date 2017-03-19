#include "neon_texture_loader.h"

texture LoadBMP_RGBA(char const * Filename)
{
	texture Result = {};

	read_file_result Data = ReadFile(Filename);

	u8 *InBytes = (u8*)Data.Content;
	
	if(*(u16 *)InBytes != 0x4D42)
	{
		printf("%s is not a valid BMP file.\n", Filename);
	}

	Result.Width  = *(s32 *)(InBytes + 18);
	Result.Height = *(s32 *)(InBytes + 22);

	u16 BitsPerPixel = *(u16 *)(InBytes + 28);
	u32 Comp = *(u32 *)(InBytes + 30);

	u32 BitmapDataOffset = *(u32 *)(InBytes + 10);

	Result.ContentSize = *(u32 *)(InBytes + 34); 

	Result.Content = malloc(Result.ContentSize);
	memcpy(Result.Content, InBytes + BitmapDataOffset, Result.ContentSize);
	// InFile : ABGR
	// Output : RGBA
	u32 *PixelPointer = (u32 *)Result.Content;
	for(u32 PixelOffset = 0; PixelOffset < (Result.ContentSize / 4); ++PixelOffset)
	{
		u32 *Pixel = (PixelPointer + PixelOffset);
		// u32 R = (*Pixel & 0x000000FF) << 24;
		u32 R = (*Pixel)			  << 24;
		u32 G = (*Pixel & 0x0000FF00) << 8;
		u32 B = (*Pixel & 0x00FF0000) >> 8;
		u32 A = (*Pixel)			  >> 24;
		// u32 A = (*Pixel & 0xFF000000) >> 24;
		*Pixel = R | G | B | A;
	}
	
	return Result;
}

void FreeTextureMemory(texture *Texture)
{
	free(Texture->Content);
}
