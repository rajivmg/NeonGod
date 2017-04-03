#include "neon_texture_loader.h"
texture* LoadTextureRGBA(char const * Filename)
{
	texture *Result = (texture *)malloc(sizeof(texture));

	read_file_result Data = ReadFile(Filename);

	u8 *InBytes = (u8*)Data.Content;
	
	if(*(u16 *)InBytes != 0x4D42)
	{
		Assert(!"Unsupported file format");
		// printf("%s is not a valid BMP file.\n", Filename);
	}

	Result->Width  = *(s32 *)(InBytes + 18);
	Result->Height = *(s32 *)(InBytes + 22);

	u16 BitsPerPixel = *(u16 *)(InBytes + 28);
	u32 Comp = *(u32 *)(InBytes + 30);

	u32 BitmapDataOffset = *(u32 *)(InBytes + 10);

	Result->ContentSize = *(u32 *)(InBytes + 34); 

	Result->Content = malloc(Result->ContentSize);
	memcpy(Result->Content, InBytes + BitmapDataOffset, Result->ContentSize);
	FreeFileMemory(&Data);
	// InFile : ABGR
	// Output : RGBA
	u32 *PixelPointer = (u32 *)Result->Content;
	for(u32 PixelOffset = 0; PixelOffset < (Result->ContentSize / 4); ++PixelOffset)
	{
		u32 *Pixel = (PixelPointer + PixelOffset);
		u32 R = (*Pixel)			  << 24;
		u32 G = (*Pixel & 0x0000FF00) << 8;
		u32 B = (*Pixel & 0x00FF0000) >> 8;
		u32 A = (*Pixel)			  >> 24;
		*Pixel = R | G | B | A;
	}
	Result->VFliped = true;
	return Result;
}

void TextureFlipV(texture *Texture)
{
	u32 *Data = (u32 *)malloc(Texture->ContentSize);

	memcpy(Data, Texture->Content, Texture->ContentSize);
	for(int Row = 0; Row < Texture->Height; ++Row)
	{
		for(int Col = 0; Col < Texture->Width; ++Col)
		{	
			*((u32 *)(Texture->Content) + ((Texture->Width) * (Texture->Height - 1 - Row)) + Col)
				= *(Data + (Texture->Width) * Row + Col); 
		}
	}

	free(Data);
}

void FreeTextureMemory(texture *Texture)
{
	free(Texture->Content);
}

void SaveAsTGA(char const * Filename, texture *Texture)
{	
	if(Texture->VFliped)
	{
		TextureFlipV(Texture);
		Texture->VFliped = false;
	}

	tga_header Header = {};

	Header.IDLength = 0;       
	Header.ColorMapType = 0;   
	Header.ImageType = 2;      
	Header.CMapStart = 0;      
	Header.CMapLength = 0;     
	Header.CMapDepth = 0;      
	Header.XOffset = 0;        
	Header.YOffset = 0;        
	Header.Width = (u16)Texture->Width;          
	Header.Height = (u16)Texture->Height;         
	Header.PixelDepth = 32;     
	Header.ImageDescriptor = 0x20;
	
	// Change the colors byte position
	// Spec:   AA  RR  GG  BB
	u32 *PixelPointer = (u32 *)Texture->Content;
	for(u32 PixelOffset = 0; PixelOffset < (Texture->ContentSize / 4); ++PixelOffset)
	{
		u32 *Pixel = (PixelPointer + PixelOffset);
	
		u32 R = (*Pixel & 0x000000FF) <<16;
		u32 G = (*Pixel & 0x0000FF00);
		u32 B = (*Pixel & 0x00FF0000) >> 16;
		u32 A = (*Pixel & 0xFF000000);
		*Pixel = A | R | G | B;
	}

	void *FileContent = malloc(sizeof(tga_header) + Texture->ContentSize);
	memcpy(FileContent, &Header, sizeof(tga_header));
	memcpy((u8 *)FileContent + sizeof(tga_header), Texture->Content, Texture->ContentSize);
	WriteFile(Filename, sizeof(tga_header) + Texture->ContentSize, FileContent);
}