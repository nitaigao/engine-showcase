#ifndef __HUICONTROLLERFIXTURE_H
#define __GUICONTROLLERFIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "Graphics/IRenderer.hpp"

class GuiControllerFixture : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( GuiControllerFixture );

	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

private:

	IRenderer* _renderer;

};

#endif