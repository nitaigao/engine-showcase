/** @file
    @brief    base class for MockObject exceptions

  $Id: Exception.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Tue Dec 31 2002
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

#include <mockpp/compat/Exception.h>


MOCKPP_NS_START


MOCKPP_API_IMPL0  Exception::Exception(unsigned line, const char *file, const String &msg)
  :
#ifdef MOCKPP_USE_STD_EXCEPTION
    MOCKPP_STD_NS::exception(),
#endif
    message(msg),
    srcline(line),
#ifdef MOCKPP_UNICODE
    srcfile(getUnicode(file))
#else
    srcfile(getLatin1(file))
#endif
{
}


MOCKPP_API_IMPL0  Exception::~Exception () throw()
{
}


#ifdef MOCKPP_USE_STD_EXCEPTION

MOCKPP_API_IMPL(const char *) Exception::what() const throw()
{
  what_helper = getLatin1(getMessage());
  return what_helper.c_str();
}

#endif


MOCKPP_API_IMPL(String)  Exception::getMessage() const
{
  return message;
}


MOCKPP_API_IMPL(unsigned)  Exception::getSrcLine() const
{
  return srcline;
}


MOCKPP_API_IMPL(String) Exception::getSrcFile() const
{
  return srcfile;
}


MOCKPP_NS_END


