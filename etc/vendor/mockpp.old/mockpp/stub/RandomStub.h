/** @file
    @brief  Stub returning a random value

  $Id: RandomStub.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Sep 21 2006
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

#ifndef MOCKPP_RANDOMSTUB_H
#define MOCKPP_RANDOMSTUB_H

#include <mockpp/mockpp.h> // always first

#include <cstdlib>

#include <mockpp/compat/Formatter.h>
#include <mockpp/compat/Asserter.h>
#include <mockpp/stub/TypelessStub.h>


MOCKPP_NS_START

/** Return a random value.
  * @ingroup grp_stub
  * @see mockpp::randomValue
  */
template <typename R, typename T = int>
class RandomStub : public mockpp::TypelessStub<R>
{
  public:

  /** Creates the object.
    * @param  low   lower bound of random values
    * @param  high  upper bound of random values
    */
    RandomStub(T low, T high)
      : lower(low)
      , higher(high)
    {
    }

  /** Creates the object.
    * @param  high  upper bound of random values
    */
    RandomStub(T high)
      : lower(0)
      , higher(high)
    {
    }

  /** Creates a random number within a given range
    * @param  low   lower bound of random values
    * @param  high  upper bound of random values
    */
    static R getNumber(T low, T high)
    {
      MOCKPP_ASSERT_TRUE(low < high);
      return low + (R) (float(high - low) * (MOCKPP_STD_NS::rand() / (RAND_MAX + 1.0)));
    }

  /** Mock the invocation without passing parameters.
    * @return random value from mocked invocation
    */
    virtual R typelessInvoke()
    {
      return getNumber(lower, higher);
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual mockpp::String describeTo( mockpp::String &buffer ) const
    {
        mockpp::String fmt = MOCKPP_PCHAR("returns random {%1 .. %2}");
        fmt << lower << higher;
        buffer += fmt;
        return buffer;
    }

  private:

    T lower;
    T higher;
};



MOCKPP_NS_END


#endif // MOCKPP_RANDOMSTUB_H


