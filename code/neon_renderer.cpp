#include "neon_renderer.h"

using namespace renderer;

void shader::Sources(debug_read_file_result *VsFile, debug_read_file_result *FsFile)
{
	Vs = glCreateShader(GL_VERTEX_SHADER);
	Fs = glCreateShader(GL_FRAGMENT_SHADER);
	
	Compile(VsFile, FsFile);
	MakeProgram();
}

void shader::Compile(debug_read_file_result *VsFile, debug_read_file_result *FsFile)
{
	glShaderSource(Vs, 1, (GLchar * const *)(&VsFile->Content), (const GLint *)(&VsFile->ContentSize));
	glShaderSource(Fs, 1, (GLchar * const *)(&FsFile->Content), (const GLint *)(&FsFile->ContentSize));
	// @TODO: Dump errors

	glCompileShader(Vs);
	glCompileShader(Fs);
	// @TODO: Dump errors
}

void shader::MakeProgram()
{
	Program = glCreateProgram();
	glAttachShader(Program, Vs);
	glAttachShader(Program, Fs);

	glLinkProgram(Program);
	// @TODO: Dump errors

	glDetachShader(Program, Vs);
	glDetachShader(Program, Fs);

	glDeleteShader(Vs);
	glDeleteShader(Fs);

}

void shader::Use()
{
	glUseProgram(Program);
}

void buffer_memory::Init()
{
	MemAvailable = RENDER_BUFFER_MEMORY_SIZE_MB; // In bytes
	MemUsed 	 = 0;
	Content 	 = malloc(RENDER_BUFFER_MEMORY_SIZE_MB);
	Head 		 = Content;
	if(Content == 0)
	{
		// @TODO: exit(0); and show message
	}
	/*else
	{
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, RENDER_BUFFER_MEMORY_SIZE_MB, 0, GL_DYNAMIC_DRAW);
	}*/
}

void render_buffer::PushRect(rect const* Rect)
{
	if(BufferMemory.Content == 0)
	{
		BufferMemory.Init();
	}
	
	if(BufferMemory.MemAvailable >= sizeof(rect))
	{
		memcpy(BufferMemory.Content, Rect, sizeof(rect));
		BufferMemory.Content = (rect *)BufferMemory.Content + 1; // Move the pointer forward to point to unused memory
		BufferMemory.MemAvailable -= sizeof(rect);
		BufferMemory.MemUsed += sizeof(rect);
	}
	else
	{
		// exit(0) and show message(Insufficient buffer memory size)
	}
}

void render_buffer::CopyTOGPU()
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, BufferMemory.MemUsed, BufferMemory.Content, GL_DYNAMIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//glVertexAttribPointer();
}

void render_buffer::SetShaderProgram(shader Shader)
{
	ShaderProgram = Shader.Program;
}

void render_buffer::SetTextureMap()
{

}

void render_buffer::Draw()
{	
	glUse(ShaderProgram;
	glDrawElements(GL_TRIANGLES, , GL_USIGNED_BYTE, 
	// glDrawArray()

	// Clear ram memory buffer (MemAvailable = RENDER_BUFFER_SIZE)
	// glBindVertexArray(0)
	// glDeleteVertexArrays(VAO)
	// glBindBuffer(0)
	// glDeleteBuffer(VBO)
}

void render_buffer::ClearBuffer()
{
	BufferMemory.Content = BufferMemory.Head;
	BufferMemory.MemUsed = 0;
	BufferMemory.MemAvailable = RENDER_BUFFER_MEMORY_SIZE_MB;
}
