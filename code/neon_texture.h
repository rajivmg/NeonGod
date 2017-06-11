#ifndef neon_texture_H
#define neon_texture_H
#include "neon_platform.h"

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
////
////	Texture utility functions
////
enum coordinate_convention
{
	TOP_LEFT_ZERO,
	BOTTOM_LEFT_ZERO
};

// typedef struct texture
// {
// 	s32 	Width;
// 	s32 	Height;
// 	void*	Content;
// 	u32  	ContentSize;
//   	u8    	BitsPerPixel; //32
//   	coordinate_convention Convention;
//   	bool 	Initialised;
// } texture;

#pragma pack(push, 1)
typedef struct tga_header
{
  u8    IDLength;       
  u8    ColorMapType;   
  u8    ImageType;      
  u16   CMapStart;      
  u16   CMapLength;     
  u8    CMapDepth;      
  u16   XOffset;        
  u16   YOffset;        
  u16   Width;          
  u16   Height;         
  u8    PixelDepth;     
  u8    ImageDescriptor;
} tga_header;
#pragma pack(pop)


/**
 * texture
 *	
 */
class texture
{
public:
	s32 	Width;
	s32 	Height;
	void*	Content;
	u32  	ContentSize;
  	coordinate_convention Convention;
  	bool 	Initialised;
	
	void LoadFromFile(char const *Filename);
	void FlipVertically();
	void FreeMemory();

	texture();
	~texture();
};


// texture*  LoadTextureFromFile(char const * Filename);
// void      FlipTextureVertically(texture *Texture);
// void      FreeTextureMemory(texture *Texture);

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
////
////	TextureAltas functions
////
struct texture_coordinates
{
	r32 BL_X;
	r32 BL_Y;
	r32 TR_X;
	r32 TR_Y;
};

struct texture_rect
{
	u32 OriginX;
	u32 OriginY;
	
	u32 Width;
	u32 Height;
};

struct binary_t_node
{
	binary_t_node *Child[2];

	texture_rect Rect;
	bool Filled;
};

// struct texture_atlas
// {
// 	void	*Content;
// 	u32		ContentSize;
// 	u32		Width;
// 	u32		Height;
// 	u32		Padding;
// 	bool  	Initialised;

// 	binary_t_node Node;
// 	coordinate_convention Convention;
// };

// void	InitTextureAtlas(texture_atlas *Atlas, u32 Width, u32 Heigth, u16 Padding);
// texture_coordinates	PackTexture(texture_atlas *Atlas, texture *Texture);
// void 	ConvertAtlasToTexture(texture_atlas *Atlas, texture *Texture);

class texture_atlas
{
public:
	void	*Content;
	u32		ContentSize;
	u32		Width;
	u32		Height;
	u32		Padding;
	bool  	Initialised;

	binary_t_node Node;
	coordinate_convention Convention;

	void TextureAtlas(u32 AtlasWidth, u32 AtlasHeigth, u16 AtlasPadding);
	texture_coordinates PackTexture(texture *Texture);
	void FreeMemory();

	texture_atlas();
	~texture_atlas();
};

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
////
////	Texture Debugging functions
////
#ifdef DEBUG_BUILD
  #define DebugTextureSave(F, T) DebugTextureSave_(F, T)
#elif
  #define DebugTextureSave(F, T)
#endif

void      DebugTextureSave_(char const * Filename, texture *Texture);


#endif