#include "neon_tex.h"

// @NOTE: Image row start with bottom left and end with top right.
texture* LoadTextureFromFile(char const * Filename)
{

	texture *Result = (texture *)malloc(sizeof(texture));
	
	read_file_result Data = Platform->ReadFile(Filename);

	u8 *InBytes = (u8*)Data.Content;

	u8 TGAType = *(u8 *)(InBytes + 2);
	Assert(TGAType == 2); // @TODO: Error message 

	Result->Width  = *(u16 *)(InBytes + 12);
	Result->Height = *(u16 *)(InBytes + 14);

	u8 BitsPerPixel = *(u8 *)(InBytes + 16);
	Assert(BitsPerPixel == 32); // @TODO: Error message
	Result->BitsPerPixel = 32;

	Result->ContentSize = Result->Width * Result->Height * (BitsPerPixel/8); 

	Result->Content = malloc(Result->ContentSize);
	
	memcpy(Result->Content, InBytes + sizeof(tga_header), Result->ContentSize);
	
	Platform->FreeFileMemory(&Data);

	// Little-endian 
	// On disk : 		  BB GG RR AA
	// In Mem  : 		  AA RR GG BB (Disk BBGGRRAA)
	// After Processing : RR GG BB AA (Mem 0xAABBGGRR)
	u32 *PixelPointer = (u32 *)Result->Content;
	for(u32 PixelOffset = 0; PixelOffset < (Result->ContentSize / 4); ++PixelOffset)
	{
		u32 *Pixel = (PixelPointer + PixelOffset);
		u32 B = (*Pixel & 0x000000FF) << 16;
		u32 G = (*Pixel & 0x0000FF00) ;//<< 8;
		u32 R = (*Pixel & 0x00FF0000) >> 16;
		u32 A = (*Pixel & 0xFF000000);// >> 24;
		*Pixel = R | G | B | A;
	}
	
	Result->VFliped = true;
	
	/// Test if the rows starts from top left or bottom left
	// @NOTE: The image descriptor bits are incorrect in GIMP exported TGA file.
	//
	// u8 ImageDescriptor = *(u8 *)(InBytes + 17);
	// if((ImageDescriptor & 1<<5) == 0x20)
	// {
	// 	Result->VFliped = false;
	// }
	// else
	// {
	// 	Result->VFliped = true;
	// }

	// if((ImageDescriptor & 1<<4) == 0x10)
	// {
	// 	Assert(!"Color within row start with the right side & end with left side");
	// } 
	
	return Result;
}

void FlipTextureVertically(texture *Texture)
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

void DebugTextureSave_(char const * Filename, texture *Texture)
{	
	if(Texture->VFliped)
	{
		FlipTextureVertically(Texture);
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
	// Little-endian
	// Spec:   AA RR GG BB [ BB GG RR AA (On file) ]
	
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
	Platform->WriteFile(Filename, sizeof(tga_header) + Texture->ContentSize, FileContent);

	free(FileContent);
}

void InitPack2d(texpack2d *Pack, s32 Width, s32 Height)
{
	Pack->Content = malloc(Width * Height * 4);
	if(Pack->Content == 0)
	{
		Assert(!"Error");
	}

	Pack->Width = Width;
	Pack->Height = Height;
	Pack->ContentSize = Width * Height * 4;

	Pack->Initialised = true;
	Pack->RowH = 1;
	Pack->ColW = 1;
}

void PackTexture(texpack2d *Pack, texture *Bitmap)
{
#ifdef OPENGL
	
	Assert(Pack->Initialised == true);

	if(Bitmap->VFliped)
	{
		FlipTextureVertically(Bitmap);
	}

	if((Pack->Width - Pack->ColW) >= Bitmap->Width) // Width wise space available
	{
		if((Pack->Height - Pack->RowH) >= Bitmap->Height) // Height wise space available
		{
			//
			//	pixel(i,j) = pitch*i + j
			//
			int Pitch = Pack->Width;
			for(int i = Pack->RowH; i <= Bitmap->Height; ++i)
			{

				for(int j = Pack->ColW; j <= Bitmap->Width; ++j)
				{
					
					int Offset = Pitch*(i - 1) + (j - 1);
					*((u32 *)Pack->Content + Offset) = 0; //Bitmap->Content[0]; //0xAABBGGRR
				}
			}
			
		}
	}

#endif
}