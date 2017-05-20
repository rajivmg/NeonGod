#ifndef NEON_TEX_H
#define NEON_TEX_H
#include "neon_platform.h"

#ifdef INTERNAL_BUILD
  #define DebugTextureSave(F, T) DebugTextureSave_(F, T)
#elif
  #define DebugTextureSave(F, T)
#endif

typedef struct texture
{
	s32 	Width;
	s32 	Height;
	void*	Content;
	u32  	ContentSize;
  u8    BitsPerPixel; //32
	b32		VFliped;
} texture;

#pragma pack(push, 1)
typedef struct tga_header
{
  u8    IDLength;        /* 00h  Size of Image ID field */
  u8    ColorMapType;    /* 01h  Color map type */
  u8    ImageType;       /* 02h  Image type code */
  u16   CMapStart;       /* 03h  Color map origin */
  u16   CMapLength;      /* 05h  Color map length */
  u8    CMapDepth;       /* 07h  Depth of color map entries */
  u16   XOffset;         /* 08h  X origin of image */
  u16   YOffset;         /* 0Ah  Y origin of image */
  u16   Width;           /* 0Ch  Width of image */
  u16   Height;          /* 0Eh  Height of image */
  u8    PixelDepth;      /* 10h  Image pixel size */
  u8    ImageDescriptor; /* 11h  Image descriptor byte */
} tga_header;
#pragma pack(pop)

// 4 channel bitmap pack
//  TexPack coordinates
//    1------------ Width
//    |
//    |
//    |
//    |
//    |
//    Height

struct texpack2d
{
  void  *Content;
  u32   ContentSize;
  s32   Width;
  s32   Height;
  
  bool  Initialised;

  s32   RowH;
  s32   ColW;
  u16   WriteX;
  u16   WriteY;
};

texture*  LoadTextureFromFile(char const * Filename);
void      FlipTextureVertically(texture *Texture);
void      FreeTextureMemory(texture *Texture);

void      InitPack2d(texpack2d *Pack);
void      PackTexture(texpack2d *Pack, texture *Bitmap);

void      DebugTextureSave_(char const * Filename, texture *Texture);
#endif