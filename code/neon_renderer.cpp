#include "neon_renderer.h"

void rectangle::Rectangle(r32 MinX, r32 MinY, r32 MaxX, r32 MaxY)
{

}

u8 texture_units::GetFreeTextureUnit()
{
	// Value 0 : Free And Value 1: Used

	for(u8 Index = 0; Index < MAX_TEXTURE_UNITS; ++Index)
	{
		if(Unit[Index] == 0)
		{
			Unit[Index] = 1;
			return Index;
		}
	}

	Assert(!"No free texture unit found.");
	
	// Invalid return
	return MAX_TEXTURE_UNITS;
}

void texture_units::FreeTextureUnit(u8 Index)
{
	Assert(Index < MAX_TEXTURE_UNITS);
	Unit[Index] = 0;
}

void shader_program::Sources(read_file_result *VsFile, read_file_result *FsFile)
{
	Vs = glCreateShader(GL_VERTEX_SHADER);
	Fs = glCreateShader(GL_FRAGMENT_SHADER);
	
	Compile(VsFile, FsFile);
	MakeProgram();
}

void shader_program::Compile(read_file_result *VsFile, read_file_result *FsFile)
{
	glShaderSource(Vs, 1, (GLchar * const *)(&VsFile->Content), (const GLint *)(&VsFile->ContentSize));
	glShaderSource(Fs, 1, (GLchar * const *)(&FsFile->Content), (const GLint *)(&FsFile->ContentSize));

	glCompileShader(Vs);
	glCompileShader(Fs);
}

void shader_program::MakeProgram()
{
	Program = glCreateProgram();
	glAttachShader(Program, Vs);
	glAttachShader(Program, Fs);

	glLinkProgram(Program);
		
	glValidateProgram(Program);
	GLint Validated;
	glGetProgramiv(Program, GL_VALIDATE_STATUS, &Validated);
	
	if(!Validated)
	{
		char VsErrors[8192];
		char FsErrors[8192];
		char ProgramErrors[8192];
		glGetShaderInfoLog(Vs, sizeof(VsErrors), 0, VsErrors);
		glGetShaderInfoLog(Fs, sizeof(FsErrors), 0, FsErrors);
		glGetProgramInfoLog(Program, sizeof(ProgramErrors), 0, ProgramErrors);

		Assert(!"Shader compilation and/or linking failed");	
	}	

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

void rectangle_buffer::Init()
{
	MemAvailable = RECT_BUFFER_SIZE; // In bytes
	MemUsed 	 = 0;
	Content 	 = malloc(RECT_BUFFER_SIZE);
	Head 		 = Content;
	if(Content == 0)
	{
		Assert(!"AllocError");
		// @TODO: exit(0); and show message
	}
}

void rectangle_renderer::PushRect(rectangle const * Rectangle)
{
	if(_RectangleBuffer.Content == 0)
	{
		_RectangleBuffer.Init();
	}
	
	if(_RectangleBuffer.MemAvailable >= sizeof(rectangle))
	{
		memcpy(_RectangleBuffer.Content, Rectangle, sizeof(rectangle));
		_RectangleBuffer.Content = (rectangle *)_RectangleBuffer.Content + 1; // Move the pointer forward to point to unused memory
		_RectangleBuffer.MemAvailable -= sizeof(rectangle);
		_RectangleBuffer.MemUsed += sizeof(rectangle);
	}
	else
	{
		Assert(!"NoMem");
		// exit(0) and show message(Insufficient buffer memory size)
	}
}

void rectangle_renderer::CreateGPUBuffer()
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, _RectangleBuffer.MemUsed, _RectangleBuffer.Head, GL_DYNAMIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(ShaderProgram.AttribLoc("in_vp"), 3, GL_FLOAT, GL_FALSE, 32, (GLvoid *)0);
	glEnableVertexAttribArray(ShaderProgram.AttribLoc("in_vp"));
	glVertexAttribPointer(ShaderProgram.AttribLoc("in_vc"), 3, GL_FLOAT, GL_FALSE, 32, (GLvoid *)12);
	glEnableVertexAttribArray(ShaderProgram.AttribLoc("in_vc"));
	glVertexAttribPointer(ShaderProgram.AttribLoc("in_tc"), 2, GL_FLOAT, GL_FALSE, 32, (GLvoid *)24);
	glEnableVertexAttribArray(ShaderProgram.AttribLoc("in_tc"));
	// ...
}

void rectangle_renderer::UpdateGPUBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, _RectangleBuffer.MemUsed, _RectangleBuffer.Head, GL_DYNAMIC_DRAW);
}

void rectangle_renderer::SetShaderProgram(shader_program Program)
{
	ShaderProgram = Program;
}

void rectangle_renderer::SetTextureMap(texture *Texture)
{
	u8 FreeTextureSlot = texture_units::GetFreeTextureUnit();

	glActiveTexture(GL_TEXTURE0 + FreeTextureSlot);

	glGenTextures(1, &Tex);
	glBindTexture(GL_TEXTURE_2D, Tex);

	glTexImage2D(GL_TEXTURE_2D,
 				 0,
			 	 GL_RGBA,
			 	 Texture->Width,
			 	 Texture->Height,
			 	 0,
			 	 GL_RGBA,
			 	 GL_UNSIGNED_BYTE,
			 	 Texture->Content);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}

void rectangle_renderer::Draw()
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
	glDrawArrays(GL_TRIANGLES, 0, _RectangleBuffer.MemUsed / sizeof(rectangle) * 6);

	ClearRectBuffer();
}

void rectangle_renderer::ClearRectBuffer()
{
	_RectangleBuffer.Content = _RectangleBuffer.Head;
	_RectangleBuffer.MemUsed = 0;
	_RectangleBuffer.MemAvailable = RECT_BUFFER_SIZE;
}

void renderer::Start()
{
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_DEPTH_TEST);
}

void renderer::ClearBackBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
