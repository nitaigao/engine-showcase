/** @file
    @brief    MockObjects that can be visited under control


  $Id: VisitableMockMethod.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Fri Oct 14 2005
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

#include <mockpp/mockpp_dbc.h>
#include <mockpp/visiting/VisitableMockMethod.h>


MOCKPP_NS_START


MOCKPP_API_IMPL0 VisitableMockMethodBase::VisitableMockMethodBase(const String &name,
                                                                  VisitableMockObject *parent)
  : MockObject(name, parent)
  , visitable(parent)
  , throwables (name + MOCKPP_PCHAR("/throwables"), this)
  , throwablesInline (true)
{
  MOCKPP_PRE(parent != 0);
  MOCKPP_PRE(name.length() != 0);
  reset();
}


MOCKPP_API_IMPL(String) VisitableMockMethodBase::getMethodName(bool fully) const
{
  if (fully)
    return visitable->getVerifiableName()
           + MOCKPP_PCHAR(".")
           + getVerifiableName()
           + MOCKPP_PCHAR(".");

  else
    return getVerifiableName();
}


MOCKPP_API_IMPL(MOCKPP_STL::string) VisitableMockMethodBase::getMethodIdentifier() const
{
  return getLatin1(getMethodName());
}


MOCKPP_API_IMPL(VisitableMockObject *) VisitableMockMethodBase::getVisitableMockObject() const
{
  return visitable;
}


MOCKPP_NS_END

