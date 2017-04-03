#ifndef 2D_BITMAP_PACKER
#define 2D_BITMAP_PACKER

// 4 channel bitmap pack
struct pack2d
{
	void *Content;
	u32 ContentSize;
	s32	Width;
	s32 Height;
};

// 4 channel bitmap
struct bitmap2d
{
	void *Content;
	u32 ContentSize;
	s32 Width;
	s32 Height;
};

void InitPack2d(pack2d *Pack);
void PackBitmap(pack2d *Pack, bitmap2d *Bitmap);
	
void InitPack2d(pack2d *Pack, s32 Width, s32 Height)
{
	Pack->Content = malloc(Width * Height * 4);
	if(Content == 0)
	{
		Assert(!"Error");
	}

	Pack->Width = Width;
	Pack->Height = Height;
	Pack->ContentSize = Width * Height * 4;
}

void PackBitmap(pack2d *Pack, bitmap2d *Bitmap)
{
	
}

#endif