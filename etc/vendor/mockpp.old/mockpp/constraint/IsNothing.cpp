/** @file
    @brief  Constraint which is never true

 $Id: IsNothing.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

***************************************************************************/

/**************************************************************************

   begin                : Sat Aug 21 2004
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

#include <mockpp/constraint/IsNothing.h>


MOCKPP_NS_START


MOCKPP_API_IMPL0 IsNothing::IsNothing()
    : description( MOCKPP_PCHAR( "NOTHING" ) )
{
}


MOCKPP_API_IMPL0 IsNothing::IsNothing( const String &in_description )
  : description( in_description )
{
}


MOCKPP_API_IMPL0 IsNothing::~IsNothing()
{
}


MOCKPP_API_IMPL(bool) IsNothing::typelessEval( ) const
{
  return false;
}


MOCKPP_API_IMPL(String) IsNothing::describeTo( String &buffer ) const
{
  buffer += description;
  return buffer;
}


MOCKPP_NS_END
