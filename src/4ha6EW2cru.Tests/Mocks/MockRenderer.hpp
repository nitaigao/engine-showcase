#ifndef __MOCKRENDERER_H
#define __MOCKRENDERER_H

#include "Graphics/IRenderer.hpp"

#include <Windows.h>

#include "mockpp/mockpp.h"
#include "mockpp/MockObject.h"
#include "mockpp/ExpectationCounter.h"

USING_NAMESPACE_MOCKPP

class MockRenderer : public IRenderer, public MockObject
{

public:

	MockRenderer( )
		: MockObject( "MockEventTrigger", 0 )
	{

	}

	bool Initialize( int width, int height, bool fullScreen ) { return true; };
	void Render( ) const { };
	size_t GetHwnd( ) const { return ( size_t )GetConsoleWindow( ); };
	Gui* GetGui( ) { return 0; };
	void Update( ) const { };

};

#endif