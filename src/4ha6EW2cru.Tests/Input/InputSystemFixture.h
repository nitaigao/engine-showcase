#ifndef __INPUTSYSTEM_FIXTURE_H
#define __INPUTSYSTEM_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "Graphics/IRenderer.hpp"
#include "Input/IInputSystem.hpp"

class InputSystemFixture : public CPPUNIT_NS::TestFixture
{

  CPPUNIT_TEST_SUITE( InputSystemFixture );
  CPPUNIT_TEST( Should_Initialize_Properly );
  CPPUNIT_TEST( Should_Throw_Given_Already_Initialized );
  CPPUNIT_TEST( Should_Not_Initialize_With_NULL_HWND );

  CPPUNIT_TEST( Should_Throw_Given_Invalid_Capture_Area );
  CPPUNIT_TEST( Should_Accept_Valid_Capture_Area );

  CPPUNIT_TEST( Should_Throw_Given_Unitialized_SetCaptureArea );
  
  CPPUNIT_TEST( Should_Update_Given_Initialized );
  CPPUNIT_TEST( Should_Throw_Given_UnIntialized_Update );

  CPPUNIT_TEST_SUITE_END( );

public:

	void setUp ( void );
	void tearDown ( void );

protected:

  void Should_Initialize_Properly( );
  void Should_Throw_Given_Already_Initialized( );
  void Should_Not_Initialize_With_NULL_HWND( );
  void Should_Throw_Given_Invalid_Capture_Area( );
  void Should_Accept_Valid_Capture_Area( );
  void Should_Throw_Given_Unitialized_SetCaptureArea( );
  void Should_Update_Given_Initialized( );
  void Should_Throw_Given_UnIntialized_Update( );

private:

	IRenderer* _renderer;
	IInputSystem* _inputSystem;

};

#endif