/** @file
    @brief    signal failed assertions

  $Id: AssertionFailedError.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Mon Dec 23 2002
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

#include <mockpp/compat/AssertionFailedError.h>


MOCKPP_NS_START


MOCKPP_API_IMPL0
AssertionFailedError::AssertionFailedError (unsigned in_srcline, const char* in_srcfile,
                                            const String &in_msg)
  : Exception(in_srcline, in_srcfile, in_msg)
{
}


MOCKPP_API_IMPL0
AssertionFailedError::~AssertionFailedError() throw()
{
}


MOCKPP_API_IMPL(void)
assertionFailed(unsigned in_srcline, const String& in_srcfile,
                     const String &in_message)
{
  assertionFailed(in_srcline, getLatin1(in_srcfile).c_str(), in_message);
}


/** default failure forwarder. Throws an exception.
  */
void defaultAssertionFailedForwarder(const AssertionFailedError &err)
{
  MOCKPP_THROW(err);
}


/** default failure forwarder pointer.
  * @internal
  */
AssertionFailedForwarder_t forwardAssertionFailed = defaultAssertionFailedForwarder;


MOCKPP_API_IMPL(void)
assertionFailed(unsigned srcline, const char* srcfile,
                     const String &message)
{
  if (forwardAssertionFailed != 0)
    forwardAssertionFailed(AssertionFailedError(srcline, srcfile, message));
  else
    defaultAssertionFailedForwarder(AssertionFailedError(srcline, srcfile, message));
}


MOCKPP_NS_END



