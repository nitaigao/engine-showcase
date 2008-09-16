#ifndef __IRENDERER_H
#define __IRENDERER_H

#include "MyGUI.h"
using namespace MyGUI;

class IRenderer
{

public:

	virtual ~IRenderer( ) { };

	virtual bool Initialize( int width, int height, bool fullScreen ) = 0;
	virtual void Render( ) const = 0;
	virtual size_t GetHwnd( ) const = 0;
	virtual Gui* GetGui( ) = 0; // TODO: This is just plain wrong, i think the GUi should live outside of the renderer
	virtual void Update( ) const = 0;

};

#endif