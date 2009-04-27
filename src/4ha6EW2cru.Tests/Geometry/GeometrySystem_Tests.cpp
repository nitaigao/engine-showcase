#include "GeometrySystem_Tests.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( GeometrySystem_Tests, Suites::GeometrySuite( ) );

#include "Geometry/GeometrySystem.h"
using namespace Geometry;

#include "System/ISystemScene.hpp"

void GeometrySystem_Tests::Should_Return_GeometrySystemScene_On_CreateScene( )
{
	GeometrySystem system;
	ISystemScene* scene = system.CreateScene( );

	CPPUNIT_ASSERT( scene != 0 );

	delete scene;
}