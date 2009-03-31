/***************************************************************************
   SelectUnittestFramework.h  -  setup for unittest framework in use
                             -------------------
    begin                : Fri Dec 9 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: SelectUnittestFramework.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 ***************************************************************************/

#ifndef MOCKPP_SELECTUNITTESTFRAMEWORK_H
#define MOCKPP_SELECTUNITTESTFRAMEWORK_H

/** @defgroup grp_framework Support for test frameworks.
  * Mockpp as no test framework of its own. For that reason
  * there is support for various existing test frameworks.
  */

/** @def MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
  * @ingroup grp_framework
  * Contains a code fragment to create a test suite that contains
  * one or more test cases which are arranged as class methods.
  * This is done by publicly deriving from an according class of
  * the test framework.
  * For frameworks that don't support this, the macro is empty.
  */

/** @def MOCKPP_TESTMETHOD_DECL
  * @ingroup grp_framework
  * Depending on the test framework the test methods are
  * regular class methods or rather static to react like free
  * functions. So this macro is used to declare the methods accordingly.
  */

#ifdef DOXYGEN_SHOULD_INCLUDE_THIS // make doxygen happily include docs
#define MOCKPP_USE_CXXTEST
#define MOCKPP_USE_CPPUNIT
#define MOCKPP_USE_BOOSTTEST
#endif

/** @def MOCKPP_USE_CXXTEST
  * @ingroup grp_config
  * Defined if CxxTest is used as framework.
  */

/** @def MOCKPP_USE_CPPUNIT
  * @ingroup grp_config
  * Defined if CppUnit is used as framework.
  */

/** @def MOCKPP_USE_BOOSTTEST
  * @ingroup grp_config
  * Defined if Boost.Test is used as framework.
  */

//=================================================
#if defined(MOCKPP_USE_CXXTEST)
//=================================================

# define MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK : public CXXTEST_NS::TestSuite

#define MOCKPP_TESTMETHOD_DECL /* static or not */

#define CXXTEST_HAVE_STD
#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD

#ifndef MOCKPP_NO_EXCEPTIONS
#define CXXTEST_ABORT_TEST_ON_FAIL
#define _CXXTEST_HAVE_EH
#endif

#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/RealDescriptions.h>

#include <mockpp/framework/CxxTestSupport.h>

//=================================================
#elif defined(MOCKPP_USE_CPPUNIT)
//=================================================

#define MOCKPP_TESTMETHOD_DECL /* not static */

# include <cppunit/extensions/HelperMacros.h>
# define MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK : public CppUnit::TestFixture

//=================================================
#elif defined(MOCKPP_USE_BOOSTTEST)
//=================================================

# define MOCKPP_TESTMETHOD_DECL static

# include <boost/test/auto_unit_test.hpp>

# define MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK /* : public boost::test_case */

/** Register a method as test.
  * The content of this macro is a part of boost macro
  * BOOST_AUTO_UNIT_TEST
  * @ingroup grp_framework
  * @param funcname name of the free function or static class method.
  */
# define MOCKPP_BOOST_TEST(func_name) \
static boost::unit_test_framework::detail::auto_unit_test_registrar \
    BOOST_JOIN( test_registrar, __LINE__)                           \
        ( BOOST_TEST_CASE( func_name ) );

//=================================================
#else
//=================================================

# pragma message ("No unittest framework available at compile time")
# define MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK   /**/

#endif // frameworks

#endif // MOCKPP_SELECTUNITTESTFRAMEWORK_H
