#include "OpenGL.h"
#include <SDL.h>

#include <iostream>

PFNGLENABLEPROC glEnable;
PFNGLDISABLEPROC glDisable;
PFNGLBLENDFUNCPROC glBlendFunc;
PFNGLCULLFACEPROC glCullFace;
PFNGLSCISSORPROC glScissor;
PFNGLVIEWPORTPROC glViewport;
PFNGLCLEARPROC glClear;

PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLDETACHSHADERPROC glDetachShader;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORM2IVPROC glUniform2iv;
PFNGLUNIFORM2FVPROC glUniform2fv;
PFNGLUNIFORM3IVPROC glUniform3iv;
PFNGLUNIFORM3FVPROC glUniform3fv;
PFNGLUNIFORM4IVPROC glUniform4iv;
PFNGLUNIFORM4FVPROC glUniform4fv;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLBUFFERSUBDATAPROC glBufferSubData;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLDRAWELEMENTSPROC glDrawElements;

PFNGLGENTEXTURESPROC glGenTextures;
PFNGLDELETETEXTURESPROC glDeleteTextures;
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLBINDTEXTUREPROC glBindTexture;

#define STR2(X) STR(X)
#define STR(X) #X

void glInitialize()
{
	// Replace RegEx: ([^ ]+) ([^ ]+);   ->   \t\2 = (\1)SDL_GL_GetProcAddress("\2");
	glEnable = (PFNGLENABLEPROC)SDL_GL_GetProcAddress("glEnable");
	glDisable = (PFNGLDISABLEPROC)SDL_GL_GetProcAddress("glDisable");
	glBlendFunc = (PFNGLBLENDFUNCPROC)SDL_GL_GetProcAddress("glBlendFunc");
	glCullFace = (PFNGLCULLFACEPROC)SDL_GL_GetProcAddress("glCullFace");
	glScissor = (PFNGLSCISSORPROC)SDL_GL_GetProcAddress("glScissor");
	glViewport = (PFNGLVIEWPORTPROC)SDL_GL_GetProcAddress("glViewport");
	glClear = (PFNGLCLEARPROC)SDL_GL_GetProcAddress("glClear");

	glCreateShader = (PFNGLCREATESHADERPROC)SDL_GL_GetProcAddress("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)SDL_GL_GetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)SDL_GL_GetProcAddress("glCompileShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)SDL_GL_GetProcAddress("glGetShaderiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)SDL_GL_GetProcAddress("glGetShaderInfoLog");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)SDL_GL_GetProcAddress("glCreateProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)SDL_GL_GetProcAddress("glAttachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)SDL_GL_GetProcAddress("glLinkProgram");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)SDL_GL_GetProcAddress("glGetProgramiv");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)SDL_GL_GetProcAddress("glGetProgramInfoLog");
	glDetachShader = (PFNGLDETACHSHADERPROC)SDL_GL_GetProcAddress("glDetachShader");
	glDeleteShader = (PFNGLDELETESHADERPROC)SDL_GL_GetProcAddress("glDeleteShader");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)SDL_GL_GetProcAddress("glDeleteProgram");
	glUseProgram = (PFNGLUSEPROGRAMPROC)SDL_GL_GetProcAddress("glUseProgram");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)SDL_GL_GetProcAddress("glGetUniformLocation");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)SDL_GL_GetProcAddress("glGetAttribLocation");
	glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)SDL_GL_GetProcAddress("glGetActiveUniform");
	glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)SDL_GL_GetProcAddress("glGetActiveAttrib");
	glUniform1i = (PFNGLUNIFORM1IPROC)SDL_GL_GetProcAddress("glUniform1i");
	glUniform1f = (PFNGLUNIFORM1FPROC)SDL_GL_GetProcAddress("glUniform1f");
	glUniform2iv = (PFNGLUNIFORM2IVPROC)SDL_GL_GetProcAddress("glUniform2iv");
	glUniform2fv = (PFNGLUNIFORM2FVPROC)SDL_GL_GetProcAddress("glUniform2fv");
	glUniform3iv = (PFNGLUNIFORM3IVPROC)SDL_GL_GetProcAddress("glUniform3iv");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)SDL_GL_GetProcAddress("glUniform3fv");
	glUniform4iv = (PFNGLUNIFORM4IVPROC)SDL_GL_GetProcAddress("glUniform4iv");
	glUniform4fv = (PFNGLUNIFORM4FVPROC)SDL_GL_GetProcAddress("glUniform4fv");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)SDL_GL_GetProcAddress("glUniformMatrix4fv");

	glGenBuffers = (PFNGLGENBUFFERSPROC)SDL_GL_GetProcAddress("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)SDL_GL_GetProcAddress("glBindBuffer");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)SDL_GL_GetProcAddress("glDeleteBuffers");
	glBufferData = (PFNGLBUFFERDATAPROC)SDL_GL_GetProcAddress("glBufferData");
	glBufferSubData = (PFNGLBUFFERSUBDATAPROC)SDL_GL_GetProcAddress("glBufferSubData");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)SDL_GL_GetProcAddress("glVertexAttribPointer");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress("glEnableVertexAttribArray");
	glDrawElements = (PFNGLDRAWELEMENTSPROC)SDL_GL_GetProcAddress("glDrawElements");

	glGenTextures = (PFNGLGENTEXTURESPROC)SDL_GL_GetProcAddress("glGenTextures");
	glDeleteTextures = (PFNGLDELETETEXTURESPROC)SDL_GL_GetProcAddress("glDeleteTextures");
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)SDL_GL_GetProcAddress("glActiveTexture");
	glBindTexture = (PFNGLBINDTEXTUREPROC)SDL_GL_GetProcAddress("glBindTexture");

}

