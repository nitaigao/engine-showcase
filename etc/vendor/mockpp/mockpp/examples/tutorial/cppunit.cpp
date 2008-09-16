/***************************************************************************
             cppunit.cpp  -  example usage of cppunit

                             -------------------
    begin                : Sun 2 Jan 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: cppunit.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

#include <mockpp/mockpp.h> // always first

#include <mockpp/Throwable.h>
#include <mockpp/visiting/VisitableMockObject.h>
#include <mockpp/chaining/ChainableMockObject.h>
#include <mockpp/chaining/ChainingMockObjectSupport.h>

#if defined(MOCKPP_USE_CPPUNIT)

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#endif

#include "Calculator.h"

using MOCKPP_NS::eq;
using MOCKPP_NS::once;
using MOCKPP_NS::exactly;
using MOCKPP_NS::returnValue;
using MOCKPP_NS::throwException;

/** A Chainable mock object.
  * @internal
  */
class ChainDemoMock : public MOCKPP_NS::ChainableMockObject
{
  public:

    ChainDemoMock()
      : MOCKPP_NS::ChainableMockObject(MOCKPP_PCHAR("ChainDemoMock"), 0)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT2(add, ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE0(network_read)
    {}

    MOCKPP_CHAINABLE_EXT2(ChainDemoMock, int, add, int, int,
                                         int, ext, int, int);

    MOCKPP_CHAINABLE0(ChainDemoMock, int, network_read);
};


/** A Visitable mock object.
  * @internal
  */
class VisitDemoMock : public MOCKPP_NS::VisitableMockObject
{
  public:

    VisitDemoMock()
      : MOCKPP_NS::VisitableMockObject(MOCKPP_PCHAR("VisitDemoMock"), 0)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT2(add, ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE0(network_read)
    {}

    MOCKPP_VISITABLE_EXT2(VisitDemoMock, int, add, int, int,
                                         int, ext, int, int);

    MOCKPP_VISITABLE0(VisitDemoMock, int, network_read);
};

/** A helper class for the unit tests
  * @internal
  */
class Calculator_test 
#if defined(MOCKPP_USE_CPPUNIT)
	: public CppUnit::TestFixture
#endif

{
 public:

#if defined(MOCKPP_USE_CPPUNIT)

  CPPUNIT_TEST_SUITE( Calculator_test );
     CPPUNIT_TEST(test_add);
     CPPUNIT_TEST(test_sub);
     CPPUNIT_TEST(test_add_v);
     CPPUNIT_TEST(test_add_c);
     CPPUNIT_TEST(test_network_read_v);
     CPPUNIT_TEST(test_network_read_c);
   CPPUNIT_TEST_SUITE_END();

#endif

 public:

   void test_add();
   void test_add_v();
   void test_add_c();
   void test_sub();
   void test_network_read_v();
   void test_network_read_c();
};

#if defined(MOCKPP_USE_CPPUNIT)

// Automatically register tests
CPPUNIT_TEST_SUITE_REGISTRATION(Calculator_test);

#endif

// Test add(int) method
void Calculator_test::test_add()
{
  Calculator calc(100);
  MOCKPP_ASSERT_EQUALS(123, calc.add(23));
}

// Test add(int, int) method
void Calculator_test::test_add_c()
{
  ChainDemoMock mock;
  MOCKPP_CHAINER_FOR_EXT(ChainDemoMock, add, ext) add_chainer (&mock);

  add_chainer.stubs()
             .with(eq(1), eq(2))
             .will(returnValue(3));

  add_chainer.stubs()
             .with(eq(99), eq(11))
             .will(returnValue(110));

  add_chainer.stubs()
             .will(returnValue(-1));

  MOCKPP_ASSERT_EQUALS(3, mock.add(1, 2));
  MOCKPP_ASSERT_EQUALS(110, mock.add(99, 11));
  MOCKPP_ASSERT_EQUALS(-1, mock.add(99, 12));
  MOCKPP_ASSERT_EQUALS(-1, mock.add(98, 11));

  mock.verify();
}


// Test add(int, int) method
void Calculator_test::test_add_v()
{
  VisitDemoMock mock;
  MOCKPP_CONTROLLER_FOR_EXT(VisitDemoMock, add, ext) add_controller (&mock);

  add_controller.addResponseValue(3, 1, 2);      // 1 and 2 are expected
  add_controller.addResponseValue(110, 99, 11);  // 99 and 11 are expected
  add_controller.setDefaultReturnValue(-1);

  mock.add(1, 2);
  mock.add(99, 11);
  mock.add(99, 12);
  mock.add(98, 11);

  mock.activate();

  MOCKPP_ASSERT_EQUALS(3, mock.add(1, 2));
  MOCKPP_ASSERT_EQUALS(110, mock.add(99, 11));
  MOCKPP_ASSERT_EQUALS(-1, mock.add(99, 12));
  MOCKPP_ASSERT_EQUALS(-1, mock.add(98, 11));

  mock.verify();
}


// Test read() method
void Calculator_test::test_network_read_c()
{
    ChainDemoMock mock;
    MOCKPP_CHAINER_FOR(ChainDemoMock, network_read) read_chainer (&mock);

    read_chainer.stubs(exactly(10))
              .will(returnValue(0));

    read_chainer.stubs(once())
                .will(throwException<int>(NetworkError()));

    read_chainer.stubs()
              .will(returnValue(1));

    MOCKPP_ASSERT_EQUALS(0, mock.network_read());
    MOCKPP_ASSERT_EQUALS(0, mock.network_read());
    MOCKPP_ASSERT_EQUALS(0, mock.network_read());
    MOCKPP_ASSERT_EQUALS(0, mock.network_read());
    MOCKPP_ASSERT_EQUALS(0, mock.network_read());
    MOCKPP_ASSERT_EQUALS(0, mock.network_read());
    MOCKPP_ASSERT_EQUALS(0, mock.network_read());
    MOCKPP_ASSERT_EQUALS(0, mock.network_read());
    MOCKPP_ASSERT_EQUALS(0, mock.network_read());
    MOCKPP_ASSERT_EQUALS(0, mock.network_read());

    try
    {
      mock.network_read();
      MOCKPP_FAIL(MOCKPP_PCHAR("should have thrown here"));
    }
    catch(const NetworkError &/*ex*/)
    {
    }

    MOCKPP_ASSERT_EQUALS(1, mock.network_read());
    MOCKPP_ASSERT_EQUALS(1, mock.network_read());

    mock.verify();
}


// Test read() method
void Calculator_test::test_network_read_v()
{
  VisitDemoMock mock;
    MOCKPP_CONTROLLER_FOR(VisitDemoMock, network_read) read_controller (&mock);

    mock.network_read();
    mock.network_read();
    mock.network_read();
    mock.network_read();
    mock.network_read();
    mock.network_read();
    mock.network_read();
    mock.network_read();
    mock.network_read();
    mock.network_read();

    mock.network_read();

    mock.network_read();
    mock.network_read();

    read_controller.setDefaultReturnValue(1);
    read_controller.addReturnValue(0, 10);
    read_controller.addThrowable(MOCKPP_NS::make_throwable(NetworkError()));

    mock.activate();

    MOCKPP_ASSERT_EQUALS(0, mock.network_read());
    MOCKPP_ASSERT_EQUALS(0, mock.network_read());
    MOCKPP_ASSERT_EQUALS(0, mock.network_read());
    MOCKPP_ASSERT_EQUALS(0, mock.network_read());
    MOCKPP_ASSERT_EQUALS(0, mock.network_read());
    MOCKPP_ASSERT_EQUALS(0, mock.network_read());
    MOCKPP_ASSERT_EQUALS(0, mock.network_read());
    MOCKPP_ASSERT_EQUALS(0, mock.network_read());
    MOCKPP_ASSERT_EQUALS(0, mock.network_read());
    MOCKPP_ASSERT_EQUALS(0, mock.network_read());

    try
    {
      mock.network_read();
      MOCKPP_FAIL(MOCKPP_PCHAR("should have thrown here"));
    }
    catch(const NetworkError &/*ex*/)
    {
    }

    MOCKPP_ASSERT_EQUALS(1, mock.network_read());
    MOCKPP_ASSERT_EQUALS(1, mock.network_read());

    mock.verify();
}


// Test sub() method
void Calculator_test::test_sub()
{
  Calculator calc(100);
  MOCKPP_ASSERT_EQUALS(78, calc.sub(22));
}


int main(int argc, char ** /*argv*/)
{
  if (argc != 1)
    global_error = argc;  // disturb tests

#if defined(MOCKPP_USE_CPPUNIT)
  // create tests and run them
  CppUnit::TextUi::TestRunner runner;
  CppUnit::TestFactory &registry = CppUnit::TestFactoryRegistry::getRegistry();
  runner.addTest(registry.makeTest());
  return runner.run() ? 0 : 1;
#else
  return 0;
#endif
}

