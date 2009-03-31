/***************************************************************************
   TrackingCounter_test.cpp  -  unit tests for TrackingCounter class
                             -------------------
    begin                : Fri Feb 18 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: TrackingCounter_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not,
 write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 ***************************************************************************/

#include <mockpp/mockpp.h> // always first

#include "SelectFramework.h"

#include <mockpp/TrackingCounter.h>
#include <mockpp/compat/Asserter.h>
#include <mockpp/util/AssertMo.h>


class TrackingCounter_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( TrackingCounter_test );

 CPPUNIT_TEST(test_hasExpectations);
 CPPUNIT_TEST(test_name);
 CPPUNIT_TEST(test_clearActual);
 CPPUNIT_TEST(test_clear);
 CPPUNIT_TEST(test_add_remove);
 CPPUNIT_TEST(test_counter);

 CPPUNIT_TEST(test_expectNothing);
 CPPUNIT_TEST(test_expectNothing_revoked);
 CPPUNIT_TEST(test_expectActualImmediate_client);
 CPPUNIT_TEST(test_expectActualImmediate_master);
 CPPUNIT_TEST(test_expectActualVerify);
 CPPUNIT_TEST(test_ifNoExpected);

 CPPUNIT_TEST(fail_expectActualImmediate_client);
 CPPUNIT_TEST(test_expectActualImmediate_client_verify);
 CPPUNIT_TEST(fail_expectActualImmediate_master);
 CPPUNIT_TEST(test_master_verify);
 CPPUNIT_TEST(fail_expectActualVerify);
 CPPUNIT_TEST(fail_expectNothing);

 CPPUNIT_TEST(fail_ifNoActual_master);
 CPPUNIT_TEST(fail_ifNoActual_client);

CPPUNIT_TEST_SUITE_END();

#endif

 public:
  MOCKPP_TESTMETHOD_DECL void test_expectNothing();
  MOCKPP_TESTMETHOD_DECL void test_expectNothing_revoked();
  MOCKPP_TESTMETHOD_DECL void fail_expectNothing();

  MOCKPP_TESTMETHOD_DECL void test_expectActualImmediate_client();
  MOCKPP_TESTMETHOD_DECL void fail_expectActualImmediate_client();
  MOCKPP_TESTMETHOD_DECL void test_expectActualImmediate_client_verify();
  MOCKPP_TESTMETHOD_DECL void test_expectActualImmediate_master();
  MOCKPP_TESTMETHOD_DECL void fail_expectActualImmediate_master();
  MOCKPP_TESTMETHOD_DECL void test_master_verify();

  MOCKPP_TESTMETHOD_DECL void test_expectActualVerify();
  MOCKPP_TESTMETHOD_DECL void fail_expectActualVerify();

  MOCKPP_TESTMETHOD_DECL void test_add_remove();
  MOCKPP_TESTMETHOD_DECL void test_counter();
  MOCKPP_TESTMETHOD_DECL void test_name();
  MOCKPP_TESTMETHOD_DECL void test_clearActual();
  MOCKPP_TESTMETHOD_DECL void test_clear();
  MOCKPP_TESTMETHOD_DECL void test_hasExpectations();
  MOCKPP_TESTMETHOD_DECL void test_ifNoExpected();

  MOCKPP_TESTMETHOD_DECL void fail_ifNoActual_master();
  MOCKPP_TESTMETHOD_DECL void fail_ifNoActual_client();
  MOCKPP_TESTMETHOD_DECL void doNothing(MOCKPP_NS::TrackingCounterBase &ec);
  MOCKPP_TESTMETHOD_DECL void doActualImmediate(MOCKPP_NS::TrackingCounterClient &ec);
  MOCKPP_TESTMETHOD_DECL void doActualVerify(MOCKPP_NS::TrackingCounterClient &ec);
};

#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( TrackingCounter_test );

MOCKPP_CXXTEST(TrackingCounter_test, test_hasExpectations);
MOCKPP_CXXTEST(TrackingCounter_test, test_name);
MOCKPP_CXXTEST(TrackingCounter_test, test_clearActual);
MOCKPP_CXXTEST(TrackingCounter_test, test_clear);
MOCKPP_CXXTEST(TrackingCounter_test, test_add_remove);
MOCKPP_CXXTEST(TrackingCounter_test, test_counter);

MOCKPP_CXXTEST(TrackingCounter_test, test_expectNothing);
MOCKPP_CXXTEST(TrackingCounter_test, test_expectNothing_revoked);
MOCKPP_CXXTEST(TrackingCounter_test, test_expectActualImmediate_client);
MOCKPP_CXXTEST(TrackingCounter_test, test_expectActualImmediate_master);
MOCKPP_CXXTEST(TrackingCounter_test, test_expectActualVerify);
MOCKPP_CXXTEST(TrackingCounter_test, test_ifNoExpected);

MOCKPP_CXXTEST(TrackingCounter_test, fail_expectActualImmediate_client);
MOCKPP_CXXTEST(TrackingCounter_test, test_expectActualImmediate_client_verify);
MOCKPP_CXXTEST(TrackingCounter_test, fail_expectActualImmediate_master);
MOCKPP_CXXTEST(TrackingCounter_test, test_master_verify);
MOCKPP_CXXTEST(TrackingCounter_test, fail_expectActualVerify);
MOCKPP_CXXTEST(TrackingCounter_test, fail_expectNothing);

MOCKPP_CXXTEST(TrackingCounter_test, fail_ifNoActual_master);
MOCKPP_CXXTEST(TrackingCounter_test, fail_ifNoActual_client);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(TrackingCounter_test::test_hasExpectations);
MOCKPP_BOOST_TEST(TrackingCounter_test::test_name);
MOCKPP_BOOST_TEST(TrackingCounter_test::test_clearActual);
MOCKPP_BOOST_TEST(TrackingCounter_test::test_clear);
MOCKPP_BOOST_TEST(TrackingCounter_test::test_add_remove);
MOCKPP_BOOST_TEST(TrackingCounter_test::test_counter);

MOCKPP_BOOST_TEST(TrackingCounter_test::test_expectNothing);
MOCKPP_BOOST_TEST(TrackingCounter_test::test_expectNothing_revoked);
MOCKPP_BOOST_TEST(TrackingCounter_test::test_expectActualImmediate_client);
MOCKPP_BOOST_TEST(TrackingCounter_test::test_expectActualImmediate_master);
MOCKPP_BOOST_TEST(TrackingCounter_test::test_expectActualVerify);
MOCKPP_BOOST_TEST(TrackingCounter_test::test_ifNoExpected);

MOCKPP_BOOST_TEST(TrackingCounter_test::fail_expectActualImmediate_client);
MOCKPP_BOOST_TEST(TrackingCounter_test::test_expectActualImmediate_client_verify);
MOCKPP_BOOST_TEST(TrackingCounter_test::fail_expectActualImmediate_master);
MOCKPP_BOOST_TEST(TrackingCounter_test::test_master_verify);
MOCKPP_BOOST_TEST(TrackingCounter_test::fail_expectActualVerify);
MOCKPP_BOOST_TEST(TrackingCounter_test::fail_expectNothing);

MOCKPP_BOOST_TEST(TrackingCounter_test::fail_ifNoActual_master);
MOCKPP_BOOST_TEST(TrackingCounter_test::fail_ifNoActual_client);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (TrackingCounter_test);

#endif // frameworks



class MyTrackingCounterBase : public MOCKPP_NS::TrackingCounterBase
{
  public:
    MyTrackingCounterBase()
      :MOCKPP_NS::TrackingCounterBase(MOCKPP_PCHAR("name"), 0)
    {}

    virtual void verify()
    {}

    virtual void reset()
    {}

    void clearActual()
    {}

   unsigned getActual() const
   {
     return 0;
   }

};


void TrackingCounter_test::test_name()
{
  MOCKPP_NS::TrackingCounterMaster master(MOCKPP_PCHAR("master"), 0);
  MOCKPP_NS::TrackingCounterClient ec (MOCKPP_PCHAR("client"), master);
  MOCKPP_ASSERT_TRUE(ec.getVerifiableName() == (MOCKPP_PCHAR("client")));
  MOCKPP_ASSERT_TRUE(master.getVerifiableName() == (MOCKPP_PCHAR("master")));
}


void TrackingCounter_test::test_hasExpectations()
{
  {
    MyTrackingCounterBase ec;
    MOCKPP_ASSERT_TRUE(false == ec.hasExpectations());
    ec.setExpected(2);
    MOCKPP_ASSERT_TRUE(true == ec.hasExpectations());
  }

  MyTrackingCounterBase ec2;
  MOCKPP_ASSERT_TRUE(false == ec2.hasExpectations());
  ec2.setExpectNothing();
  MOCKPP_ASSERT_TRUE(true == ec2.hasExpectations());
}


void TrackingCounter_test::test_clear()
{
  MOCKPP_NS::TrackingCounterMaster master(MOCKPP_PCHAR("master"), 0);
  MOCKPP_NS::TrackingCounterClient ec (MOCKPP_PCHAR("client"), master);
  ec.setExpected(3);
  MOCKPP_ASSERT_TRUE(ec.inc() == 1);
  ec.reset();
  ec.verify();  // no fail because no expectations
}


void TrackingCounter_test::test_clearActual()
{
  MOCKPP_NS::TrackingCounterMaster master(MOCKPP_PCHAR("master"), 0);
  MOCKPP_NS::TrackingCounterClient ec (MOCKPP_PCHAR("client"), master);
  ec.setExpected(3);
  master.setExpected(3);
  MOCKPP_ASSERT_TRUE(ec.inc() == 1);
  MOCKPP_ASSERT_TRUE(ec.inc() == 2);
  ec.inc();   // no fail anyway
  master.clearActual();
  MOCKPP_ASSERT_TRUE(ec.getActual() == 0);
  MOCKPP_ASSERT_TRUE(master.getActual() == 0);
  ec.inc();
  ec.inc();
  ec.inc();   // no fail even now
  ec.verify();
  ec.reset();
  MOCKPP_ASSERT_TRUE(false == ec.hasExpectations());
}


void TrackingCounter_test::test_add_remove()
{
  MOCKPP_NS::TrackingCounterMaster master(MOCKPP_PCHAR("master"), 0);
  MOCKPP_ASSERT_TRUE(master.numClients() == 0);
  {
    MOCKPP_NS::TrackingCounterClient ec1 (MOCKPP_PCHAR("client-1"), master);
    MOCKPP_NS::TrackingCounterClient ec2 (MOCKPP_PCHAR("client-2"), master);
    MOCKPP_ASSERT_TRUE(master.numClients() == 2);
  }
  MOCKPP_ASSERT_TRUE(master.numClients() == 0);
}


void TrackingCounter_test::test_counter()
{
  MOCKPP_NS::TrackingCounterMaster master(MOCKPP_PCHAR("master"), 0);
  MOCKPP_NS::TrackingCounterClient ec1 (MOCKPP_PCHAR("client-1"), master);
  MOCKPP_NS::TrackingCounterClient ec2 (MOCKPP_PCHAR("client-2"), master);
  ec2++;
  ec1++;
  ++ec2;
  ++ec2;
  ec1.inc();
  MOCKPP_ASSERT_TRUE(master.getActual() == 5);
  MOCKPP_ASSERT_TRUE(ec1.getActual() == 5);
  MOCKPP_ASSERT_TRUE(ec2.getActual() == 4);
  MOCKPP_ASSERT_TRUE(ec1.numTags() == 2);
  MOCKPP_ASSERT_TRUE(ec2.numTags() == 3);

  MOCKPP_ASSERT_TRUE(ec2.getTag(0) == 1);
  MOCKPP_ASSERT_TRUE(ec1.getTag(0) == 2);
  MOCKPP_ASSERT_TRUE(ec2.getTag(1) == 3);
  MOCKPP_ASSERT_TRUE(ec2.getTag(2) == 4);
  MOCKPP_ASSERT_TRUE(ec1.getTag(1) == 5);
}


void TrackingCounter_test::test_ifNoExpected()
{
  MOCKPP_NS::TrackingCounterMaster master(MOCKPP_PCHAR("master"), 0);
  MOCKPP_NS::TrackingCounterClient ec (MOCKPP_PCHAR("client"), master);
  ec.inc(); // no fail yet
  ec.verify();
  master.verify();
}


void TrackingCounter_test::fail_ifNoActual_client()
{
  MOCKPP_NS::TrackingCounterMaster master(MOCKPP_PCHAR("master"), 0);
  MOCKPP_NS::TrackingCounterClient ec (MOCKPP_PCHAR("client"), master);
  ec.setExpected(2);
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.verify();  // fails because no value
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void TrackingCounter_test::fail_ifNoActual_master()
{
  MOCKPP_NS::TrackingCounterMaster master(MOCKPP_PCHAR("master"), 0);
  MOCKPP_NS::TrackingCounterClient ec (MOCKPP_PCHAR("client"), master);
  master.setExpected(2);
  bool thrown = false;
  MOCKPP_TRY
  {
    master.verify();  // fails because no value
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void TrackingCounter_test::doNothing(MOCKPP_NS::TrackingCounterBase &ec)
{
  ec.setExpected(3);
  ec.setExpectNothing();
}


void TrackingCounter_test::test_expectNothing()
{
  MOCKPP_NS::TrackingCounterMaster master(MOCKPP_PCHAR("master"), 0);
  MOCKPP_NS::TrackingCounterClient ec (MOCKPP_PCHAR("client"), master);
  doNothing(ec);
  ec.verify();  // no fail
}


void TrackingCounter_test::test_expectNothing_revoked()
{
  MOCKPP_NS::TrackingCounterMaster master(MOCKPP_PCHAR("master"), 0);
  MOCKPP_NS::TrackingCounterClient ec (MOCKPP_PCHAR("client"), master);
  ec.setExpectNothing();
  ec.setExpected(3);
  ec.inc();
  ec.inc();
  ec.inc();
  ec.verify();  // no fail
}


void TrackingCounter_test::fail_expectNothing()
{
  MOCKPP_NS::TrackingCounterMaster master(MOCKPP_PCHAR("master"), 0);
  MOCKPP_NS::TrackingCounterClient ec (MOCKPP_PCHAR("client"), master);
  doNothing(ec);
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.inc();  // fails
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void TrackingCounter_test::doActualImmediate(MOCKPP_NS::TrackingCounterClient &ec)
{
  ec.setExpected(2);
  ec.inc();
  ec.inc();
}


void TrackingCounter_test::test_expectActualImmediate_client()
{
  MOCKPP_NS::TrackingCounterMaster master(MOCKPP_PCHAR("master"), 0);
  MOCKPP_NS::TrackingCounterClient ec (MOCKPP_PCHAR("client"), master);
  doActualImmediate(ec);
  ec.verify();     // no fail
  master.verify(); // no fail
}


void TrackingCounter_test::test_expectActualImmediate_master()
{
  MOCKPP_NS::TrackingCounterMaster master(MOCKPP_PCHAR("master"), 0);
  MOCKPP_NS::TrackingCounterClient ec (MOCKPP_PCHAR("client"), master);
  master.setExpected(2);
  ec.inc();
  ec.inc();
  ec.verify();     // no fail
  master.verify(); // no fail
}


void TrackingCounter_test::fail_expectActualImmediate_client()
{
  MOCKPP_NS::TrackingCounterMaster master(MOCKPP_PCHAR("master"), 0);
  MOCKPP_NS::TrackingCounterClient ec (MOCKPP_PCHAR("client"), master);
  doActualImmediate(ec);
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.inc();   // fails
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void TrackingCounter_test::test_expectActualImmediate_client_verify()
{
  MOCKPP_NS::TrackingCounterMaster master(MOCKPP_PCHAR("master"), 0);
  MOCKPP_NS::TrackingCounterClient ec (MOCKPP_PCHAR("client"), master);
  ec.setExpected(2);
  ec.setFailOnVerify();
  ec.inc();
#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING_COND(ec.verify(),
                              MOCKPP_PCHAR("test_client_verify()"),
                              MOCKPP_NS::AssertionFailedError,
                              exception_object.getMessage().find(MOCKPP_PCHAR("tagged with 1 which is less "))  != MOCKPP_STL::string::npos);
#endif
  ec.inc();
  ec.inc();
#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING_COND(ec.verify(),
                              MOCKPP_PCHAR("test_client_verify()"),
                              MOCKPP_NS::AssertionFailedError,
                              exception_object.getMessage().find(MOCKPP_PCHAR("tagged with 3 which is more"))  != MOCKPP_STL::string::npos);
#endif
}


void TrackingCounter_test::fail_expectActualImmediate_master()
{
  MOCKPP_NS::TrackingCounterMaster master(MOCKPP_PCHAR("master"), 0);
  MOCKPP_NS::TrackingCounterClient ec (MOCKPP_PCHAR("client"), master);
  master.setExpected(2);
  ec.inc();
  ec.inc();
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.inc();
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void TrackingCounter_test::test_master_verify()
{
  MOCKPP_NS::TrackingCounterMaster master(MOCKPP_PCHAR("master"), 0);
  MOCKPP_NS::TrackingCounterClient ec (MOCKPP_PCHAR("client"), master);
  master.setExpected(2);
  master.setFailOnVerify();
  ec.inc();
#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING_COND(master.verify(),
                              MOCKPP_PCHAR("test_master_verify()"),
                              MOCKPP_NS::AssertionFailedError,
                              exception_object.getMessage().find(MOCKPP_PCHAR("tagged 1 times which is less "))  != MOCKPP_STL::string::npos);
#endif
  ec.inc();
  ec.inc();
#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING_COND(master.verify(),
                              MOCKPP_PCHAR("test_master_verify()"),
                              MOCKPP_NS::AssertionFailedError,
                              exception_object.getMessage().find(MOCKPP_PCHAR("tagged 3 times which is more"))  != MOCKPP_STL::string::npos);
#endif
}


void TrackingCounter_test::doActualVerify(MOCKPP_NS::TrackingCounterClient &ec)
{
  ec.setExpected(2);
  ec.setFailOnVerify();
  ec.inc();
  ec.inc();
  ec.inc();
  ec.inc();
}


void TrackingCounter_test::test_expectActualVerify()
{
  MOCKPP_NS::TrackingCounterMaster master(MOCKPP_PCHAR("master"), 0);
  MOCKPP_NS::TrackingCounterClient ec (MOCKPP_PCHAR("client"), master);
  doActualVerify(ec); // no fail, no verify()
}


void TrackingCounter_test::fail_expectActualVerify()
{
  MOCKPP_NS::TrackingCounterMaster master(MOCKPP_PCHAR("master"), 0);
  MOCKPP_NS::TrackingCounterClient ec (MOCKPP_PCHAR("client"), master);
  doActualVerify(ec); // no fail
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.verify(); // fails
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}



