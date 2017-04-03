#ifndef NEON_RENDERER_H
#define NEON_RENDERER_H
#include <GL/glew.h>
#include "neon_math.h"
#include "neon_platform.h"
#include "neon_texture_loader.h"
#include <string.h> // memcpy

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
//	NOTE:
//	- This renderer follows left-hand coordinate system.
//	
//	- Clock-wise winding order shows triangle's front.
//	
//	- When rendering through OGL left-handed coordinate system
//		is emulated. 
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
struct quad_colored
{
	GLfloat Content[42];
};

quad* MakeQuad(Vector2 Origin, Vector2 Size,
				Vector4 Color, Vector4 UVCoords);
quad_colored MakeColoredQuad(Vector2 Origin, Vector2 Size,
						Vector4 Color);
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
struct shader
{
	GLuint Vs;
	GLuint Fs;
	GLuint Program;
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
	QUAD_COLORED,
	MESH
};

struct quad_batch
{
	GLuint VBO;
	GLuint VAO;
	GLuint Tex;
	shader Shader;	
	u16	TexUnit;
	
	mem_buffer Buffer;
	
	batch_type Type;
	
	b32 GPUBufferAvailable;
	b32 ShaderAvailable;
	b32 TextureAvailable;
	u32 QuadCount;

	Matrix4 Proj;
};

void InitQuadBatch(quad_batch *QuadBatch, batch_type Type, u32 BufferSize);
void SetTextureRGBA(quad_batch *QuadBatch, texture *Texture);
void SetShader(quad_batch *QuadBatch, shader *_Shader);
void CreateGPUBuffer(quad_batch *QuadBatch);
void UpdateGPUBuffer(quad_batch *QuadBatch);
void PushQuad(quad_batch *QuadBatch, quad *Quad);
void PushQuad(quad_batch *QuadBatch, quad_colored *Quad);
void DrawQuadBatch(quad_batch *QuadBatch);
void FlushBatch(quad_batch *QuadBatch);

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
////
////	Text rendering functions
////
struct glyph
{
	Vector2 UV;
	r32 Left;
	r32 Top;
	r32	AdvanceX;
};

struct text_batch
{
	quad_batch Batch;
	glyph *Glyph;

};

void InitTextBatch(text_batch *TextBatch, char const * FontSrc, u16 FontHeigth);
void DrawText(text_batch *TextBatch, char const * Text, int X, int Y);

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
////
////	Renderer functions
//// 
void InitRenderer();
void BackBufferFlush();

#endif