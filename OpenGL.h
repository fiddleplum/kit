#pragma once

#include <SDL_opengl.h>
#include "Box.h"

void pushClipBounds (Box2i bounds);

void popClipBounds ();

#ifdef _WIN32

#define NOMINMAX
#include <windows.h>
#undef near
#undef far
#include <gl/GL.h>

#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8

typedef void (APIENTRY * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint * buffers);
typedef void (APIENTRY * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, GLuint const * buffers);
typedef void (APIENTRY * PFNGLBUFFERDATAARBPROC) (GLenum target, int size, GLvoid const * data, GLenum usage);
typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum texture);

extern PFNGLDRAWRANGEELEMENTSWINPROC glDrawRangeElements;
extern PFNGLGENBUFFERSARBPROC glGenBuffers;
extern PFNGLBINDBUFFERARBPROC glBindBuffer;
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffers;
extern PFNGLBUFFERDATAARBPROC glBufferData;
extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTexture;

extern int maxIndices;

#endif

