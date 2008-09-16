/** @file
    @brief    support throwing lists of exceptions

  $Id: ThrowableList.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : We Jan 22 2003
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

#include <mockpp/compat/Asserter.h>
#include <mockpp/compat/Formatter.h>

#include <mockpp/ThrowableList.h>

MOCKPP_NS_START


MOCKPP_API_IMPL0 ThrowableList::ThrowableList(const String &name, VerifiableList *parent)
 : Verifiable (name, parent)
{
}


MOCKPP_API_IMPL0 ThrowableList::~ThrowableList() throw()
{
  reset();
}


MOCKPP_API_IMPL(void) ThrowableList::push_back(Throwable* t)
{
  addObjectToThrow(t);
}


MOCKPP_API_IMPL(void) ThrowableList::addObjectToThrow(Throwable* t)
{
//  MOCKPP_ASSERT_TRUE(t != 0);  Tricky: ResonseVector uses 0 to distinguish
//                               between values and throwables
  list.push_back(t);
}


MOCKPP_API_IMPL(unsigned) ThrowableList::size() const
{
  return already_used.size() + list.size();
}


MOCKPP_API_IMPL(Throwable *) ThrowableList::at(unsigned i)
{
  MOCKPP_ASSERT_TRUE(i < already_used.size() + list.size());
  if (i < already_used.size() )
    return already_used[i];

  return list[i-already_used.size()];
/*
  if (i < already_used.size() )
    return already_used.at(i);

  return list.at(i-already_used.size());
*/
}


MOCKPP_API_IMPL(Throwable *) ThrowableList::nextThrowableObject()
{
  String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 has run out of objects."));
  fmt << getVerifiableName();

  MOCKPP_ASSERT_TRUE_MESSAGE(fmt, list.size() > 0);
  Throwable *temp = list[0];
  list.erase(list.begin());
  already_used.push_back(temp);

  return temp;
}


MOCKPP_API_IMPL(bool) ThrowableList::hasMoreObjects() const
{
   return list.size() != 0;
}


MOCKPP_API_IMPL(void) ThrowableList::reset()
{
  for (unsigned i1 = 0; i1 < list.size(); ++i1)
    delete list[i1];
  list.clear();

  for (unsigned i2 = 0; i2 < already_used.size(); ++i2)
    delete already_used[i2];
  already_used.clear();
}


MOCKPP_API_IMPL(void) ThrowableList::verify()
{
  String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 has %2 un-used objects."));
  fmt << getVerifiableName() << list.size();

  MOCKPP_ASSERT_EQUALS_MESSAGE(fmt, (size_t)0, list.size());
}

MOCKPP_NS_END

