#include "neon_renderer.h"

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
////
////	Shader
////
void CreateShader(shader *Shader, read_file_result *VsFile, read_file_result *FsFile)
{
	Shader->Vs = glCreateShader(GL_VERTEX_SHADER);
	Shader->Fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(Shader->Vs, 1, (GLchar * const *)(&VsFile->Content), (const GLint *)(&VsFile->ContentSize));
	glShaderSource(Shader->Fs, 1, (GLchar * const *)(&FsFile->Content), (const GLint *)(&FsFile->ContentSize));

	glCompileShader(Shader->Vs);
	glCompileShader(Shader->Fs);

	Shader->Program = glCreateProgram();
	glAttachShader(Shader->Program, Shader->Vs);
	glAttachShader(Shader->Program, Shader->Fs);

	glLinkProgram(Shader->Program);
		
	glValidateProgram(Shader->Program);
	GLint Validated;
	glGetProgramiv(Shader->Program, GL_VALIDATE_STATUS, &Validated);
	
	if(!Validated)
	{
		char VsErrors[8192];
		char FsErrors[8192];
		char ProgramErrors[8192];
		glGetShaderInfoLog(Shader->Vs, sizeof(VsErrors), 0, VsErrors);
		glGetShaderInfoLog(Shader->Fs, sizeof(FsErrors), 0, FsErrors);
		glGetProgramInfoLog(Shader->Program, sizeof(ProgramErrors), 0, ProgramErrors);

		Assert(!"Shader compilation and/or linking failed");	
	}	

	glDetachShader(Shader->Program, Shader->Vs);
	glDetachShader(Shader->Program, Shader->Fs);

	glDeleteShader(Shader->Vs);
	glDeleteShader(Shader->Fs);
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////
////	Quad related functions
////
quad* MakeQuad(Vector2 Origin, Vector2 Size, Vector4 Color, Vector4 UVCoords)
{
	quad *QuadVertex = (quad *)malloc(sizeof(quad)); 

/*
	D--------C
	|  U 	/|
	|      / |
	|  	  /	 |
	|    /   |
	|	/    |
	|  /     |
	| /      |
	|/    B  |
	A--------B
	
	A.XYZ = Origin.XYZ
	A.UV  = UVCoords.XY

	B.X   = Origin.X + Size.X
	B.Y   = Origin.Y
	B.Z   = 0
	B.U   = UVCoords.Z 
	B.V   = UVCoords.Y 

	C.X   = Origin.X + Size.X
	C.Y   = Origin.Y + Size.Y
	C.Z   = 0
	C.UV  = UVCoords.ZW

	D.X   = Origin.X 
	D.Y   = Origin.Y + Size.Y
	D.Z   = 0
	D.U   = UVCoords.X
	D.V   = UVCoords.W
 */

	// Upper triangle
	// D
	QuadVertex->Content[0] = Origin.X;
	QuadVertex->Content[1] = Origin.Y + Size.Y;
	QuadVertex->Content[2] = 0;

	QuadVertex->Content[3]  = Color.R;
	QuadVertex->Content[4]  = Color.G;
	QuadVertex->Content[5]  = Color.B;
	QuadVertex->Content[6]  = Color.A;

	QuadVertex->Content[7]  = UVCoords.X;
	QuadVertex->Content[8]  = UVCoords.W;

	// C
	QuadVertex->Content[9]  = Origin.X + Size.X;
	QuadVertex->Content[10] = Origin.Y + Size.Y;
	QuadVertex->Content[11] = 0;

	QuadVertex->Content[12] = Color.R;
	QuadVertex->Content[13] = Color.G;
	QuadVertex->Content[14] = Color.B;
	QuadVertex->Content[15] = Color.A;

	QuadVertex->Content[16] = UVCoords.Z;
	QuadVertex->Content[17] = UVCoords.W;

	// A
	QuadVertex->Content[18] = Origin.X;
	QuadVertex->Content[19] = Origin.Y;
	QuadVertex->Content[20] = 0;

	QuadVertex->Content[21] = Color.R;
	QuadVertex->Content[22] = Color.G;
	QuadVertex->Content[23] = Color.B;
	QuadVertex->Content[24] = Color.A;
  
	QuadVertex->Content[25] = UVCoords.X;
	QuadVertex->Content[26] = UVCoords.Y;


	// Lower triangle
	// A
	QuadVertex->Content[27] = Origin.X;
	QuadVertex->Content[28] = Origin.Y;
	QuadVertex->Content[29] = 0;

	QuadVertex->Content[30] = Color.R;
	QuadVertex->Content[31] = Color.G;
	QuadVertex->Content[32] = Color.B;
	QuadVertex->Content[33] = Color.A;
  
	QuadVertex->Content[34] = UVCoords.X;
	QuadVertex->Content[35] = UVCoords.Y;

	// C
	QuadVertex->Content[36] = Origin.X + Size.X;
	QuadVertex->Content[37] = Origin.Y + Size.Y;
	QuadVertex->Content[38] = 0;

	QuadVertex->Content[39] = Color.R;
	QuadVertex->Content[40] = Color.G;
	QuadVertex->Content[41] = Color.B;
	QuadVertex->Content[42] = Color.A;

	QuadVertex->Content[43] = UVCoords.Z;
	QuadVertex->Content[44] = UVCoords.W;

	// B
	QuadVertex->Content[45] = Origin.X + Size.X;
	QuadVertex->Content[46] = Origin.Y;
	QuadVertex->Content[47] = 0;

	QuadVertex->Content[48] = Color.R;
	QuadVertex->Content[49] = Color.G;
	QuadVertex->Content[50] = Color.B;
	QuadVertex->Content[51] = Color.A;

	QuadVertex->Content[52] = UVCoords.Z;
	QuadVertex->Content[53] = UVCoords.Y;

	return QuadVertex;
}

quad_colored MakeColoredQuad(Vector2 Origin, Vector2 Size,
						Vector4 Color)
{
	// quad_colored *QuadVertex = (quad_colored *)malloc(sizeof(quad_colored)); 
	quad_colored QuadVertex;
	// Upper triangle
	// D
	QuadVertex.Content[0] = Origin.X;
	QuadVertex.Content[1] = Origin.Y + Size.Y;
	QuadVertex.Content[2] = 0;

	QuadVertex.Content[3]  = Color.R;
	QuadVertex.Content[4]  = Color.G;
	QuadVertex.Content[5]  = Color.B;
	QuadVertex.Content[6]  = Color.A;

	// C
	QuadVertex.Content[7]  = Origin.X + Size.X;
	QuadVertex.Content[8] = Origin.Y + Size.Y;
	QuadVertex.Content[9] = 0;

	QuadVertex.Content[10] = Color.R;
	QuadVertex.Content[11] = Color.G;
	QuadVertex.Content[12] = Color.B;
	QuadVertex.Content[13] = Color.A;

	// A
	QuadVertex.Content[14] = Origin.X;
	QuadVertex.Content[15] = Origin.Y;
	QuadVertex.Content[16] = 0;

	QuadVertex.Content[17] = Color.R;
	QuadVertex.Content[18] = Color.G;
	QuadVertex.Content[19] = Color.B;
	QuadVertex.Content[20] = Color.A;

	// Lower t.angle
	// A
	QuadVertex.Content[21] = Origin.X;
	QuadVertex.Content[22] = Origin.Y;
	QuadVertex.Content[23] = 0;

	QuadVertex.Content[24] = Color.R;
	QuadVertex.Content[25] = Color.G;
	QuadVertex.Content[26] = Color.B;
	QuadVertex.Content[27] = Color.A;

	// C
	QuadVertex.Content[28] = Origin.X + Size.X;
	QuadVertex.Content[29] = Origin.Y + Size.Y;
	QuadVertex.Content[30] = 0;

	QuadVertex.Content[31] = Color.R;
	QuadVertex.Content[32] = Color.G;
	QuadVertex.Content[33] = Color.B;
	QuadVertex.Content[34] = Color.A;

	// B
	QuadVertex.Content[35] = Origin.X + Size.X;
	QuadVertex.Content[36] = Origin.Y;
	QuadVertex.Content[37] = 0;

	QuadVertex.Content[38] = Color.R;
	QuadVertex.Content[39] = Color.G;
	QuadVertex.Content[40] = Color.B;
	QuadVertex.Content[41] = Color.A;

	return QuadVertex;
}

void InitQuadBatch(quad_batch *QuadBatch, batch_type Type, u32 BufferSize)
{
	QuadBatch->Type = Type;
	QuadBatch->QuadCount = 0;
	QuadBatch->GPUBufferAvailable = false;
	QuadBatch->ShaderAvailable = false;
	QuadBatch->TextureAvailable = false;
	QuadBatch->TexUnit = 0;

	QuadBatch->Buffer.MemSize 	   = BufferSize;
	QuadBatch->Buffer.MemAvailable = BufferSize; // In bytes
	QuadBatch->Buffer.MemUsed 	   = 0;
	QuadBatch->Buffer.Content 	   = malloc(BufferSize);
	QuadBatch->Buffer.Head 		   = QuadBatch->Buffer.Content;
	if(QuadBatch->Buffer.Content == 0)
	{
		Assert(!"AllocError");
		// @TODO: exit(0); and show message
	}
}

void SetTextureRGBA(quad_batch *QuadBatch, texture *Texture)
{
	Assert(QuadBatch->Type == QUAD);
	QuadBatch->Tex = TextureManager(GET_TEXTURE_UNIT);
	glActiveTexture(GL_TEXTURE0 + QuadBatch->TexUnit);

	glGenTextures(1, &QuadBatch->Tex);
	glBindTexture(GL_TEXTURE_2D, QuadBatch->Tex);

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

void SetShader(quad_batch *QuadBatch, shader *_Shader)
{
	QuadBatch->Shader = *_Shader;
	QuadBatch->ShaderAvailable = true;
}

void CreateGPUBuffer(quad_batch *QuadBatch)
{
	glGenBuffers(1, &QuadBatch->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, QuadBatch->VBO);
	
	glBufferData(GL_ARRAY_BUFFER, QuadBatch->Buffer.MemUsed, QuadBatch->Buffer.Head, GL_DYNAMIC_DRAW);
	
	glGenVertexArrays(1, &QuadBatch->VAO);
	
	glBindVertexArray(QuadBatch->VAO);
	
	Assert(QuadBatch->ShaderAvailable);
	
	GLint posLoc = glGetAttribLocation(QuadBatch->Shader.Program, "in_position");
	Assert(posLoc != -1);
	GLint colorLoc = glGetAttribLocation(QuadBatch->Shader.Program, "in_color");
	Assert(colorLoc != -1);
	if(QuadBatch->Type == QUAD)
	{
		GLint texcoordLoc = glGetAttribLocation(QuadBatch->Shader.Program, "in_texcoord");
		Assert(texcoordLoc != -1);

		glVertexAttribPointer((GLuint)posLoc, 3, GL_FLOAT, GL_FALSE, 36, (GLvoid *)0);
		glEnableVertexAttribArray((GLuint)posLoc);
		
		glVertexAttribPointer((GLuint)colorLoc, 4, GL_FLOAT, GL_FALSE, 36, (GLvoid *)12);
		glEnableVertexAttribArray((GLuint)colorLoc);
		
		glVertexAttribPointer((GLuint)texcoordLoc, 2, GL_FLOAT, GL_FALSE, 36, (GLvoid *)28);
		glEnableVertexAttribArray((GLuint)texcoordLoc);
	
		GLint posSampler = glGetUniformLocation(QuadBatch->Shader.Program, "diffuse_sampler");
		Assert(posSampler != -1);
		glUniform1i(posSampler, QuadBatch->TexUnit);
	}
	else if(QuadBatch->Type == QUAD_COLORED)
	{
		glVertexAttribPointer((GLuint)posLoc, 3, GL_FLOAT, GL_FALSE, 28, (GLvoid *)0);
		glEnableVertexAttribArray((GLuint)posLoc);
		
		glVertexAttribPointer((GLuint)colorLoc, 4, GL_FLOAT, GL_FALSE, 28, (GLvoid *)12);
		glEnableVertexAttribArray((GLuint)colorLoc);
	}
}


void UpdateGPUBuffer(quad_batch *QuadBatch)
{
	glBindBuffer(GL_ARRAY_BUFFER, QuadBatch->VBO);
	glBufferData(GL_ARRAY_BUFFER, QuadBatch->Buffer.MemUsed, QuadBatch->Buffer.Head, GL_DYNAMIC_DRAW);
}

void FlushBatch(quad_batch *QuadBatch)
{
	QuadBatch->QuadCount = 0;
	QuadBatch->Buffer.Content = QuadBatch->Buffer.Head;
	QuadBatch->Buffer.MemUsed = 0;
	QuadBatch->Buffer.MemAvailable = QuadBatch->Buffer.MemSize;
}

void PushQuad(quad_batch *QuadBatch, quad *Quad)
{
	Assert(QuadBatch->Type == QUAD);
	if(QuadBatch->Buffer.Content == 0)
	{
		InitQuadBatch(QuadBatch, QUAD, MEGABYTE(4));
	}

	if(QuadBatch->Buffer.MemAvailable >= sizeof(quad))
	{
		memcpy(QuadBatch->Buffer.Content, Quad->Content, sizeof(quad));
		QuadBatch->Buffer.Content = (quad *)QuadBatch->Buffer.Content + 1; // Move the pointer forward to point to unused memory
		QuadBatch->Buffer.MemAvailable -= sizeof(quad);
		QuadBatch->Buffer.MemUsed += sizeof(quad);
		++(QuadBatch->QuadCount);
	}
	else
	{
		Assert(!"No memory available");
		// exit(0) and show message(Insufficient buffer memory size)
	}
}

void PushQuad(quad_batch *QuadBatch, quad_colored *Quad)
{
	Assert(QuadBatch->Type == QUAD_COLORED);
	if(QuadBatch->Buffer.Content == 0)
	{
		InitQuadBatch(QuadBatch, QUAD_COLORED, MEGABYTE(4));
	}

	if(QuadBatch->Buffer.MemAvailable >= sizeof(quad_colored))
	{
		memcpy(QuadBatch->Buffer.Content, Quad->Content, sizeof(quad_colored));
		QuadBatch->Buffer.Content = (quad_colored *)QuadBatch->Buffer.Content + 1; // Move the pointer forward to point to unused memory
		QuadBatch->Buffer.MemAvailable -= sizeof(quad_colored);
		QuadBatch->Buffer.MemUsed += sizeof(quad_colored);
		++(QuadBatch->QuadCount);
	}
	else
	{
		Assert(!"No memory available");
		// exit(0) and show message(Insufficient buffer memory size)
	}
}

void DrawQuadBatch(quad_batch *QuadBatch)
{
	if(!QuadBatch->GPUBufferAvailable)
	{
		CreateGPUBuffer(QuadBatch);
		QuadBatch->GPUBufferAvailable = true;
	}
	else
	{
		UpdateGPUBuffer(QuadBatch);
	}
	
	glBindVertexArray(QuadBatch->VAO);

	glUseProgram(QuadBatch->Shader.Program);

	r32 A = 2.0f/(r32)GetWindowWidth(), B = 2.0f/(r32)GetWindowHeight();
	Matrix4 *Proj = &QuadBatch->Proj;
	Proj->_00 = A; Proj->_01 = 0; Proj->_02 = 0; Proj->_03 = -1;
	Proj->_10 = 0; Proj->_11 = B; Proj->_12 = 0; Proj->_13 = -1;
	Proj->_20 = 0; Proj->_21 = 0; Proj->_22 = 1; Proj->_23 =  0;
	Proj->_30 = 0; Proj->_31 = 0; Proj->_32 = 0; Proj->_33 =  1;

	GLint projLoc = glGetUniformLocation(QuadBatch->Shader.Program, "proj");
	Assert(projLoc != -1);
	glUniformMatrix4fv((GLuint)projLoc, 1, GL_TRUE, Proj->Elements);
	
	GLsizei ContentSize = QuadBatch->Type == QUAD ? sizeof(quad) : sizeof(quad_colored);
	glDrawArrays(GL_TRIANGLES, 0, QuadBatch->Buffer.MemUsed / ContentSize * 6);

	FlushBatch(QuadBatch);
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
////
////	Texture manager functions
////
u16 TextureManager(tex_command Command, u16 TexUnit)
{
#define MAX_TEX_UNITS 12
	local_persist b32 Unit[12];
	//@NOTE: Currently max texture count is 12.
	// Unit[] is initialised to 0. 0 means free and 1 means in use.
	if(Command == GET_TEXTURE_UNIT)
	{
		for(u16 i = 0; i < MAX_TEX_UNITS; ++i)
		{
			if(Unit[i] == 0)
				return i;
		}
		Assert(!"No free texture unit")
	}
	else if(Command == RELEASE_TEXTURE_UNIT)
	{
		Assert(TexUnit != -1);
		Assert(Unit[TexUnit] != 0); // Assert is the TexUnit is already free.
		Unit[TexUnit] = 0;
	}
	return (u16)-1;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
////
////	Renderer functions
////
void InitRenderer()
{
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

	//glEnable(GL_DEPTH_TEST);
	
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); //@TODO: Handle transparent triangles

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void BackBufferFlush()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}