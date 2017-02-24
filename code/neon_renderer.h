#ifndef NEON_RENDERER_H
#define NEON_RENDERER_H
#include <GL/glew.h>
#include "neon_game.h"
#include <string.h>

namespace renderer
{
	union rect
	{
// XCXCXC XCXCXC => xxx ccc xxx ccc xxx ccc xxx ccc xxx ccc xxx ccc
		GLfloat Content[36];
	};

	struct rect_buffer
	{
		void  Init();
		u32   MemAvailable;
		u32   MemUsed;
		void* Head;
		void* Content;
	};

	local_persist rect_buffer RectBuffer = {};

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

	struct rect_renderer
	{
		GLuint	   		VBO;
		GLuint 	   		VAO;
		shader_program 	ShaderProgram;
		u32		   		RectCount;	
		b32 			GPUBufferCreated;
		//texture_map 	*TextureMap;

		void PushRect(rect const* Rect);
		void CreateGPUBuffer();
		void UpdateGPUBuffer();
		void SetShaderProgram(shader_program Program);
		void SetTextureMap();
		void Draw();
		void ClearRectBuffer();
	};

	void ClearWindowBuffers();
	void Start();
}

#endif
