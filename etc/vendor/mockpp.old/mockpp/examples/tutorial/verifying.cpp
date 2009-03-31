/***************************************************************************
           verifying.cpp  -  example usage of verifying test case

                             -------------------
    begin                : Sat 22 Jan 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: verifying.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

#include <mockpp/mockpp.h> // always first

#include <iostream>

#ifdef MOCKPP_USE_CPPUNIT

#include <mockpp/framework/VerifyingTestCase.h>

#include <mockpp/ExpectationValue.h>

#include <mockpp/compat/AssertionFailedError.h>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Calculator.h"

/** A helper class for the unit tests
  * @internal
  */
class VerifyingCalculator_test : public MOCKPP_NS::VerifyingTestCase
{
 public:

   CPPUNIT_TEST_SUITE( VerifyingCalculator_test );
     MOCKPP_TEST(test_ok_no_verify);
     MOCKPP_VERIFYING_TEST(test_ok);
     MOCKPP_VERIFYING_TEST_EXCEPTION(test_fail, MOCKPP_NS::AssertionFailedError);
     MOCKPP_TEST_EXCEPTION(test_fail_no_verify, MOCKPP_NS::AssertionFailedError);
   CPPUNIT_TEST_SUITE_END();

 public:

   void test_ok();
   void test_fail();
   void test_ok_no_verify();
   void test_fail_no_verify();
};


// Automatically register tests
CPPUNIT_TEST_SUITE_REGISTRATION(VerifyingCalculator_test);


void VerifyingCalculator_test::test_ok_no_verify()
{
  MOCKPP_NS::ExpectationValue<int> ev (MOCKPP_PCHAR("ev"), this);
  ev.setExpected(123);
  ev.setActual(123);
  verify();
}


void VerifyingCalculator_test::test_ok()
{
  MOCKPP_NS::ExpectationValue<int> *ev = new MOCKPP_NS::ExpectationValue<int>(MOCKPP_PCHAR("ev"));
  registerVerifiable(ev);
  ev->setExpected(123);
  ev->setActual(123);
  ev->verify();

  MOCKPP_NS::ExpectationValue<int> ev2 (MOCKPP_PCHAR("ev2"), this);
  ev2.setExpected(123);
  ev2.setActual(123);
  ev2.verify();

  MOCKPP_NS::ExpectationValue<int> ev3 (MOCKPP_PCHAR("ev3"));
  ev3.setExpected(123);
  ev3.setActual(123);
  ev3.verify();

  MOCKPP_NS::ExpectationValue<int> *ev4 = new MOCKPP_NS::ExpectationValue<int>(MOCKPP_PCHAR("ev"));
  registerVerifiable(ev4);
  ev4->setExpected(123);
  ev4->setActual(123);
  ev4->verify();

  verify();
}


void VerifyingCalculator_test::test_fail_no_verify()
{
  MOCKPP_NS::ExpectationValue<int> ev(MOCKPP_PCHAR("ev"), 0) ;
  ev.setExpected(123);
  ev.setActual(123456);
}


void VerifyingCalculator_test::test_fail()
{
  MOCKPP_NS::ExpectationValue<int> *ev = new MOCKPP_NS::ExpectationValue<int>(MOCKPP_PCHAR("ev"));
  registerVerifiable(ev);
  ev->setExpected(123);
  // missing actual
}

#endif // MOCKPP_USE_CPPUNIT

int main(int argc, char ** /*argv*/)
{
#ifdef MOCKPP_USE_CPPUNIT
  if (argc != 1)
    global_error = argc;  // disturb tests

  // create tests and run them
  CppUnit::TextUi::TestRunner runner;
  CppUnit::TestFactory &registry = CppUnit::TestFactoryRegistry::getRegistry();
  runner.addTest(registry.makeTest());
  return runner.run() ? 0 : 1;
#else
  MOCKPP_STD_NS::cout << "CppUnit was not available at compile time.\n";
  return 0;
#endif
}
