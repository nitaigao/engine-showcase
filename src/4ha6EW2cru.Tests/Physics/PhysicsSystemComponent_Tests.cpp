#include "PhysicsSystemComponent_Tests.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( PhysicsSystemComponent_Tests, Suites::PhysicsSuite( ) );

#include "Physics/PhysicsSystemComponent.h"
#include "Geometry/GeometrySystemComponent.h"

void PhysicsSystemComponent_Tests::Should_Observe_A_Geometry_Change()
{
	GeometrySystemComponent geomComponent( "Test Component" );

	PhysicsSystemComponent component( "Test Physics Component", 0 );
	component.Observe( &geomComponent );
}