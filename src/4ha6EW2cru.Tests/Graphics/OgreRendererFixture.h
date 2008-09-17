#ifndef __OGRERENDERER_FIXTURE_H
#define __OGRERENDERER_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "Graphics/IRenderer.hpp"

class OgreRendererFixture : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( OgreRendererFixture );
	CPPUNIT_TEST( Should_Intialize_Correctly );
	CPPUNIT_TEST( Should_Throw_ScreenDimensionsException_On_Initialize_Given_Negative_Dimensions );
	CPPUNIT_TEST( Should_Throw_ScreenDimensionsException_On_Initialize_Given_Zero_Dimensions );
	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Intialize_Correctly( );
	void Should_Throw_ScreenDimensionsException_On_Initialize_Given_Negative_Dimensions( );
	void Should_Throw_ScreenDimensionsException_On_Initialize_Given_Zero_Dimensions( );

private:

	IRenderer* _renderer;

};

#endif