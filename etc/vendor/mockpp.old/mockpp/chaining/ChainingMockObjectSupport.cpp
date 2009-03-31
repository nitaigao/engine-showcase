/** @file
    @brief  Simplify handling of some constraints

 $Id: ChainingMockObjectSupport.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

***************************************************************************/

/**************************************************************************

   begin                : Fri Sep 24 2004
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

#include <mockpp/chaining/ChainingMockObjectSupport.h>

#include <mockpp/constraint/IsAnything.h>
#include <mockpp/constraint/IsNothing.h>

#include <mockpp/matcher/InvokeOnceMatcher.h>
#include <mockpp/matcher/InvokeAtLeastOnceMatcher.h>
#include <mockpp/matcher/InvokeAtLeastMatcher.h>
#include <mockpp/matcher/InvokeAtMostMatcher.h>
#include <mockpp/matcher/InvokeCountMatcher.h>
#include <mockpp/matcher/UnlimitedMatcher.h>
#include <mockpp/matcher/TestFailureMatcher.h>


MOCKPP_NS_START


MOCKPP_API_IMPL(AutoPointer<TypelessMatcher>) once()
{
  return new InvokeOnceMatcher();
}


MOCKPP_API_IMPL(AutoPointer<TypelessMatcher>) atLeastOnce()
{
  return new InvokeAtLeastOnceMatcher();
}


MOCKPP_API_IMPL(AutoPointer<TypelessMatcher>) atMost( int expectedCount )
{
  return new InvokeAtMostMatcher( expectedCount );
}


MOCKPP_API_IMPL(AutoPointer<TypelessMatcher>) atLeast(int expectedCount)
{
    return new InvokeAtLeastMatcher(expectedCount);
}


MOCKPP_API_IMPL(AutoPointer<TypelessMatcher>) unlimited()
{
    return new UnlimitedMatcher();
}


MOCKPP_API_IMPL(AutoPointer<TypelessMatcher>) exactly( int expectedCount )
{
  return new InvokeCountMatcher( expectedCount );
}


MOCKPP_API_IMPL(AutoPointer<TypelessMatcher>) never()
{
  return new TestFailureMatcher( MOCKPP_PCHAR("never expected") );
}


MOCKPP_API_IMPL(AutoPointer<TypelessMatcher>) never( const String &errorMessage )
{
  return new TestFailureMatcher( MOCKPP_PCHAR( "never expected (" )
                                 + errorMessage
                                 + MOCKPP_PCHAR( ")" ) );
}


MOCKPP_API_IMPL0 TypelessConstraint::AP any()
{
  return new IsAnything;
}


MOCKPP_API_IMPL0 TypelessConstraint::AP nothing()
{
  return new IsNothing;
}


MOCKPP_API_IMPL0 TypelessStub<void>::AP isVoid()
{
  return new VoidStub;
}

MOCKPP_NS_END
