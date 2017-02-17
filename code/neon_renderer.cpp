#include "neon_renderer.h"

using namespace renderer;

void shader_program::Sources(debug_read_file_result *VsFile, debug_read_file_result *FsFile)
{
	Vs = glCreateShader(GL_VERTEX_SHADER);
	Fs = glCreateShader(GL_FRAGMENT_SHADER);
	
	Compile(VsFile, FsFile);
	MakeProgram();
}

void shader_program::Compile(debug_read_file_result *VsFile, debug_read_file_result *FsFile)
{
	glShaderSource(Vs, 1, (GLchar * const *)(&VsFile->Content), (const GLint *)(&VsFile->ContentSize));
	glShaderSource(Fs, 1, (GLchar * const *)(&FsFile->Content), (const GLint *)(&FsFile->ContentSize));
	// @TODO: Dump errors

	glCompileShader(Vs);
	glCompileShader(Fs);
	// @TODO: Dump errors
}

void shader_program::MakeProgram()
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

GLuint shader_program::AttribLoc(char const * Attrib)
{
	GLint Loc = glGetAttribLocation(Program, Attrib);
	Assert(Loc!=-1);
	return (GLuint)Loc;
}

GLuint shader_program::UniformLoc(char const * Uniform)
{
	GLint Loc = glGetUniformLocation(Program, Uniform);
	Assert(Loc != -1);
	return (GLuint)Loc;
}

void rect_buffer::Init()
{
	MemAvailable = RECT_BUFFER_SIZE_MB; // In bytes
	MemUsed 	 = 0;
	Content 	 = malloc(RECT_BUFFER_SIZE_MB);
	Head 		 = Content;
	if(Content == 0)
	{
		Assert("AllocError");
		// @TODO: exit(0); and show message
	}
}

void rect_renderer::PushRect(rect const * Rect)
{
	if(RectBuffer.Content == 0)
	{
		RectBuffer.Init();
	}
	
	if(RectBuffer.MemAvailable >= sizeof(rect))
	{
		memcpy(RectBuffer.Content, Rect, sizeof(rect));
		RectBuffer.Content = (rect *)RectBuffer.Content + 1; // Move the pointer forward to point to unused memory
		RectBuffer.MemAvailable -= sizeof(rect);
		RectBuffer.MemUsed += sizeof(rect);
	}
	else
	{
		Assert("NoMem");
		// exit(0) and show message(Insufficient buffer memory size)
	}
}

void rect_renderer::CreateGPUBuffer()
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, RectBuffer.MemUsed, RectBuffer.Head, GL_DYNAMIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(ShaderProgram.AttribLoc("in_vp"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(ShaderProgram.AttribLoc("in_vp"));
	// ...
}

void rect_renderer::UpdateGPUBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, RectBuffer.MemUsed, RectBuffer.Head, GL_DYNAMIC_DRAW);
}

void rect_renderer::SetShaderProgram(shader_program Program)
{
	ShaderProgram = Program;
}

void rect_renderer::SetTextureMap()
{

}

void rect_renderer::Draw()
{	
	if(!GPUBufferCreated)
	{
		CreateGPUBuffer();
		GPUBufferCreated = 1;
	}
	else
	{
		UpdateGPUBuffer();
	}
	
	glBindVertexArray(VAO);

	glUseProgram(ShaderProgram.Program);
	glDrawArrays(GL_TRIANGLES, 0, RectBuffer.MemUsed / sizeof(rect) * 6);

	ClearRectBuffer();
}

void rect_renderer::ClearRectBuffer()
{
	RectBuffer.Content = RectBuffer.Head;
	RectBuffer.MemUsed = 0;
	RectBuffer.MemAvailable = RECT_BUFFER_SIZE_MB;
}

void renderer::ClearWindowBuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
