#include "PhysicsSystemComponent_Tests.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( PhysicsSystemComponent_Tests, Suites::PhysicsSuite( ) );

#include "Physics/PhysicsSystemComponent.h"

void PhysicsSystemComponent_Tests::Should_Observe_A_Geometry_Change()
{
	/*GeometrySystemComponent geomComponent( "Test Component" );

	PhysicsSystemComponent component( "Test Physics Component", 0 );
	component.Observe( &geomComponent, System::Changes::Geometry::Position );*/

	unsigned int SomeValue = ( 1 << 0 );
	unsigned int AnotherValue = ( 1 << 1 );
	unsigned int LastValue = ( 1 << 2 );

	unsigned int changes = SomeValue | AnotherValue | LastValue;

	unsigned int theResult1 = changes & SomeValue;
	unsigned int theResult2 = changes & AnotherValue;

	bool testResult = ( changes & SomeValue );
	bool testResult2 = ( changes & AnotherValue );
	bool testResult3 = ( changes & LastValue );

	float a = 1.57079633;


	float _x = (1 * cos(a)) + (0 * -sin(a));
	float _y = (0 * sin(a)) + (1 * cos(a));
	float _z = 1;
}