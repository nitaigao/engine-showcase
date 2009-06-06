#include "AISystemComponent_Tests.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( AISystemComponent_Tests, Suites::AISuite( ) );

#include "Logging/Logger.h"
using namespace Logging;

#include "Management/Management.h"

#include "AI/AISystemComponent.h"
using namespace AI;

void AISystemComponent_Tests::setUp()
{
	Management::Initialize( );
	Management::GetFileManager( )->Mount( "../../../etc/data", "/" );
}

void AISystemComponent_Tests::tearDown()
{
	Management::Release( );
}

void AISystemComponent_Tests::Should_Return_Name_On_GetName()
{
	std::string name = "test";
	ISystemComponent* component = new AISystemComponent( name );

	CPPUNIT_ASSERT( component->GetAttributes( )[ System::Attributes::Name ].As< std::string >( ) == name );

	delete component;
}

void AISystemComponent_Tests::Should_Set_BehaviorName_On_SetBehavior()
{
	AISystemComponent* component = new AISystemComponent( "test" );
	
	AnyType::AnyTypeMap properties;
	//component->Initialize( properties );

	//component->SetBehavior( "walking" );
	//CPPUNIT_ASSERT( component->GetBehavior( ) == "walking" );

	delete component;
}