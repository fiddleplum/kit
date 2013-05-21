#pragma once

#include "gl3.h"

void glInitialize();

extern PFNGLSCISSORPROC glScissor;

// Shaders
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLDETACHSHADERPROC glDetachShader;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLUSEPROGRAMPROC glUseProgram;

#ifdef _WIN32

#define NOMINMAX
#include <windows.h>
#undef near
#undef far
#include <gl/GL.h>



extern PFNGLDRAWRANGEELEMENTSWINPROC glDrawRangeElements;
extern PFNGLGENBUFFERSARBPROC glGenBuffers;
extern PFNGLBINDBUFFERARBPROC glBindBuffer;
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffers;
extern PFNGLBUFFERDATAARBPROC glBufferData;
extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTexture;

extern int maxIndices;

#endif

