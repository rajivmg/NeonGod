#ifndef NEON_RENDERER_H
#define NEON_RENDERER_H
#include <GL/glew.h>
#include "neon_game.h"
#include <string.h>

namespace renderer
{
	//GLfloat Content[90]; // 6x(XXXUUU) ... (X = xxx U= uu) = 15/vertex
	union rect
	{
		GLfloat Content[18];
		struct
		{
			GLfloat Vertex[18];
			//GLfloat UV[12];
		}; 
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

		void Sources(debug_read_file_result *VsFile, debug_read_file_result *FsFile);
		void Compile(debug_read_file_result *VsFile, debug_read_file_result *FsFile);
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
}

#endif
