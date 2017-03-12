#ifndef NEON_RENDERER_H
#define NEON_RENDERER_H
#include <GL/glew.h>
#include "neon_math.h"
#include "neon_game.h"
#include "neon_texture_loader.h"
#include <string.h>

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

struct rectangle
{

/*
	XCTXCTXCT XCTXCTXCT
	xxx ccc tt xxx ccc tt xxx ccc tt xxx ccc tt xxx ccc tt xxx ccc tt
*/
	GLfloat	Content[48];

};

struct rect
{
	GLfloat Content[54];
};

// #define MAX_TEXTURE_UNITS 16

// struct texture_units
// {
// 	static b32 Unit[MAX_TEXTURE_UNITS];
// 	static u8 		GetFreeTextureUnit();
// 	static void 	FreeTextureUnit(u8 Index);
// };

// b32 texture_units::Unit[MAX_TEXTURE_UNITS] = {};

#define RECT_BUFFER_SIZE MEGABYTE(64)
struct rect_buffer
{
	void Init();
	
	u32 MemAvailable;
	u32	MemUsed;
	
	void* Head;
	void* Content;
};

struct shader_program
{
	GLuint Vs;
	GLuint Fs;
	GLuint Program;

	void Sources(read_file_result *VsFile, read_file_result *FsFile);
	void Compile(read_file_result *VsFile, read_file_result *FsFile);
	void MakeProgram();
	
	GLuint AttribLoc(char const * Attrib);
	GLuint UniformLoc(char const * Uniform);
};

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
	shader_program ShaderProgram;

	batch_type Type;

	void* BufferDataHead; // Pointer to the data in _RectangleBuffer
	u32	BufferDataSize;

	u32 RectangleCount;

	b32	IsBound;
	b32 IsShaderSet;
	b32 GPUBufferCreated;
	batch_status Status;
	Matrix4 Proj;

	//
	rect_batch()
	{ 
		Type = RECT;
		Status = NEVER_BOUND;
		IsBound = 0;
		IsShaderSet = 0;
		RectangleCount = 0;
		BufferDataHead = 0;
		BufferDataSize = 0;
	}
	
	//
	void Push(rectangle const* Rectangle);
	
	//
	void CreateGPUBuffer();
	void UpdateGPUBuffer();
	
	//
	void SetShaderProgram(shader_program Program);
	void SetTextureMap(texture *Texture);
	
	//
	void Bind();
	void Unbind();
	void Draw();
	void Flush();
};

namespace renderer
{
	local_persist rect_batch *CurrentBatch; 
	local_persist rect_buffer RectBuffer = {};
	//
	void Init();

	//
	rect* MakeRectangle(Vector2 Origin, Vector2 Size,
						Vector3 Color, Vector4 UVCoords);

	//
	//@TODO: Replace with macros
	void BatchBind(rect_batch *RectangleBatch);
	void BatchUnbind(rect_batch *RectangleBatch);
	void BatchPushContent(rect *Rect);
	void BatchDraw(rect_batch *RectangleBatch);


	//
	void BackBufferFlush();
	void RectBufferFlush();
}

#endif
