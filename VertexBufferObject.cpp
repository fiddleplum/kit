#include "VertexBufferObject.h"
#include "OpenGL.h"

/*
Notes:

GL_STREAM_DRAW when vertices data could be updated between each rendering.
GL_DYNAMIC_DRAW when vertices data could be updated between each frames.
GL_STATIC_DRAW when vertices data are never or almost never updated.
glClientActiveTextureARB for multitextures
*/
VertexBufferObject::VertexBufferObject(Mesh const& mesh, bool dynamic_)
: dynamic(dynamic_), vertex_format(mesh->get_vertex_format()), num_indices(mesh->indices.size()) {

  // Initialize the ARB extensions, if needed.
  if(glGenBuffersARB == NULL) {
    if(!glIsExtensionSupported("GL_ARB_vertex_buffer_object") || !glIsExtensionSupported("GL_ARB_multitexture"))
      throw std::runtime_error("Vertex Buffer Objects and/or Multitexturing are not available. Upgrade your computer's graphics capabilities. ");
    glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
    glBindBufferARB = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
    glBufferDataARB = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
    glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
    glClientActiveTextureARB = (PFNGCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress("glClientActiveTextureARB");
  }

  stride = mesh->get_vertex_size();

  // Generate the vertex buffer id
  glGenBuffersARB(1, &id);

  // Tell OpenGL that we're using the vertex buffer.
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, id);

  // Set usage based on if it is dynamic.
  GLenum usage;
  if(dynamic)
    usage = GL_DYNAMIC_DRAW_ARB;
  else
    usage = GL_STATIC_DRAW_ARB;

  // Copy the data over.
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, mesh->vertices.size(), &mesh->vertices[0], usage);

  // Generate the index buffer id
  glGenBuffersARB(1, &index_id);

  // Tell OpenGL that we're using the index buffer.
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, index_id);

  // Copy the data over.
  glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(unsigned int) * num_indices, &mesh->indices[0], usage);
}

VertexBufferObject::~VertexBufferObject() {
  // Delete the buffers.
  glDeleteBuffersARB(1, &index_id);
  glDeleteBuffersARB(1, &id);
}

void VertexBufferObject::draw() {
  // Tell OpenGL that we're using the vertex buffer.
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, id);

  unsigned int texture_stream = 0;
  unsigned int offset = 0;
  for(unsigned int i = 0; i < vertex_format.size() - 1; i += 2) {
    unsigned int dim = vertex_format[i+1] - '0';
    if(vertex_format[i] == 'v') {
      glVertexPointer(dim, GL_FLOAT, stride, (void const*)offset);
      glEnableClientState(GL_VERTEX_ARRAY);
    } else if(vertex_format[i] == 'n') {
      glNormalPointer(GL_FLOAT, stride, (void const*)offset);
      glEnableClientState(GL_NORMAL_ARRAY);
    } else if(vertex_format[i] == 't') {
      glClientActiveTextureARB(GL_TEXTURE0_ARB + texture_stream);
      glTexCoordPointer(dim, GL_FLOAT, stride, (void const*)offset);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      texture_stream++;
    }
    offset += sizeof(float) * dim;
  }

  // Tell OpenGL that we're using the index buffer.
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, index_id);

  glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
}

void VertexBufferObject::update() {
  // glBufferSubDataARB
}

