/** @file
    @brief    support throwing exceptions


  $Id: Throwable.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sam Feb 8 2003
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

#include <mockpp/Throwable.h>

#include <iosfwd>


MOCKPP_NS_START


MOCKPP_API_IMPL0 ThrowableItem::ThrowableItem(Throwable *it)
  : item(0)
{
  take(it);
}


MOCKPP_API_IMPL0 ThrowableItem::ThrowableItem(const ThrowableItem &it)
  : item(const_cast<ThrowableItem&>(it).release())
{
}


MOCKPP_API_IMPL(ThrowableItem &) ThrowableItem::operator = (const ThrowableItem &it)
{
  take(const_cast<ThrowableItem&>(it).release());
  return *this;
}


MOCKPP_API_IMPL0 ThrowableItem::~ThrowableItem() throw()
{
  take((Throwable*)0);
}


MOCKPP_API_IMPL(void) ThrowableItem::take(Throwable *it)
{
  delete item;
  item = it;
}


MOCKPP_API_IMPL(Throwable *) ThrowableItem::release()
{
  Throwable *tmp = item;
  item = 0;
  return tmp;
}


MOCKPP_API_IMPL(Throwable *) ThrowableItem::get() const
{
  return item;
}


MOCKPP_API_IMPL(void) ThrowableItem::reset()
{
  take((Throwable*)0);
}


MOCKPP_NS_END

