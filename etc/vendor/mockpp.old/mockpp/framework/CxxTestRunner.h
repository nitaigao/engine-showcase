/** @file
    @brief  Run tests based on CXXTEST_NS::TestRunner

  $Id: CxxTestRunner.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sat Dec 10 2005
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

#ifndef MOCKPP_CXXTESTRUNNER_H
#define MOCKPP_CXXTESTRUNNER_H

#include <mockpp/mockpp.h> // always first

#if defined(MOCKPP_USE_CXXTEST)

#include <mockpp/compat/Exception.h>

#include <cxxtest/TestRunner.h>


MOCKPP_NS_START


/** Helper class to run tests with CxxTest framework.
  * @ingroup grp_framework
  */
class CxxTestRunner : public CXXTEST_NS::TestRunner
{
  public:

   /** Internal method wihich runs all the tests.
     * @param listener  pointer to test listener
     */
    static void runAllTests( CXXTEST_NS::TestListener &listener )
    {
        CXXTEST_NS::tracker().setListener( &listener );

        _TS_TRY { CxxTestRunner().runWorld(); }

        _TS_CATCH_TYPE( (MOCKPP_NS::Exception &ex),
                        {CXXTEST_NS::tracker().failedTest( MOCKPP_NS::getLatin1(ex.getSrcFile()).c_str(), ex.getSrcLine(), MOCKPP_NS::getLatin1(ex.getMessage()).c_str() ); } )

        _TS_CATCH_TYPE( (CXXTEST_STD_NS::exception &ex),
                        {CXXTEST_NS::tracker().failedTest( __FILE__, __LINE__, ex.what() ); } )

        _TS_LAST_CATCH( { CXXTEST_NS::tracker().failedTest( __FILE__, __LINE__, "Exception thrown from world" ); } );

        CXXTEST_NS::tracker().setListener( 0 );
    }
};


MOCKPP_NS_END


#endif // MOCKPP_USE_CXXTEST


#endif // MOCKPP_CXXTESTRUNNER_H
