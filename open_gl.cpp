#include "open_gl.h"
#include <vector>

#ifdef _WIN32

#pragma message("----opengl32.lib will be linked in as a static library.----")
#pragma comment(lib, "opengl32.lib")

#endif

std::vector<Box2i> clipStack;

void pushClipBounds (Box2i bounds)
{
  glScissor(bounds.min[0], bounds.min[1], bounds.getSize()[0], bounds.getSize()[1]);
  clipStack.push_back(bounds);
}

void popClipBounds ()
{
  clipStack.pop_back();
  if (clipStack.empty() == false)
  {
    glScissor(clipStack.back().min[0], clipStack.back().min[1], clipStack.back().getSize()[0], clipStack.back().getSize()[1]);
  }
}

