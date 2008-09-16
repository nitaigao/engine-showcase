/** @file
    @brief  Base class for invocations

$Id: Invocation.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

***************************************************************************/

/**************************************************************************

   begin                : Sat Oct 23 2004
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

#define MOCKPP_USE_INVOCATION_EQUALS

#define MOCKPP_NEED_EXPORTS
#include <mockpp/mockpp.h> // always first

#include <mockpp/chaining/Invocation.h>


MOCKPP_NS_START


MOCKPP_API_IMPL0 InvocationBase::~InvocationBase()
{
}


/////////////////////////////////////////////////////////////


MOCKPP_API_IMPL0 Invocation::Invocation( const String &in_name)
  : name(in_name)
{
}


MOCKPP_API_IMPL(String) Invocation::describeTo( String &buffer ) const
{
  buffer += getMethodName();
  buffer += MOCKPP_PCHAR( "(" ) + describeParameters() + MOCKPP_PCHAR( ")" ) ;
  return buffer;
}


MOCKPP_API_IMPL(String) Invocation::getMethodName() const
{
  return name;
}


/////////////////////////////////////////////////////////////


MOCKPP_API_IMPL0 Invocation0::Invocation0(  const String &in_name)
  : Invocation( in_name)
{
}


#ifdef MOCKPP_USE_INVOCATION_EQUALS

MOCKPP_API_IMPL(bool) Invocation0::equals( const Invocation0 &/*other*/ ) const
{
  return true;
}


MOCKPP_API_IMPL(bool) Invocation0::equals( const InvocationBase &/*other*/ ) const
{
  return false;
}

#endif // MOCKPP_USE_INVOCATION_EQUALS

MOCKPP_API_IMPL(String) Invocation0::describeParameters() const
{
  return String();
}


MOCKPP_NS_END

