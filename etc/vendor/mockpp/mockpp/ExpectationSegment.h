/** @file
    @brief    Check expectations for substrings

  $Id: ExpectationSegment.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Dec 18 2002
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

#ifndef MOCKPP_EXPECTATIONSEGMENT_H
#define MOCKPP_EXPECTATIONSEGMENT_H


#include <mockpp/mockpp.h> // always first

#include <mockpp/AbstractExpectation.h>
#include <mockpp/util/AssertMo.h>


MOCKPP_NS_START


/** A class to verify the occurence of a substring.
  * \ingroup grp_basic_mo
  */
template <class Str>
class ExpectationSegment : public AbstractExpectation<Str>
{
  public:

  /** Constructs the expectation.
    * @param name    human readable description about the expectation
    * @param parent  parent verifiable
    */
    ExpectationSegment(const String &name, VerifiableList *parent = 0)
      : AbstractExpectation<Str>(name, parent),
        expectNothing(false),
        haveActualValue(false)
    {
    }


  /** Resets the internal state completely.
    */
    virtual void reset()
    {
      this->clearFailOnVerify();
      clearActual();
      clearExpectation();
    }


  /** Resets the internal state to reflect that there is no actual value set.
    */
    void clearActual()
    {
      haveActualValue = false;
      myActualString = Str();
    }


  /** Sets the actual target string which must contain the string segment.
    * @param aString   the actual target string
    */
    void setActual(const Str &aString)
    {
      haveActualValue = true;
      myActualString = aString;
      if (this->shouldCheckImmediately())
      {
        verify();
      }
    }


  /** Sets the expected sub string.
    * @param segment substring which must occur
    */
    void setExpected(const Str &segment)
    {
      myExpectedSegment = segment;
      expectNothing = false;
      this->setHasExpectations();
    }


  /** Tell the object to expect nothing to happen to it, perhaps because the test is exercising
    * the handling of an error. The Expectation will fail if any actual values are set.
    *
    * Note that this is not the same as not setting any expectations, in which case verify()
    * will do nothing.
    */
    void setExpectNothing()
    {
      expectNothing = true;
      this->setHasExpectations();
    }


  /** Verify that the substring occurs in the target string.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void verify()
    {
      if( expectNothing )
      {
        String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 expected no value."));
        fmt << this->getVerifiableName();
        MOCKPP_ASSERT_FALSE_MESSAGE(fmt, haveActualValue );

      }
      else
      {
        if (!this->hasExpectations() )
          return;

        String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 expected a value."));
        fmt << this->getVerifiableName();
        MOCKPP_ASSERT_TRUE_MESSAGE(fmt, haveActualValue );

        MOCKPP_ASSERT_INCLUDES_MESSAGE(
            mockpp_i18n(MOCKPP_PCHAR("Should include string segment.")),
            myExpectedSegment,
            myActualString);
      }
    }

  protected:

  /** Clears the expectation list.
    */
    virtual void clearExpectation()
    {
      this->clearHasExpectations();
    }


  private:

    Str   myExpectedSegment;
    Str   myActualString;
    bool  expectNothing;
    bool  haveActualValue;
};


MOCKPP_NS_END


#endif // MOCKPP_EXPECTATIONSEGMENT_H
