#ifndef NEON_TEXTURE_LOADER_H
#define NEON_TEXTURE_LOADER_H
#include "neon_platform.h"

typedef struct texture
{
	s32 	Width;
	s32 	Height;
	void*	Content;
	u32  	ContentSize;
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

texture 	LoadBMP_RGBA(char const * Filename);
void 		FreeTextureMemory(texture *Texture);

#endif