#ifndef NEON_GL_H
#define NEON_GL_H

#ifdef _MSC_VER
#include "Windows.h"
#define GLAPI WINAPI
#endif //_MSC_VER

#define GL_TEXTURE0                       0x84C0
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_VALIDATE_STATUS                0x8B83
#define GL_ARRAY_BUFFER                   0x8892
#define GL_STREAM_DRAW                    0x88E0
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8

typedef char GLchar;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

#include <GL/gl.h>

#define GLPROCLIST \
GLPROC(GLuint, 	CreateShader, GLenum type)\
GLPROC(void, 	ShaderSource, GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length)\
GLPROC(void, 	CompileShader, GLuint shader)\
GLPROC(void, 	DeleteShader, GLuint shader)\
GLPROC(void, 	DetachShader, GLuint program, GLuint shader)\
GLPROC(GLuint, 	CreateProgram, void)\
GLPROC(void,	AttachShader, GLuint program, GLuint shader)\
GLPROC(void,	LinkProgram, GLuint program)\
GLPROC(void,	UseProgram, GLuint program)\
GLPROC(void,	DeleteProgram, GLuint program)\
GLPROC(void,	ValidateProgram, GLuint program)\
GLPROC(void,	GetProgramiv, GLuint program, GLenum pname, GLint *params)\
GLPROC(void,	GetShaderInfoLog, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)\
GLPROC(void,	GetProgramInfoLog, GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)\
GLPROC(void,	GenBuffers, GLsizei n, GLuint *buffers)\
GLPROC(void,	BindBuffer, GLenum target, GLuint buffer)\
GLPROC(void,	BufferData, GLenum target, GLsizeiptr size, const void *data, GLenum usage)\
GLPROC(void,	BufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const void *data)\
GLPROC(void,	GenVertexArrays, GLsizei n, GLuint *arrays)\
GLPROC(void,	BindVertexArray, GLuint array)\
GLPROC(GLint,	GetAttribLocation, GLuint program, const GLchar *name)\
GLPROC(void,	VertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)\
GLPROC(void,	EnableVertexAttribArray, GLuint index)\
GLPROC(GLint,	GetUniformLocation, GLuint program, const GLchar *name)\
GLPROC(void,	Uniform1i, GLint location, GLint v0)\
GLPROC(void, 	UniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)\
GLPROC(void,	ActiveTexture, GLenum texture)\
//GLPROC(void, 	Finish, void)

#define GLPROC(Ret, Name, ...) typedef Ret GLAPI gl_type_##Name (__VA_ARGS__); extern gl_type_##Name *gl##Name;
GLPROCLIST
#undef GLPROC

// typedef void 	GLAPI glUseProgram (GLuint program);

// extern gl_type_UseProgram *glUseProgram;

#ifdef NEON_INIT_GL
 
#define GLPROC(Ret, Name, ...) gl_type_##Name *gl##Name;
GLPROCLIST
#undef GLPROC
bool InitGL()
{

#ifdef _MSC_VER

#define GLPROC(Ret, Name, ...) gl##Name = (gl_type_##Name *)wglGetProcAddress("gl"#Name); \
	if(gl##Name == 0) \
	{ \
		OutputDebugStringA("NEON_GL: gl" #Name "couldn't be loaded!\n"); \
	}
GLPROCLIST
#undef GLPROC

// glUseProgram = (gl_type_UseProgram *)wglGetProcAddress("glUseProgram");

#endif //_MSC_VER
return true;
}
#endif //NEON_GL_LOADER

#endif //NEON_GL_H