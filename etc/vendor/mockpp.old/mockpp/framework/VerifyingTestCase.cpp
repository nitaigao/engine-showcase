/** @file
    @brief  Unit testcase with verification

  $Id: VerifyingTestCase.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#define MOCKPP_NEED_EXPORTS
#include <mockpp/mockpp.h> // always first

#include <mockpp/framework/VerifyingTestCase.h>

#include <mockpp/compat/Exception.h>
#include <mockpp/compat/Formatter.h>
#include <mockpp/compat/AssertionFailedError.h>

#ifdef MOCKPP_USE_CPPUNIT

#include <cppunit/Exception.h>
#include <cppunit/SourceLine.h>
#include <cppunit/TestResult.h>


MOCKPP_NS_START


MOCKPP_API_IMPL0 VerifyingTestCase::VerifyingTestCase()
  : CppUnit::TestCase()
  , MockObject(MOCKPP_PCHAR("VerifyingTestCase/*"), 0)
{
}


MOCKPP_API_IMPL0
VerifyingTestCase::VerifyingTestCase( const MOCKPP_STD_NS::string &name)
  : CppUnit::TestCase(name)
#ifdef MOCKPP_ALTERNATIVE_STL
  , MockObject(MOCKPP_PCHAR("VerifyingTestCase/")+MOCKPP_GET_STRING(name.c_str()), 0)
#else
  , MockObject(MOCKPP_PCHAR("VerifyingTestCase/")+MOCKPP_GET_STRING(name), 0)
#endif
{
}


MOCKPP_API_IMPL0 VerifyingTestCase::~VerifyingTestCase()
{
  unregisterVerifiables();
}


void MOCKPP_API_IMPL0 VerifyingTestCase::verify()
{
  if (numVerifiables() == 0)
  {
    String s = MOCKPP_PCHAR("VerifyingTestCase \"%1\" has no sub-objects to verify.\n")
               MOCKPP_PCHAR("Did you forget to register?");
    s << getVerifiableName();
    assertionFailed (__LINE__, __FILE__, s);
  }

  MockObject::verify();
}


void MOCKPP_API_IMPL0 VerifyingTestCase::registerVerifiable(AutoPointer<Verifiable> in_verifiable)
{
  Verifiable *verifiable = in_verifiable.release();
  registeredVerifiables.push_back(verifiable);

  if (!hasVerifiable(verifiable))
    addVerifiable(verifiable);

  if (verifiable->getParent() == 0)
    verifiable->setParent(this);
}


void MOCKPP_API_IMPL0 VerifyingTestCase::unregisterVerifiables()
{
  clearVerifiables();
  for (unsigned i = 0; i < registeredVerifiables.size(); ++i)
    delete registeredVerifiables[i];
  registeredVerifiables.clear();
}


MOCKPP_NS_END


#endif // MOCKPP_USE_CPPUNIT

