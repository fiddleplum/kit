#include "OpenGL.h"
#include <vector>

#ifdef _WIN32

#pragma message("\n----Depends on opengl32.lib.----")

#endif

bool glIsExtensionSupported( char* szTargetExtension )
{
	const unsigned char *pszExtensions = NULL;
	const unsigned char *pszStart;
	unsigned char *pszWhere, *pszTerminator;

	// Extension names should not have spaces
	pszWhere = (unsigned char *) strchr( szTargetExtension, ' ' );
	if( pszWhere || *szTargetExtension == '\0' )
		return false;

	// Get Extensions String
	pszExtensions = glGetString( GL_EXTENSIONS );

	// Search The Extensions String For An Exact Copy
	pszStart = pszExtensions;
	for(;;)
	{
		pszWhere = (unsigned char *) strstr( (const char *) pszStart, szTargetExtension );
		if( !pszWhere )
			break;
		pszTerminator = pszWhere + strlen( szTargetExtension );
		if( pszWhere == pszStart || *( pszWhere - 1 ) == ' ' )
			if( *pszTerminator == ' ' || *pszTerminator == '\0' )
				return true;
		pszStart = pszTerminator;
	}
	return false;
}

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

