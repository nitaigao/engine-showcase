/** @file
    @brief    Support for CxxTest similar to CppUnit

  $Id: CxxTestSupport.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 **************************************************************************/

/**************************************************************************

   begin                : Fri Dec 9 2005
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

#ifndef MOCKPP_CXXTEST_SUPPORT_H
#define MOCKPP_CXXTEST_SUPPORT_H

////////////////////////////////////////////////////////////////////////////
//

/** Register a class with methods as a test suite.
  * @ingroup grp_framework
  * @param classname name of the class
  */
#define MOCKPP_CXXTEST_SUITE_REGISTRATION(classname) \
  static classname suite_ ## classname; \
  \
  static CXXTEST_NS::List tests_ ## classname = { 0, 0 }; \
  \
  CXXTEST_NS::StaticSuiteDescription suiteDescription_ ## classname( __FILE__, \
                                                                  __LINE__, \
                                                                  #classname, \
                                                                  suite_ ## classname, \
                                                                  tests_ ## classname)

////////////////////////////////////////////////////////////////////////////
//

/** Register a method as test.
  * @ingroup grp_framework
  * @param classname name of the class
  * @param meth      name of the method
  */
#define MOCKPP_CXXTEST(classname, meth) \
static class TestDescription_ ## classname ## _ ## meth \
  : public CXXTEST_NS::RealTestDescription { \
public: \
 TestDescription_ ## classname ## _ ## meth() \
   : CXXTEST_NS::RealTestDescription( tests_ ## classname, \
                                   suiteDescription_ ##classname, \
                                   __LINE__, \
                                   #meth ) \
 {} \
 \
 void runTest() \
 { \
   suite_ ## classname . meth(); \
 } \
} testDescription_ ## classname ## _ ## meth

////////////////////////////////////////////////////////////////////////////
//

// #define MOCKPP_CXXTEST_SUITE_END

#endif // MOCKPP_CXXTEST_SUPPORT_H
