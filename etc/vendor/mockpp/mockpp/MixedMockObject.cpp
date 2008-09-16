/** @file
    @brief MockObject that merges all the advanced mock objects


  $Id: MixedMockObject.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sat Mar 05 2005
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

#include <mockpp/MixedMockObject.h>


MOCKPP_NS_START


MOCKPP_API_IMPL0 MixedMockObject::MixedMockObject(const String &name, VerifiableList *parent)
 : MockObject(name, parent)
 , VisitableMockObjectBase(name+MOCKPP_PCHAR("/Visitable"), parent)
 , ChainableMockObjectBase(name+MOCKPP_PCHAR("/Chainable"))
{
}


MOCKPP_API_IMPL0 MixedMockObject::~MixedMockObject()
{
}


MOCKPP_API_IMPL(void) MixedMockObject::verify()
{
  MockObject::verify();
  visitableVerify();
  chainableVerify();
}


MOCKPP_API_IMPL(void) MixedMockObject::reset()
{
  MockObject::reset();
  visitableReset();
  chainableReset();
}


MOCKPP_NS_END
