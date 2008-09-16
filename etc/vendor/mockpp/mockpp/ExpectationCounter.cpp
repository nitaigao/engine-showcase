/** @file
    @brief    check range of of expected calls

  $Id: ExpectationCounter.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Mon Dec 30 2002
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

#define MOCKPP_NEED_EXPORTS
#include <mockpp/mockpp.h> // always first

#include <mockpp/ExpectationCounter.h>

#include <mockpp/compat/Formatter.h>

MOCKPP_NS_START


MOCKPP_API_IMPL0
ExpectationCounter::ExpectationCounter(const String &name, VerifiableList *parent)
  : AbstractExpectation<unsigned>(name, parent)
{
  expectNothing = false;
  myExpectedMinCalls = 0;
  myExpectedMaxCalls = 0;
  myActualCalls = 0;
}


MOCKPP_API_IMPL(void) ExpectationCounter::clearActual()
{
  myActualCalls = 0;
}


MOCKPP_API_IMPL(void) ExpectationCounter::clearExpectation()
{
  clearHasExpectations();
}


MOCKPP_API_IMPL(void) ExpectationCounter::reset()
{
  clearActual();
  clearExpectation();
  this->clearHasExpectations();
}


MOCKPP_API_IMPL(unsigned) ExpectationCounter::getActual() const
{
  return myActualCalls;
}


MOCKPP_API_IMPL(void) ExpectationCounter::inc()
{
  myActualCalls++;
  if (shouldCheckImmediately())
  {
     String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 was called %2 times but should not be called more than %3 times."));

     fmt << getVerifiableName() << myActualCalls << myExpectedMaxCalls;
     MOCKPP_ASSERT_TRUE_MESSAGE(fmt, myActualCalls <= myExpectedMaxCalls);
  }
}


MOCKPP_API_IMPL(void)
ExpectationCounter::setExpected(unsigned expectedMinCalls, unsigned expectedMaxCalls)
{
  expectNothing = false;
  MOCKPP_ASSERT_TRUE(expectedMinCalls <= expectedMaxCalls);
  myExpectedMinCalls = expectedMinCalls;
  myExpectedMaxCalls = expectedMaxCalls;
  setHasExpectations();
}


MOCKPP_API_IMPL(void) ExpectationCounter::setExpected(unsigned expectedCalls)

{
  setExpected(expectedCalls, expectedCalls);
}


MOCKPP_API_IMPL(void) ExpectationCounter::setExpectNothing()
{
  myExpectedMinCalls = 0;
  myExpectedMaxCalls = 0;
  setHasExpectations();
  expectNothing = true;
}


MOCKPP_API_IMPL(void) ExpectationCounter::verify()
{
  if (!this->hasExpectations() )
    return;

  if( expectNothing )
  {
    String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 expected no value."));
    fmt << this->getVerifiableName();

    MOCKPP_ASSERT_TRUE_MESSAGE(fmt, myActualCalls == 0 );
  }
  else
  {
    String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 was called %2 times which is less than the expected count of %3."));
    fmt << getVerifiableName() << myActualCalls << myExpectedMinCalls;
    MOCKPP_ASSERT_TRUE_MESSAGE(fmt, myExpectedMinCalls <= myActualCalls);

    fmt = mockpp_i18n(MOCKPP_PCHAR("%1 was called %2 times which is more than the expected count of %3."));
    fmt << getVerifiableName() << myActualCalls << myExpectedMaxCalls;
    MOCKPP_ASSERT_TRUE_MESSAGE(fmt, myExpectedMaxCalls >= myActualCalls);
  }
}


MOCKPP_NS_END
