/** @file
    @brief    base class for mock objects


  $Id: MockObject.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Fri Dec 18 2002
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

#include <mockpp/MockObject.h>

#include <mockpp/util/AssertMo.h>


MOCKPP_NS_START


MOCKPP_API_IMPL0 MockObject::MockObject(const String &name, VerifiableList *parent)
 : VerifiableList(name, parent)
{
}


MOCKPP_API_IMPL(void) MockObject::notImplemented()
{
#ifndef MOCKPP_NO_RTTI
# ifdef MOCKPP_UNICODE
  MOCKPP_NOT_IMPLEMENTED(getUnicode(typeid(*this).name()));
# else
  MOCKPP_NOT_IMPLEMENTED(typeid(*this).name());
# endif
#else
  MOCKPP_NOT_IMPLEMENTED(MOCKPP_PCHAR("[rtti-disabled in MockObject::notImplemented()") );
#endif
}


MOCKPP_API_IMPL(void) MockObject::reset() // @todo test
{
  for (unsigned i = 0; i < numVerifiables(); ++i)
    getVerifiable(i)->reset();
}


MOCKPP_API_IMPL(void) MockObject::verify()
{
  verifyAnObject(this);
}


MOCKPP_API_IMPL(void) MockObject::verifyAnObject(VerifiableList* anObject)
{
  VerifierVec myProcessingObjects;
  myProcessingObjects.push_back(anObject); // avoids endless recursion
  verifySubFields(anObject, myProcessingObjects);
}


MOCKPP_API_IMPL(void) MockObject::verifySubFields(Verifiable  *anObject,
                                               VerifierVec &alreadyProcessed)
{
  if (anObject == 0)
    return;

  if (anObject != 0)
    verifyField(anObject, alreadyProcessed);

  for (unsigned i = 0; i < anObject->numVerifiables(); ++i)
    verifySubFields(anObject->getVerifiable(i), alreadyProcessed);

}


MOCKPP_API_IMPL(void) MockObject::verifyField(Verifiable  *anObject,
                                           VerifierVec &alreadyProcessed)
{
  if (   isVerifiable(anObject)
      && MOCKPP_STL::find(alreadyProcessed.begin(), alreadyProcessed.end(), anObject) == alreadyProcessed.end())
  {
    alreadyProcessed.push_back(anObject);
    anObject->verify();
  }
}


MOCKPP_API_IMPL(bool) MockObject::isVerifiable(const Verifiable *anObject)
{
  return dynamic_cast<const Verifiable*>(anObject) != 0;
}


MOCKPP_NS_END
