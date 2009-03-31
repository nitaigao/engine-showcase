/** @file
    @brief    common stuff for mockpp project

  $Id: mockpp.cpp 1442 2008-05-09 14:41:02Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Dec 12 2002
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
#include <mockpp/mockpp.h>  // always first header

#include <cstdlib>

MOCKPP_NS_START


MOCKPP_API_IMPL(void)
getVersion (int &major, int &minor, int &patch,
            bool &debug, MOCKPP_STL::string &info)
{
  MOCKPP_STL::string s = MOCKPP_VERSION;
  MOCKPP_STL::string num;
  info = "";

  MOCKPP_STD_NS::size_t pos = s.find('.');
  bool good = true;
  if (pos != MOCKPP_STL::string::npos)
  {
    num = s.substr(0, pos);
    if (num.length() == 0)
      good = false;
#ifdef _MSC_VER_60_70 // MAT using namespace std; instead ? EA: no :-)
    major = atoi(num.c_str());
#else
    major = MOCKPP_STD_NS::atoi(num.c_str());
#endif
    s.erase(0, pos+1);
    pos = s.find('.');

    if (pos != MOCKPP_STL::string::npos)
    {
      num = s.substr(0, pos);
      if (num.length() == 0)
        good = false;
#ifdef _MSC_VER_60_70
      minor = atoi(num.c_str());
#else
      minor = MOCKPP_STD_NS::atoi(num.c_str());
#endif
      s.erase(0, pos+1);

      if (s.length() == 0)
        good = false;
#ifdef _MSC_VER_60_70
      patch = atoi(s.c_str());
#else
      patch = MOCKPP_STD_NS::atoi(s.c_str());
#endif
    }
    else
      good = false;
  }
  else
    good = false;

  if (!good)
  {
    major = -1;
    minor = -1;
    patch = -1;
    info = "";
  }

#ifdef DEBUG
  debug = true;
#else
  debug = false;
#endif
}


#ifdef MOCKPP_UNICODE

MOCKPP_API_IMPL(MOCKPP_STL::basic_string<wchar_t>) getUnicode(const MOCKPP_STL::string &latin1)
{
  MOCKPP_STL::basic_string<wchar_t> ret;
  for (unsigned i = 0; i < latin1.length(); ++i)
    ret += (wchar_t) latin1[i];

  return ret;
}


MOCKPP_API_IMPL(MOCKPP_STL::string) getLatin1(const MOCKPP_STL::basic_string<wchar_t> &uni)
{
  MOCKPP_STL::string ret;
  for (unsigned i = 0; i < uni.length(); ++i)
    ret += (MOCKPP_NS::Char) uni[i];

  return ret;
}

#endif

/**
  * @internal
  */
extern AssertionFailedForwarder_t forwardAssertionFailed;


MOCKPP_API_IMPL(AssertionFailedForwarder_t)
setAssertionFailedForwarder(AssertionFailedForwarder_t fwd)
{
  AssertionFailedForwarder_t temp = forwardAssertionFailed;
  forwardAssertionFailed = fwd;
  return temp;
}


MOCKPP_NS_END
