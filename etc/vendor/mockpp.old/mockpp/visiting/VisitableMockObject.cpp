/** @file
    @brief    MockObjects that can be visited under control


  $Id: VisitableMockObject.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#include <mockpp/visiting/VisitableMockObject.h>


MOCKPP_NS_START


MOCKPP_API_IMPL0 VisitableMockObjectBase::VisitableMockObjectBase(const String &name, VerifiableList *parent)
 :methodList(name+MOCKPP_PCSTRING("/methodList"), parent)
{
  activated = false;
}


MOCKPP_API_IMPL0 VisitableMockObjectBase::~VisitableMockObjectBase()
{
}


MOCKPP_API_IMPL(void) VisitableMockObjectBase::addExpectedMethod(const MOCKPP_STL::string &name) const
{
  MOCKPP_ASSERT_EQUALS(isActivated(), false);
  methodList.addExpected(name);
}


MOCKPP_API_IMPL(void) VisitableMockObjectBase::addActualMethod(const MOCKPP_STL::string &name) const
{
  MOCKPP_ASSERT_EQUALS(isActivated(), true);
  methodList.addActual(name);
}


MOCKPP_API_IMPL(void) VisitableMockObjectBase::activate()
{
  activated = true;
}


MOCKPP_API_IMPL(bool) VisitableMockObjectBase::isActivated() const
{
  return activated;
}


MOCKPP_API_IMPL(void) VisitableMockObjectBase::visitableVerify()
{
  methodList.verify();
  for (unsigned i = 0; i < controllers.size(); ++i)
    controllers[i]->verify();
}


MOCKPP_API_IMPL(void) VisitableMockObjectBase::visitableReset()
{
  activated = false;
  for (unsigned i = 0; i < controllers.size(); ++i)
    controllers[i]->reset();
}


MOCKPP_API_IMPL(void) VisitableMockObjectBase::unsetThrowablesInline()
{
  for (unsigned i = 0; i < controllers.size(); ++i)
    controllers[i]->unsetThrowablesInline();
}


MOCKPP_API_IMPL(void) VisitableMockObjectBase::addController (Controller *ctr)
{
  controllers.push_back(ctr);
}


MOCKPP_API_IMPL(void) VisitableMockObjectBase::removeController (Controller *ctr)
{
  MOCKPP_STL::vector<Controller*>::iterator it =
    MOCKPP_STL::find (controllers.begin(), controllers.end(), ctr);
  if (it != controllers.end())
    controllers.erase(it);
}


MOCKPP_API_IMPL(unsigned) VisitableMockObjectBase::numController() const
{
  return controllers.size();
}


/////////////////////////////////////////////////////////////////


MOCKPP_API_IMPL0 VisitableMockObject::VisitableMockObject(const String &name, VerifiableList *parent)
 : MockObject(name, parent)
 , VisitableMockObjectBase(name, this)
{
}


MOCKPP_API_IMPL0 VisitableMockObject::~VisitableMockObject()
{
}


MOCKPP_API_IMPL(void) VisitableMockObject::verify()
{
  MockObject::verify();
  visitableVerify();
}


MOCKPP_API_IMPL(void) VisitableMockObject::reset()
{
  MockObject::reset();
  visitableReset();
}


/////////////////////////////////////////////////////////////////


MOCKPP_API_IMPL0 VisitableMockObjectBase::Controller::~Controller()
{
}


MOCKPP_NS_END
