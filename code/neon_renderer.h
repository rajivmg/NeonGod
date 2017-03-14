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
struct rect
{
	GLfloat Content[54];
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
////
////	Rectangle buffer
////	Store vertex data of all the rectangle to be drawn
#define RECT_BUFFER_SIZE MEGABYTE(64)

struct rect_buffer
{
	u32 MemAvailable;
	u32	MemUsed;

	void* Head;
	void* Content;
};

void InitRectBuffer(rect_buffer *RectBuffer);

local_persist rect_buffer RectBuffer = {};

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
////	Rectangle Batch
////
enum batch_type
{
	RECT,
	MESH
};

enum batch_status
{
	NEVER_BOUND,
	BOUND,
	UNBOUND
};

struct rect_batch
{
	GLuint VBO;
	GLuint VAO;
	GLuint Tex;
	shader Shader;

	batch_type Type;

	void* BufferDataHead; // Pointer to the data in RectBuffer
	u32	BufferDataSize;

	u32 RectangleCount;

	b32	IsBound;
	b32 IsShaderSet;
	b32 GPUBufferCreated;
	batch_status Status;
	Matrix4 Proj;
};

local_persist rect_batch *CurrentBatch;

internal void InitBatch(rect_batch *RectBatch);
internal void SetShader(rect_batch *RectBatch);
internal void SetTexureMap(rect_batch *RectBatch, texture *Texture);
internal void CreateGPUBuffer(rect_batch *RectBatch);
internal void UpdateGPUBuffer(rect_batch *RectBatch);
internal void FlushForUpdate(rect_batch *RectBatch);

internal rect* MakeRectangle(Vector2 Origin, Vector2 Size,
						Vector3 Color, Vector4 UVCoords);
//@TODO: Replace with macros
internal void BindBatch(rect_batch *RectangleBatch);
internal void UnbindBatch(rect_batch *RectangleBatch);
internal void PushIntoBatch(rect *Rect);
internal void DrawBatch(rect_batch *RectangleBatch);

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
////
////	Renderer functions
//// 
void InitRenderer();

void UseShader();

void BackBufferFlush();
void RectBufferFlush();

#endif