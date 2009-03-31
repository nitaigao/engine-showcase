/** @file
    @brief  Unit testcase with verification

  $Id: VerifyingTestCase.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sun Aug 22 2004
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

   *
   *  Original Java Sources:  Copyright (c) 2000-2004, jMock.org
   *

 **/

#ifndef MOCKPP_VERIFYINGTESTCASE_H
#define MOCKPP_VERIFYINGTESTCASE_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/MockObject.h>

#include <mockpp/util/AutoPointer.h>

#ifdef MOCKPP_USE_CPPUNIT

#include <cppunit/TestCaller.h>


MOCKPP_NS_START


/** TestCase that verifies Verifiable
  * fields and registered Verifiable objects after the test has run and before the fixture
  * has been torn down.
  * @ingroup grp_framework
  * @ingroup grp_basic_mo
  * @ingroup grp_advanced_mo
  */
class VerifyingTestCase : public CppUnit::TestCase
                        , public MockObject
{
  public:

  /** Constructs the test case.
    */
    VerifyingTestCase();

  /** Constructs the test case.
    * @param name  test case name
    */
    VerifyingTestCase( const MOCKPP_STD_NS::string &name );

  /** Constructs the test case.
    */
    virtual ~VerifyingTestCase();

  /** Verifies the object and the mock objects it contains.
    * @attention Having no sub-objetcs to verify also counts as an error
    * because it is assumed that registration of the objects had been
    * forgotten. Otherwise there would be no reason to use this class.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void verify();

  /** Removes all verifiable fields from the list.
    */
    virtual void unregisterVerifiables();

  protected:

  /** Registers another verifiable object to the list.
    * @param verifiable  pointer to the verifiable object
    */
    void registerVerifiable(AutoPointer<Verifiable> verifiable);

  private:

    VerifyingTestCase (const VerifyingTestCase &);    // forbid
    VerifyingTestCase& operator=(VerifyingTestCase&);

    MOCKPP_STL::vector<Verifiable*> registeredVerifiables;
};


MOCKPP_NS_END


#endif // MOCKPP_VerifyingTestCaller_H

#endif // MOCKPP_VERIFYINGTESTCASE_H

#include <mockpp/framework/VerifyingTestCaller.h>

