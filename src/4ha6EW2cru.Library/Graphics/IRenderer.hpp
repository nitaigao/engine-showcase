#ifndef __IRENDERER_H
#define __IRENDERER_H

#include "MyGUI.h"
using namespace MyGUI;

class IRenderer
{

public:

	virtual ~IRenderer( ) { };

	virtual void Initialize( int width, int height, int colorDepth, bool fullScreen ) = 0;
	virtual void Render( ) const = 0;
	virtual size_t GetHwnd( ) const = 0;
	virtual void Update( float deltaMilliseconds ) const = 0;
	virtual void ChangeResolution( int width, int height, bool fullscreen ) = 0;

};

#endif
