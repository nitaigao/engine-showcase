/** @file
    @brief  Unit VerifyingTestCaller with verification

  $Id: VerifyingTestCaller.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sun Jan 23 2005
   copyright            : (C) 2002-2008 by Ewald Arnold
   email                : mockpp at ewald-arnold dot de

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

 **/

#ifndef MOCKPP_VerifyingTestCaller_H
#define MOCKPP_VerifyingTestCaller_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/MockObject.h>

#include <mockpp/framework/VerifyingTestCase.h>

#ifdef MOCKPP_USE_CPPUNIT

#include <cppunit/TestCaller.h>


MOCKPP_NS_START


/** VerifyingTestCaller that verifies Verifiable
  * fields and registered Verifiable objects after the test has run and before the fixture
  * has been torn down.
  * @ingroup grp_framework
  * @ingroup grp_basic_mo
  * @ingroup grp_advanced_mo
  */
template <typename VerifyingFixtureType, bool DoTheVerify>
class VerifyingTestCaller : public ::CppUnit::TestCaller<VerifyingFixtureType>
{
  typedef void (VerifyingFixtureType::*TestCaseMethod)();

  public:

  /** Constructor for VerifyingTestCaller.
    * This constructor does not create a new Fixture instance but accepts
    * an existing one as parameter. The VerifyingTestCaller will not own the
    * Fixture object.
    * \param name name of this VerifyingTestCaller
    * \param test the method this VerifyingTestCaller calls in runTest()
    * \param fixture the Fixture to invoke the test method on.
    */
    VerifyingTestCaller(const MOCKPP_STD_NS::string &name,
                        TestCaseMethod test,
                        VerifyingFixtureType& fixture)
      : ::CppUnit::TestCaller<VerifyingFixtureType>(name, test, fixture)
      , testcase(&fixture)
    {
    }

  /** Constructor for VerifyingTestCaller.
    * This constructor does not create a new Fixture instance but accepts
    * an existing one as parameter. The VerifyingTestCaller will own the
    * Fixture object and delete it in its destructor.
    * \param name name of this VerifyingTestCaller
    * \param test the method this VerifyingTestCaller calls in runTest()
    * \param fixture the Fixture to invoke the test method on.
    */
    VerifyingTestCaller(const MOCKPP_STD_NS::string &name,
                        TestCaseMethod test,
                        VerifyingFixtureType* fixture)
	  : ::CppUnit::TestCaller<VerifyingFixtureType>(name, test, fixture)
      , testcase(fixture)
    {
    }

  /** Runs the test method with mock object verification afterwards.
    */
    virtual void runTest()
    {
      testcase->unregisterVerifiables();
      MOCKPP_TRY
      {
        ::CppUnit::TestCaller<VerifyingFixtureType>::runTest();
        bool doVerify = DoTheVerify;
        if (doVerify)
          testcase->verify();
      }
#ifndef MOCKPP_NO_EXCEPTIONS
      catch(...)
      {
        testcase->unregisterVerifiables();
        MOCKPP_RETHROW;
      }
#endif
      testcase->unregisterVerifiables();
    }

  /** Returns the the callers string representation.
    */
    MOCKPP_STL::string toString() const
    {
      return "VerifyingTestCaller " + this->getName();
    }

  private:

    VerifyingTestCase *testcase;

    VerifyingTestCaller( const VerifyingTestCaller &other );
    VerifyingTestCaller &operator =( const VerifyingTestCaller &other );
};


MOCKPP_NS_END


/** Add a method to the suite and verify after its run.
  * Substitutes CPPUNIT_TEST for cases where you wish to call \c verify()
  * automatically after test completion.
  * \param testMethod Name of the method of the test case to add to the
  *                   suite. The signature of the method must be of
  *                   type: void testMethod();
  * \see  CPPUNIT_TEST.
  */
#define MOCKPP_VERIFYING_TEST( testMethod ) \
    CPPUNIT_TEST_SUITE_ADD_TEST(  \
        ( new MOCKPP_NS::VerifyingTestCaller<TestFixtureType, true>( \
                  context.getTestNameFor( #testMethod),   \
                  &TestFixtureType::testMethod,           \
                  context.makeFixture() ) ) )


/** Add a method to the suite (but do NOT verify after its run).
  * Substitutes CPPUNIT_TEST.
  * \param testMethod Name of the method of the test case to add to the
  *                   suite. The signature of the method must be of
  *                   type: void testMethod();
  * \see  CPPUNIT_TEST.
  */
#define MOCKPP_TEST( testMethod ) \
    CPPUNIT_TEST_SUITE_ADD_TEST(  \
        ( new MOCKPP_NS::VerifyingTestCaller<TestFixtureType, false>( \
                  context.getTestNameFor( #testMethod),   \
                  &TestFixtureType::testMethod,           \
                  context.makeFixture() ) ) )


/** Add a test which verifies after its run and fails if the specified
  * exception is not caught.
  * Substitutes CPPUNIT_TEST_EXCEPTION for cases where you wish to call \c verify()
  * automatically after test completion.
  * \param testMethod Name of the method of the test case to add to the suite.
  * \param ExceptionType Type of the exception that must be thrown by the test
  *                      method.
  * \see  CPPUNIT_TEST_EXCEPTION.
  * \deprecated Use one of the assertion macros like MOCKPP_ASSERT_THROWING instead.
  */
#define MOCKPP_VERIFYING_TEST_EXCEPTION( testMethod, ExceptionType )  \
  CPPUNIT_TEST_SUITE_ADD_TEST(                                        \
      (new ::CppUnit::ExceptionTestCaseDecorator< ExceptionType >(      \
          new MOCKPP_NS::VerifyingTestCaller< TestFixtureType, true >(   \
                               context.getTestNameFor( #testMethod ), \
                               &TestFixtureType::testMethod,          \
                               context.makeFixture() ) ) ) )

/** Add a test (which does NOT verify after its run) and fails if the specified
  * exception is not caught.
  * Substitutes CPPUNIT_TEST_EXCEPTION.
  * \param testMethod Name of the method of the test case to add to the suite.
  * \param ExceptionType Type of the exception that must be thrown by the test
  *                      method.
  * \see  CPPUNIT_TEST_EXCEPTION.
  * \deprecated Use one of the assertion macros like MOCKPP_ASSERT_THROWING instead.
  */
#define MOCKPP_TEST_EXCEPTION( testMethod, ExceptionType )  \
  CPPUNIT_TEST_SUITE_ADD_TEST(                                        \
      (new ::CppUnit::ExceptionTestCaseDecorator< ExceptionType >(      \
          new MOCKPP_NS::VerifyingTestCaller< TestFixtureType, false >(  \
                               context.getTestNameFor( #testMethod ), \
                               &TestFixtureType::testMethod,          \
                               context.makeFixture() ) ) ) )

#endif // MOCKPP_USE_CPPUNIT

#endif // MOCKPP_VerifyingTestCaller_H

