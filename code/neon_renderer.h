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

internal quad* MakeQuad(Vector2 Origin, Vector2 Size,
						Vector4 Color, Vector4 UVCoords);
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
////
////	Rectangle buffer
////	Store vertex data of all the rectangle to be drawn
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

internal void CreateShader(shader *Shader, read_file_result *VsFile, read_file_result *FsFile);

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
////
////	Rectangle Batch
////
enum batch_type
{
	QUAD,
	MESH
};

struct quad_batch
{
	GLuint VBO;
	GLuint VAO;
	GLuint Tex;
	shader Shader;	
	
	mem_buffer Buffer;
	
	batch_type Type;
	
	b32 GPUBufferAvailable;
	b32 ShaderAvailable;
	b32 TextureAvailable;
	u32 QuadCount;

	Matrix4 Proj;
};

internal void InitQuadBatch(quad_batch *QuadBatch, u32 BufferSize);
internal void SetTextureRGBA(quad_batch *QuadBatch, texture *Texture);
internal void SetShader(quad_batch *QuadBatch, shader *_Shader);
internal void CreateGPUBuffer(quad_batch *QuadBatch);
internal void UpdateGPUBuffer(quad_batch *QuadBatch);
internal void PushQuad(quad_batch *QuadBatch, quad *Quad);
internal void DrawQuadBatch(quad_batch *QuadBatch);
internal void FlushBatch(quad_batch *QuadBatch);

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
////
////	Renderer functions
//// 
void InitRenderer();
void BackBufferFlush();

#endif