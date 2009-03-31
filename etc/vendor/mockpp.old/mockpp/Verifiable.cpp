/** @file
    @brief    abstract interface for verifiables


  $Id: Verifiable.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Dec 17 2002
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

#include <mockpp/Verifiable.h>
#include <mockpp/VerifiableList.h>


MOCKPP_NS_START


MOCKPP_API_IMPL0 Verifiable::Verifiable (const String &name, VerifiableList *parent)
{
  myParent = parent;
  myName = name;
  if (myParent != 0)
    myParent->addVerifiable(this);
}


MOCKPP_API_IMPL(String) Verifiable::getVerifiableName() const
{
  return myName;
}


MOCKPP_API_IMPL0 Verifiable::~Verifiable ()
{
  if (myParent != 0)
    myParent->removeVerifiable(this);
  myParent = 0;
}


MOCKPP_API_IMPL(void) Verifiable::setParent(VerifiableList *parent)
{
  myParent = parent;
}


MOCKPP_API_IMPL(VerifiableList *) Verifiable::getParent() const
{
  return myParent;
}


MOCKPP_API_IMPL(unsigned) Verifiable::numVerifiables() const
{
  return 0;
}


MOCKPP_API_IMPL(Verifiable *) Verifiable::getVerifiable(unsigned /*idx*/)
{
  return 0;
}


MOCKPP_NS_END




