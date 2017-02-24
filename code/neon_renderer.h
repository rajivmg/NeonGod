#ifndef NEON_RENDERER_H
#define NEON_RENDERER_H
#include <GL/glew.h>
#include "neon_game.h"
#include "neon_texture_loader.h"
#include <string.h>

struct rectangle
{

/*
	XCTXCTXCT XCTXCTXCT
	xxx ccc tt xxx ccc tt xxx ccc tt xxx ccc tt xxx ccc tt xxx ccc tt
*/
	GLfloat	Content[48];
	void 	Rectangle(r32 MinX, r32 MinY, r32 MaxX, r32 MaxY);

};

#define MAX_TEXTURE_UNITS 16

struct texture_units
{
	static b32 Unit[MAX_TEXTURE_UNITS];
	static u8 		GetFreeTextureUnit();
	static void 	FreeTextureUnit(u8 Index);
};

b32 texture_units::Unit[MAX_TEXTURE_UNITS] = {};

#define RECT_BUFFER_SIZE MEGABYTE(64)

struct rectangle_buffer
{
	void 	Init();
	u32   	MemAvailable;
	u32  	MemUsed;
	void* 	Head;
	void* 	Content;
};

local_persist rectangle_buffer _RectangleBuffer = {};

struct shader_program
{
	GLuint 	Vs;
	GLuint 	Fs;
	GLuint 	Program;

	void 	Sources(read_file_result *VsFile, read_file_result *FsFile);
	void 	Compile(read_file_result *VsFile, read_file_result *FsFile);
	void 	MakeProgram();
	GLuint 	AttribLoc(char const * Attrib);
	GLuint 	UniformLoc(char const * Uniform);
};

struct rectangle_renderer
{
	GLuint	VBO;
	GLuint	VAO;
	GLuint	Tex;
	u32		RectCount;	
	b32 	GPUBufferCreated;
	
	shader_program 	ShaderProgram;

	void	PushRect(rectangle const* Rectangle);
	void 	CreateGPUBuffer();
	void 	UpdateGPUBuffer();
	void 	SetShaderProgram(shader_program Program);
	void 	SetTextureMap(texture *Texture);
	void 	Draw();
	void 	ClearRectBuffer();
};

namespace renderer
{
	void 	ClearBackBuffer();
	void 	Start();
}

#endif
