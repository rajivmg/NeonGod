#include "neon_renderer.h"

using namespace renderer;

/*u8 texture_units::GetFreeTextureUnit()
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
}*/

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////	shader_program functions
////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
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
	//@TODO: Use glBindAttribLocation()
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

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////	rect_buffer functions 
////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void rect_buffer::Init()
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

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////	rect_batch functions 
////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
void rect_batch::Push(rectangle const * Rectangle)
{
	Assert(IsBound == 1);

	if(RectBuffer.Content == 0)
	{
		RectBuffer.Init();
	}
	
	if(RectBuffer.MemAvailable >= sizeof(rectangle))
	{
		if(RectangleCount == 0)
		{
			BufferDataHead = RectBuffer.Content;
		}
		
		++RectangleCount;
		BufferDataSize += sizeof(rectangle);

		memcpy(RectBuffer.Content, Rectangle, sizeof(rectangle));
		RectBuffer.Content = (rectangle *)RectBuffer.Content + 1; // Move the pointer forward to point to unused memory
		RectBuffer.MemAvailable -= sizeof(rectangle);
		RectBuffer.MemUsed += sizeof(rectangle);
	}
	else
	{
		Assert(!"NoMem");
		// exit(0) and show message(Insufficient buffer memory size)
	}
}

//
void rect_batch::CreateGPUBuffer()
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, BufferDataSize, BufferDataHead, GL_DYNAMIC_DRAW);
	
	glGenVertexArrays(1, &VAO);
	
	glBindVertexArray(VAO);
	
	glVertexAttribPointer(ShaderProgram.AttribLoc("in_vp"), 3, GL_FLOAT, GL_FALSE, 36, (GLvoid *)0);
	glEnableVertexAttribArray(ShaderProgram.AttribLoc("in_vp"));
	
	glVertexAttribPointer(ShaderProgram.AttribLoc("in_vc"), 4, GL_FLOAT, GL_FALSE, 36, (GLvoid *)12);
	glEnableVertexAttribArray(ShaderProgram.AttribLoc("in_vc"));
	
	glVertexAttribPointer(ShaderProgram.AttribLoc("in_tc"), 2, GL_FLOAT, GL_FALSE, 36, (GLvoid *)28);
	glEnableVertexAttribArray(ShaderProgram.AttribLoc("in_tc"));
}

//
void rect_batch::UpdateGPUBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, BufferDataSize, BufferDataHead, GL_DYNAMIC_DRAW);
}

//
void rect_batch::SetShaderProgram(shader_program Program)
{
	ShaderProgram = Program;
	IsShaderSet = 1;
}

//
void rect_batch::SetTextureMap(texture *Texture)
{
	u8 FreeTextureSlot = 0;//texture_units::GetFreeTextureUnit();

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

//
void rect_batch::Bind()
{
	if(IsBound == 0)
	{
		IsBound = 1;
	}
	else if(IsBound == 2)
	{
		Assert(!"Rebinding same rectangle renderer multiple times not allowed.");
	}
}

//
void rect_batch::Unbind()
{
	if(IsBound == 1)
	{
		IsBound = 2;
	}
	else if(IsBound == 0)
	{
		Assert(!"Unbinding unbound rectangle renderer is not allowed.");
	}
}

//
void rect_batch::Draw()
{	
	Assert(IsBound != 0);

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

	r32 A = 2.0f/(r32)GetWindowWidth(), B = 2.0f/(r32)GetWindowHeight();
	Proj._00 = A; Proj._01 = 0; Proj._02 = 0; Proj._03 = -1;
	Proj._10 = 0; Proj._11 = B; Proj._12 = 0; Proj._13 = -1;
	Proj._20 = 0; Proj._21 = 0; Proj._22 = 1; Proj._23 =  0;
	Proj._30 = 0; Proj._31 = 0; Proj._32 = 0; Proj._33 =  1;

	glUniformMatrix4fv(ShaderProgram.UniformLoc("proj"), 1, GL_TRUE, Proj.Elements);
	
	glDrawArrays(GL_TRIANGLES, 0, BufferDataSize / sizeof(rectangle) * 6);

	Flush();
}

//
void rect_batch::Flush()
{
	Status = NEVER_BOUND;
	RectangleCount = 0;
	BufferDataHead = 0;
	BufferDataSize = 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////	renderer namespace
////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//
void renderer::Init()
{
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

	//glEnable(GL_DEPTH_TEST);
	
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); //@TODO: Handle transparent triangles

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


//
rect* MakeRectangle(Vector2 Origin, Vector2 Size,
					Vector4 Color, Vector4 UVCoords)
{

	//GLfloat *Elements = (GLfloat *)malloc(sizeof(GLfloat) * 48);

	rect *RectVertex = (rect *)malloc(sizeof(rect)); 
/*
	Origin(5,  0,  0)
	  Size(10, 10, 0)

	----|||||||||||

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


//
void renderer::BatchBind(rect_batch *RectangleBatch)
{
	if(RectangleBatch->Status == UNBOUND)
	{
		CurrentBatch = RectangleBatch;
		// xxx
		// Assert(!"Once batch is unbound it can be only binded after Flush()");		
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

void renderer::BatchUnbind(rect_batch *RectangleBatch)
{
	Assert(CurrentBatch == RectangleBatch);
	if(CurrentBatch->Status != NEVER_BOUND)
	{
		CurrentBatch->Status = UNBOUND;
		CurrentBatch = 0;
	}
}

void renderer::BatchPushContent(rect *Rect)
{
	Assert(CurrentBatch != 0); // Check if a batch is bound 
	Assert(CurrentBatch->Type == RECT) // Check the type of batch
	Assert(CurrentBatch->Status != UNBOUND); // Unbound batch can only be drawn
	if(RectBuffer.Content == 0)
	{
		RectBuffer.Init();
	}
	
	if(RectBuffer.MemAvailable >= sizeof(rect))
	{
		if(CurrentBatch->RectangleCount == 0)
		{
			CurrentBatch->BufferDataHead = RectBuffer.Content;
		}
		
		++(CurrentBatch->RectangleCount);
		CurrentBatch->BufferDataSize += sizeof(rect); // 54: Size of one rect

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

void renderer::BatchDraw(rect_batch *RectangleBatch)
{
	Assert(CurrentBatch->IsShaderSet);
	Assert(CurrentBatch == RectangleBatch);
	Assert(CurrentBatch->Status != NEVER_BOUND);

	if(!CurrentBatch->GPUBufferCreated)
	{
		CurrentBatch->CreateGPUBuffer();
		CurrentBatch->GPUBufferCreated = 1;
	}
	else
	{
		CurrentBatch->UpdateGPUBuffer();
	}
	
	glBindVertexArray(CurrentBatch->VAO);

	glUseProgram(CurrentBatch->ShaderProgram.Program);

	r32 A = 2.0f/(r32)GetWindowWidth(), B = 2.0f/(r32)GetWindowHeight();
	Matrix4 *Proj = &CurrentBatch->Proj;
	Proj->_00 = A; Proj->_01 = 0; Proj->_02 = 0; Proj->_03 = -1;
	Proj->_10 = 0; Proj->_11 = B; Proj->_12 = 0; Proj->_13 = -1;
	Proj->_20 = 0; Proj->_21 = 0; Proj->_22 = 1; Proj->_23 =  0;
	Proj->_30 = 0; Proj->_31 = 0; Proj->_32 = 0; Proj->_33 =  1;

	glUniformMatrix4fv(CurrentBatch->ShaderProgram.UniformLoc("proj"), 1, GL_TRUE, Proj->Elements);
	
	glDrawArrays(GL_TRIANGLES, 0, CurrentBatch->BufferDataSize / sizeof(rect) * 6);

	CurrentBatch->Flush();
}

//
void renderer::BackBufferFlush()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer::RectBufferFlush()
{
	RectBuffer.Content = RectBuffer.Head;
	RectBuffer.MemUsed = 0;
	RectBuffer.MemAvailable = RECT_BUFFER_SIZE;
}

