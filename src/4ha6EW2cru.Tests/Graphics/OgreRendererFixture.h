#ifndef __OGRERENDERER_FIXTURE_H
#define __OGRERENDERER_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class OgreRendererFixture : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( OgreRendererFixture );
	CPPUNIT_TEST( Should_Intialize_Correctly );
	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Intialize_Correctly( );

};

#endif