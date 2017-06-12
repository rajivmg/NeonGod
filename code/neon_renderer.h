#ifndef NEON_RENDERER_H
#define NEON_RENDERER_H
#include "neon_GL.h"
#include "neon_math.h"
#include "neon_platform.h"
#include "neon_texture.h"
#include <string.h> // memcpy

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
//	NOTE:
//	- This renderer follows left-hand coordinate system.
//	
//	- Clock-wise winding order represent triangle's front.
//	
//	- When rendering through OpenGL left-handed coordinate system
//		is simulated. 
//
//		+Y
//		|
//		|		 +Z (Inward)	
//		|	   / 
//		|	  /	  
//		|	 /
//		|   /
//		|  /		(3D)
//		| /
//		|/
//	    O--------------------- +X
//
//		 +Y
//		|
//		|
//		|
//		|
//		|
//		|		(2D) 
//		|
//		| 
//		O--------------------- +X
//
//
//

// Rectangle vertex data
struct quad
{
	GLfloat Content[54];
};
struct color_quad
{
	GLfloat Content[42];
};

quad Qaud(vec2 Origin, vec2 Size, vec4 UVCoords);
color_quad ColorQuad(vec2 Origin, vec2 Size, vec4 Color);

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
////
////	memory buffer
////	General purpose memory buffer
struct mem_buffer
{
	u32 MemAvailable;
	u32	MemUsed;
	u32 MemSize;

	void* Head;
	void* Content;
};

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
////
////	Shader
////
class shader
{
public:
	GLuint Vs;
	GLuint Fs;
	GLuint Program;
	bool ProgramAvailable;

	void CreateProgramFromFiles(read_file_result *VsFile, read_file_result *FsFile);

	shader();
	~shader();
};

void CreateShader(shader *Shader, read_file_result *VsFile, read_file_result *FsFile);

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
////
////	Texture manager
////
enum tex_command // Texture manager commands
{
	GET_TEXTURE_UNIT,
	RELEASE_TEXTURE_UNIT
};

u16 TextureManager(tex_command Command, u16 TexUnit = -1);

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
////
////	Rectangle Batch
////
enum batch_type
{
	QUAD,
	COLOR_QUAD,
	MESH
};

// struct quad_batch
// {
// 	GLuint VBO;
// 	GLuint VAO;
// 	GLuint Tex;
// 	shader Shader;	
// 	u16	TexUnit;
	
// 	mem_buffer Buffer;
	
// 	batch_type Type;
	
// 	b32 GPUBufferAvailable;
// 	b32 ShaderAvailable;
// 	b32 TextureAvailable;
// 	u32 QuadCount;

// 	mat4 Proj;
// };

class quad_batch
{
public:
	GLuint 	VBO;
	GLuint 	VAO;
	GLuint 	TEX;
	
	shader 	Shader;	
	u16		TextureUnit;

	u32 	QuadCount;
	
	mem_buffer Buffer;
	
	batch_type QuadType;
	
	bool Initialised;
	bool GPUBufferAvailable;
	bool ShaderAvailable;
	bool TextureAvailable;

	void Initialise(batch_type aQuadType, u32 BufferSize);
	void SetTexture(texture *Texture);
	void SetShader(shader *aShader);
	void PushQuad(quad *Quad);
	void PushQuad(color_quad *Quad);
	void Draw();

	quad_batch();
	~quad_batch();

private:
	void CreateGPUBuffer();
	void UpdateGPUBuffer();
	void Flush();
};

// void InitQuadBatch(quad_batch *QuadBatch, batch_type Type, u32 BufferSize);
// void SetBatchTexture(quad_batch *QuadBatch, texture *Texture);
// void SetBatchShader(quad_batch *QuadBatch, shader *_Shader);
// void CreateGPUBuffer(quad_batch *QuadBatch);
// void UpdateGPUBuffer(quad_batch *QuadBatch);
// void PushQuad(quad_batch *QuadBatch, quad *Quad);
// void PushQuad(quad_batch *QuadBatch, color_quad *Quad);
// void DrawQuadBatch(quad_batch *QuadBatch);
// void FlushBatch(quad_batch *QuadBatch);

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
////
////	Text rendering functions
////
struct glyph
{
	s32  BitmapWidth;
	s32  BitmapHeight;

	s32  HoriBearingX;
	s32  HoriBearingY;
	s32  HoriAdvance;

	texture_coordinates Coordinates;

	// currently unused
	s32  VertBearingX;
	s32  VertBearingY;
	s32  VertAdvance;
};

class font
{
public:
	glyph 	*Glyphs;
	u16		FontHeight;
	texture_atlas Atlas;
	bool 	Initialised;

	void Load(char const * FontSrc, u16 aFontHeight);

	font();
	~font();
};

class text_batch
{
public:
	quad_batch Batch;
	font *Font;
	bool FontSet;

	void SetFont(font *aFont);
	void PushText(char const *Text, int X, int Y);
	void Draw();

	text_batch();
	~text_batch();
};

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
////
////	Renderer functions
//// 
void InitRenderer();
void BackBufferFlush();

#endif