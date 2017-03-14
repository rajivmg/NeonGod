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

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
////
////	rect_buffer functions 
////
void InitRectBuffer(rect_buffer *Buffer)
{
	Buffer->MemAvailable = RECT_BUFFER_SIZE; // In bytes
	Buffer->MemUsed 	 = 0;
	Buffer->Content 	 = malloc(RECT_BUFFER_SIZE);
	Buffer->Head 		 = Buffer->Content;
	if(Buffer->Content == 0)
	{
		Assert(!"AllocError");
		// @TODO: exit(0); and show message
	}
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////
////	Rectangle related functions
////
rect* MakeRectangle(Vector2 Origin, Vector2 Size,
					Vector4 Color, Vector4 UVCoords)
{
	rect *RectVertex = (rect *)malloc(sizeof(rect)); 

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
	RectVertex->Content[0] = Origin.X;
	RectVertex->Content[1] = Origin.Y + Size.Y;
	RectVertex->Content[2] = 0;

	RectVertex->Content[3]  = Color.R;
	RectVertex->Content[4]  = Color.G;
	RectVertex->Content[5]  = Color.B;
	RectVertex->Content[6]  = Color.A;

	RectVertex->Content[7]  = UVCoords.X;
	RectVertex->Content[8]  = UVCoords.W;

	// C
	RectVertex->Content[9]  = Origin.X + Size.X;
	RectVertex->Content[10] = Origin.Y + Size.Y;
	RectVertex->Content[11] = 0;

	RectVertex->Content[12] = Color.R;
	RectVertex->Content[13] = Color.G;
	RectVertex->Content[14] = Color.B;
	RectVertex->Content[15] = Color.A;

	RectVertex->Content[16] = UVCoords.Z;
	RectVertex->Content[17] = UVCoords.W;

	// A
	RectVertex->Content[18] = Origin.X;
	RectVertex->Content[19] = Origin.Y;
	RectVertex->Content[20] = 0;

	RectVertex->Content[21] = Color.R;
	RectVertex->Content[22] = Color.G;
	RectVertex->Content[23] = Color.B;
	RectVertex->Content[24] = Color.A;
  
	RectVertex->Content[25] = UVCoords.X;
	RectVertex->Content[26] = UVCoords.Y;


	// Lower triangle
	// A
	RectVertex->Content[27] = Origin.X;
	RectVertex->Content[28] = Origin.Y;
	RectVertex->Content[29] = 0;

	RectVertex->Content[30] = Color.R;
	RectVertex->Content[31] = Color.G;
	RectVertex->Content[32] = Color.B;
	RectVertex->Content[33] = Color.A;
  
	RectVertex->Content[34] = UVCoords.X;
	RectVertex->Content[35] = UVCoords.Y;

	// C
	RectVertex->Content[36] = Origin.X + Size.X;
	RectVertex->Content[37] = Origin.Y + Size.Y;
	RectVertex->Content[38] = 0;

	RectVertex->Content[39] = Color.R;
	RectVertex->Content[40] = Color.G;
	RectVertex->Content[41] = Color.B;
	RectVertex->Content[42] = Color.A;

	RectVertex->Content[43] = UVCoords.Z;
	RectVertex->Content[44] = UVCoords.W;

	// B
	RectVertex->Content[45] = Origin.X + Size.X;
	RectVertex->Content[46] = Origin.Y;
	RectVertex->Content[47] = 0;

	RectVertex->Content[48] = Color.R;
	RectVertex->Content[49] = Color.G;
	RectVertex->Content[50] = Color.B;
	RectVertex->Content[51] = Color.A;

	RectVertex->Content[52] = UVCoords.Z;
	RectVertex->Content[53] = UVCoords.Y;

	return RectVertex;
}

void InitBatch(rect_batch *RectBatch)
{
	RectBatch->Type = RECT;
	RectBatch->Status = NEVER_BOUND;
	RectBatch->IsBound = 0;
	RectBatch->IsShaderSet = 0;
	RectBatch->RectangleCount = 0;
	RectBatch->BufferDataHead = 0;
	RectBatch->BufferDataSize = 0;
}

void SetShader(rect_batch *RectBatch, shader *_Shader)
{
	RectBatch->Shader = *_Shader;
	RectBatch->IsShaderSet = 1;
}

void SetTextureMap(rect_batch *RectBatch, texture *Texture)
{
	u8 FreeTextureSlot = 0;

	glActiveTexture(GL_TEXTURE0 + FreeTextureSlot);

	glGenTextures(1, &RectBatch->Tex);
	glBindTexture(GL_TEXTURE_2D, RectBatch->Tex);

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

void CreateGPUBuffer(rect_batch *RectBatch)
{
	glGenBuffers(1, &RectBatch->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, RectBatch->VBO);
	
	glBufferData(GL_ARRAY_BUFFER, RectBatch->BufferDataSize, RectBatch->BufferDataHead, GL_DYNAMIC_DRAW);
	
	glGenVertexArrays(1, &RectBatch->VAO);
	
	glBindVertexArray(RectBatch->VAO);
	
	GLint posLoc = glGetAttribLocation(RectBatch->Shader.Program, "in_position");
	Assert(posLoc != -1);
	GLint colorLoc = glGetAttribLocation(RectBatch->Shader.Program, "in_color");
	Assert(colorLoc != -1);
	GLint texcoordLoc = glGetAttribLocation(RectBatch->Shader.Program, "in_texcoord");
	Assert(texcoordLoc != -1);

	glVertexAttribPointer((GLuint)posLoc, 3, GL_FLOAT, GL_FALSE, 36, (GLvoid *)0);
	glEnableVertexAttribArray((GLuint)posLoc);
	
	glVertexAttribPointer((GLuint)colorLoc, 4, GL_FLOAT, GL_FALSE, 36, (GLvoid *)12);
	glEnableVertexAttribArray((GLuint)colorLoc);
	
	glVertexAttribPointer((GLuint)texcoordLoc, 2, GL_FLOAT, GL_FALSE, 36, (GLvoid *)28);
	glEnableVertexAttribArray((GLuint)texcoordLoc);
}

void UpdateGPUBuffer(rect_batch *RectBatch)
{
	glBindBuffer(GL_ARRAY_BUFFER, RectBatch->VBO);
	glBufferData(GL_ARRAY_BUFFER, RectBatch->BufferDataSize, RectBatch->BufferDataHead, GL_DYNAMIC_DRAW);
}

void FlushForUpdate(rect_batch *RectBatch)
{
	RectBatch->Status = NEVER_BOUND;
	RectBatch->RectangleCount = 0;
	RectBatch->BufferDataHead = 0;
	RectBatch->BufferDataSize = 0;
}

void BindBatch(rect_batch *RectangleBatch)
{
	if(RectangleBatch->Status == UNBOUND)
	{
		CurrentBatch = RectangleBatch;	
	}
	else if(RectangleBatch->Status == NEVER_BOUND)
	{
		RectangleBatch->Status = BOUND;
		CurrentBatch = RectangleBatch;
	}
	else
	{
		// Do nothing because it's already bound :)
	}
}

void UnbindBatch(rect_batch *RectangleBatch)
{
	Assert(CurrentBatch == RectangleBatch);
	if(CurrentBatch->Status != NEVER_BOUND)
	{
		CurrentBatch->Status = UNBOUND;
		CurrentBatch = 0;
	}
}

void PushIntoBatch(rect *Rect)
{
	Assert(CurrentBatch != 0); // Check if a batch is bound 
	Assert(CurrentBatch->Type == RECT) // Check the type of batch
	Assert(CurrentBatch->Status != UNBOUND); // Unbound batch can only be drawn
	if(RectBuffer.Content == 0)
	{
		InitRectBuffer(&RectBuffer);
	}
	
	if(RectBuffer.MemAvailable >= sizeof(rect))
	{
		if(CurrentBatch->RectangleCount == 0)
		{
			CurrentBatch->BufferDataHead = RectBuffer.Content;
		}
		
		++(CurrentBatch->RectangleCount);
		CurrentBatch->BufferDataSize += sizeof(rect);

		memcpy(RectBuffer.Content, Rect->Content, sizeof(rect));
		RectBuffer.Content = (rect *)RectBuffer.Content + 1; // Move the pointer forward to point to unused memory
		RectBuffer.MemAvailable -= sizeof(rect);
		RectBuffer.MemUsed += sizeof(rect);
	}
	else
	{
		Assert(!"NoMem");
		// exit(0) and show message(Insufficient buffer memory size)
	}
}

void DrawBatch(rect_batch *RectangleBatch)
{
	Assert(CurrentBatch->IsShaderSet);
	Assert(CurrentBatch == RectangleBatch);
	Assert(CurrentBatch->Status != NEVER_BOUND);

	if(!CurrentBatch->GPUBufferCreated)
	{
		CreateGPUBuffer(CurrentBatch);
		CurrentBatch->GPUBufferCreated = 1;
	}
	else
	{
		UpdateGPUBuffer(CurrentBatch);
	}
	
	glBindVertexArray(CurrentBatch->VAO);

	glUseProgram(CurrentBatch->Shader.Program);

	r32 A = 2.0f/(r32)GetWindowWidth(), B = 2.0f/(r32)GetWindowHeight();
	Matrix4 *Proj = &CurrentBatch->Proj;
	Proj->_00 = A; Proj->_01 = 0; Proj->_02 = 0; Proj->_03 = -1;
	Proj->_10 = 0; Proj->_11 = B; Proj->_12 = 0; Proj->_13 = -1;
	Proj->_20 = 0; Proj->_21 = 0; Proj->_22 = 1; Proj->_23 =  0;
	Proj->_30 = 0; Proj->_31 = 0; Proj->_32 = 0; Proj->_33 =  1;

	GLint projLoc = glGetUniformLocation(CurrentBatch->Shader.Program, "proj");
	Assert(projLoc != -1);
	glUniformMatrix4fv((GLuint)projLoc, 1, GL_TRUE, Proj->Elements);
	
	glDrawArrays(GL_TRIANGLES, 0, CurrentBatch->BufferDataSize / sizeof(rect) * 6);

	FlushForUpdate(CurrentBatch);
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

void RectBufferFlush()
{
	RectBuffer.Content = RectBuffer.Head;
	RectBuffer.MemUsed = 0;
	RectBuffer.MemAvailable = RECT_BUFFER_SIZE;
}