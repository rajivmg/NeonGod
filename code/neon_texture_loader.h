#ifndef NEON_TEXTURE_LOADER_H
#define NEON_TEXTURE_LOADER_H
#include "neon_platform.h"

// 4-channel texture
typedef struct texture
{
	s32 	Width;
	s32 	Height;
	void*	Content;
	u32  	ContentSize;
	b32		VFliped;
} texture;

typedef struct bmp_format
{
	u16 	Type;           /* Magic number for file */
    u32   	FileSize;       /* Size of file */
    u16 	Reserved1;      /* Reserved */
    u16 	Reserved2;      /* ... */
    u32   	OffBits;        /* Offset to bitmap data */

    u32   	InfoHeaderSize; /* Size of info header */
    s32     Width;          /* Width of image */
    s32     Height;         /* Height of image */
    u16 	Planes;         /* Number of color planes */
    u16 	BitCount;       /* Number of bits per pixel */
    u32 	Compression;    /* Type of compression to use */
    u32 	SizeImage;      /* Size of image data */
    s32 	XPelsPerMeter;  /* X pixels per meter */
    s32 	YPelsPerMeter;  /* Y pixels per meter */
    u32   	ClrUsed;        /* Number of colors used */
    u32   	ClrImportant;   /* Number of important colors */
} bmp_format;

#pragma pack(push, 1)
typedef struct tga_header{
  u8  IDLength;        /* 00h  Size of Image ID field */
  u8  ColorMapType;    /* 01h  Color map type */
  u8  ImageType;       /* 02h  Image type code */
  u16 CMapStart;       /* 03h  Color map origin */
  u16 CMapLength;      /* 05h  Color map length */
  u8  CMapDepth;       /* 07h  Depth of color map entries */
  u16 XOffset;         /* 08h  X origin of image */
  u16 YOffset;         /* 0Ah  Y origin of image */
  u16 Width;           /* 0Ch  Width of image */
  u16 Height;          /* 0Eh  Height of image */
  u8  PixelDepth;      /* 10h  Image pixel size */
  u8  ImageDescriptor; /* 11h  Image descriptor byte */
} tga_header;
#pragma pack(pop)

texture* LoadTextureRGBA(char const * Filename);
void TextureFlipV(texture *Texture);
void FreeTextureMemory(texture *Texture);
void DebugSaveAsTGA(char const * Filename, texture *Texture);
void SaveAsTGA(char const * Filename, texture *Texture);
#endif