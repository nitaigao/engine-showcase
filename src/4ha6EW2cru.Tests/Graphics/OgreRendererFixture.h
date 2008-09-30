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
	CPPUNIT_TEST( Should_Render_Given_Initialized );
	CPPUNIT_TEST( Should_Throw_On_Render_Given_Not_Intiailized );
	CPPUNIT_TEST( Should_Update_Given_Initialized );
	CPPUNIT_TEST( Should_Throw_On_Update_Given_Not_Intialized );
	CPPUNIT_TEST( Should_Return_Positive_Hwnd );
	CPPUNIT_TEST( Should_Throw_On_GetHWND_Given_Not_Intialized );
	CPPUNIT_TEST( Should_Throw_On_GetGUI_Given_Not_Initialized );
	CPPUNIT_TEST( Should_Return_GUI_Given_Initialized );
	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Intialize_Correctly( );
	void Should_Throw_ScreenDimensionsException_On_Initialize_Given_Negative_Dimensions( );
	void Should_Throw_ScreenDimensionsException_On_Initialize_Given_Zero_Dimensions( );
	void Should_Render_Given_Initialized( );
	void Should_Throw_On_Render_Given_Not_Intiailized( );
	void Should_Update_Given_Initialized( );
	void Should_Throw_On_Update_Given_Not_Intialized( );
	void Should_Return_Positive_Hwnd( );
	void Should_Throw_On_GetHWND_Given_Not_Intialized( );
	void Should_Throw_On_GetGUI_Given_Not_Initialized( );
	void Should_Return_GUI_Given_Initialized( );


private:

	IRenderer* _renderer;

};

#endif