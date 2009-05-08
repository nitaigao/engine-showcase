#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>

#include "TeamCity/teamcity_cppunit.h"
#include "TeamCity/teamcity_messages.h"

#include "Suites.h"

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#define _CRTDBG_MAP_ALLOC_NEW
	#define new new ( _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif	

int main (int argc, char* argv[])
{
	//_crtBreakAlloc = 13611;
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	// Create the event manager and test controller
	CPPUNIT_NS::TestResult controller;

	// Add a listener that collects test result
	CPPUNIT_NS::TestResultCollector result;
	controller.addListener( &result );        

	// Add a listener that print dots as test run.
	CPPUNIT_NS::TestListener* progress;

	if ( JetBrains::underTeamcity( ) ) 
	{
		progress = new JetBrains::TeamcityProgressListener();
	} 
	else 
	{
		progress = new CPPUNIT_NS::BriefTestProgressListener();
	}     

	controller.addListener( progress ); 

	// Add the top suite to the test runner
	CPPUNIT_NS::TestRunner runner;

	// BDD

	runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry( ).makeTest( ) );

	//TDD

	runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry( Suites::SoundSuite( ) ).makeTest( ) );
	runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry( Suites::EventsSuite( ) ).makeTest( ) );
	runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry( Suites::InputSuite( ) ).makeTest( ) );
	runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry( Suites::LoggingSuite( ) ).makeTest( ) );
	runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry( Suites::ScriptingSuite( ) ).makeTest( ) );
	runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry( Suites::UtilitySuite( ) ).makeTest( ) );
	//runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry( Suites::IOSuite( ) ).makeTest( ) );
	runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry( Suites::ViewSuite( ) ).makeTest( ) );
	runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry( Suites::SystemSuite( ) ).makeTest( ) );
	runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry( Suites::StateSuite( ) ).makeTest( ) );
	runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry( Suites::GeometrySuite( ) ).makeTest( ) );
	runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry( Suites::PhysicsSuite( ) ).makeTest( ) );
	runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry( Suites::AISuite( ) ).makeTest( ) );
	//runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry( Suites::GraphicsSuite( ) ).makeTest( ) );

	runner.run( controller );

	// Print test in a compiler compatible format.
	CPPUNIT_NS::CompilerOutputter outputter( &result, CPPUNIT_NS::stdCOut( ) );
	outputter.write( ); 

	delete progress;

	return result.wasSuccessful( ) ? 0 : 1;
}