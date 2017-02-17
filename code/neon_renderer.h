#ifndef NEON_RENDERER_H
#define NEON_RENDERER_H
#include <GL/glew.h>
#include "neon_game.h"
#include <string.h>

namespace renderer
{
	struct rect
	{
		GLfloat Content[12]; //VVUU
		union
		{
			GLfloat Vertex[6];
			GLfloat UV[6];
			GLbyte	Indices[6];
		};
	};
	
	struct buffer_memory
	{
		void  Init();
		u32   MemAvailable;
		u32   MemUsed;
		void* Head;
		void* Content;
	};

	local_persist buffer_memory BufferMemory = {};

	struct shader
	{
		GLuint Vs;
		GLuint Fs;
		GLuint Program;

		void Sources(debug_read_file_result *VsFile, debug_read_file_result *FsFile);
		void Compile(debug_read_file_result *VsFile, debug_read_file_result *FsFile);
		void MakeProgram();
		void Use();
	};

	/**
	 * @CHECK: @OPTIMIZE:
	 *	Use one glBuffer and copy all rect info into
	 * 	one memory block and use indexed draw()
	 */
	struct render_buffer
	{
		GLuint	   VBO;
		GLuint 	   VAO;
		GLuint 	   ShaderProgram;
		// texture_map *TextureMap;

		local_persist 

		void PushRect(rect const* Rect);
		void CopyTOGPU();
		void SetShaderProgram(shader Shader);
		void SetTextureMap();
		void Draw();
	};
}

#endif
