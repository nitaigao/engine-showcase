/** @file
    @brief    handle assertions of mock objects

  $Id: AssertMo.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sat Dec 21 2002
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

#include <mockpp/util/NotImplementedException.h>
#include <mockpp/util/AssertMo.h>

#include <mockpp/Verifiable.h>

#include <mockpp/compat/Formatter.h>
#include <mockpp/compat/AssertionFailedError.h>


MOCKPP_NS_START


void MOCKPP_API_IMPL0 assertExcludes(unsigned srcline, const char* srcfile,
                                  const String &description,
                                  const MOCKPP_STL::string &excludeString,
                                  const MOCKPP_STL::string &targetString)
{
  assertTrue(srcline, srcfile, description, targetString.find(excludeString) == String::npos);
}

#ifdef MOCKPP_UNICODE

void MOCKPP_API_IMPL0 assertExcludes(unsigned srcline, const char* srcfile,
                                  const String &description,
                                  const MOCKPP_STL::basic_string<wchar_t> &excludeString,
                                  const MOCKPP_STL::basic_string<wchar_t> &targetString)
{
  assertTrue(srcline, srcfile, description, targetString.find(excludeString) == String::npos);
}

#endif

void MOCKPP_API_IMPL0 assertIncludes(unsigned srcline, const char* srcfile,
                                  const String &description,
                                  const MOCKPP_STL::string &includeString,
                                  const MOCKPP_STL::string &targetString)
{
  assertTrue(srcline, srcfile, description, targetString.find(includeString) != String::npos);
}

#ifdef MOCKPP_UNICODE

void MOCKPP_API_IMPL0 assertIncludes(unsigned srcline, const char* srcfile,
                                  const String &description,
                                  const MOCKPP_STL::basic_string<wchar_t> &includeString,
                                  const MOCKPP_STL::basic_string<wchar_t> &targetString)
{
  assertTrue(srcline, srcfile, description, targetString.find(includeString) != String::npos);
}

#endif

void MOCKPP_API_IMPL0 assertStartsWith(unsigned srcline, const char* srcfile,
                                    const String &description,
                                    const MOCKPP_STL::string &startString,
                                    const MOCKPP_STL::string &targetString)
{
  assertTrue(srcline, srcfile, description, targetString.find(startString) == 0);

}

#ifdef MOCKPP_UNICODE

void MOCKPP_API_IMPL0 assertStartsWith(unsigned srcline, const char* srcfile,
                                    const String &description,
                                    const MOCKPP_STL::basic_string<wchar_t> &startString,
                                    const MOCKPP_STL::basic_string<wchar_t> &targetString)
{
  assertTrue(srcline, srcfile, description, targetString.find(startString) == 0);
}

#endif

void MOCKPP_API_IMPL0 assertVerifyFails(unsigned srcline, const char* srcfile,
                                     Verifiable *aVerifiable)
{
  bool threwException = false;
  try
  {
    aVerifiable->verify();
  }
  catch (AssertionFailedError & /*ex*/)
  {
    threwException = true;
  }

  assertTrue(srcline, srcfile, mockpp_i18n(MOCKPP_PCHAR("Should not have verified.")), threwException);
}


void MOCKPP_API_IMPL0 notImplemented(unsigned srcline, const char* srcfile,
                                  const String &mockName)
{
  MOCKPP_NS::String fmt = mockpp_i18n(MOCKPP_PCHAR("Not Implemented in %1."));
  fmt << mockName;
  MOCKPP_THROW(NotImplementedException(srcline, srcfile, fmt));
}


MOCKPP_NS_END

