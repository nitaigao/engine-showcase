/** @file
    @brief    a collection of verifiables


  $Id: VerifiableList.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#include MOCKPP_ALGORITHM_H

#include <mockpp/VerifiableList.h>
#include <mockpp/compat/Asserter.h>


MOCKPP_NS_START


MOCKPP_API_IMPL0 VerifiableList::VerifiableList (const String &name, VerifiableList *parent)
  : Verifiable(name, parent)
{
}


MOCKPP_API_IMPL(void) VerifiableList::addVerifiable (Verifiable* vf)
{
  if (vf != 0)
    verifiables.push_back(vf);
}


MOCKPP_API_IMPL(void) VerifiableList::removeVerifiable (Verifiable* vf)
{
  MOCKPP_STL::vector<Verifiable*>::iterator it =
    MOCKPP_STL::find (verifiables.begin(), verifiables.end(), vf);

  if(it != verifiables.end())
     verifiables.erase(it);
}


MOCKPP_API_IMPL(bool) VerifiableList::hasVerifiable (Verifiable* vf) const
{
  MOCKPP_STL::vector<Verifiable*>::const_iterator it =
    MOCKPP_STL::find(verifiables.begin(), verifiables.end(), vf);
  return it != verifiables.end();
}


MOCKPP_API_IMPL(unsigned) VerifiableList::numVerifiables() const
{
  return verifiables.size();
}


MOCKPP_API_IMPL(void) VerifiableList::clearVerifiables()
{
  verifiables.clear();
}


MOCKPP_API_IMPL(Verifiable *) VerifiableList::getVerifiable(unsigned idx)
{
  MOCKPP_ASSERT_TRUE(idx < verifiables.size());

  return verifiables[idx];
}


MOCKPP_NS_END

