 /***************************************************************************
    readability_test.cpp  -  visually check readability of output
                             -------------------
    begin                : Sat Apr 09 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: readability_test.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#include <mockpp/mockpp.h> // always first

#include <iostream>

#include <mockpp/MixedMockObject.h>

#define MOCKPP_IMPORT_ABBREVIATED
#include <mockpp/chaining/ChainingMockObjectSupport.h>
#include <mockpp/chaining/mockpp_pti.h>

#include <mockpp/constraint/ConstraintList.h>

#include <mockpp/visiting/VisitableMockMethod1.h>
#include <mockpp/visiting/VisitableMockMethod0.h>

#include "classes_ABCDE.h"

class MyMixedMockObject : public MOCKPP_NS::MixedMockObject
{
  public:

    MyMixedMockObject()
      : MOCKPP_NS::MixedMockObject(MOCKPP_PCHAR("MyMixedMockObjectInstance") , 0)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE5(chain)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE5(visit)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE5(void_chain)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE5(void_visit)
      , chainer(this)
      , void_chainer(this)
      , visiter(this)
      , void_visiter(this)
    {}

    MOCKPP_CHAINABLE5(MyMixedMockObject, int, chain, CA, CB, MOCKPP_STL::string, char, bool);
    MOCKPP_VISITABLE5(MyMixedMockObject, int, visit, CA, CB, MOCKPP_STL::string, char, bool);
    MOCKPP_VOID_CHAINABLE5(MyMixedMockObject, void_chain, CA, CB, MOCKPP_STL::string, char, bool);
    MOCKPP_VOID_VISITABLE5(MyMixedMockObject, void_visit, CA, CB, MOCKPP_STL::string, char, bool);

    MOCKPP_CHAINER_FOR(MyMixedMockObject, chain) chainer;
    MOCKPP_CHAINER_FOR(MyMixedMockObject, void_chain) void_chainer;
    MOCKPP_CONTROLLER_FOR(MyMixedMockObject, visit) visiter;
    MOCKPP_CONTROLLER_FOR(MyMixedMockObject, void_visit) void_visiter;
};


void chainer_1()
{
  MyMixedMockObject mmo;
  mmo.chainer.expects(exactly(3))
             .with(eq(CA()),
                   eq(CB(3)),
                   stringContains(MOCKPP_STL::string("string")),
                   eq<char>(123, 45),
                   eq(true))
             .will(returnValue(321));

  mmo.chainer.expects(atLeast(3))
             .with(eq(CA(5)),
                   eq(CB()),
                   stringContains(MOCKPP_STL::string("no-string")),
                   eq<char>(0x23),
                   eq(false))
             .will(returnValue(123));

  MOCKPP_TRY
  {
    mmo.chain(CA(), CB(), "hallo", 12, true);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_STD_NS::exception &ex)
  {
    MOCKPP_STD_NS::cout << ex.what() << MOCKPP_STD_NS::endl;
  }
#endif
}


void visiter_1()
{
  MyMixedMockObject mmo;
  mmo.void_visit(CA(), CB(), "hallo", 12, true);
  mmo.activate();

  MOCKPP_TRY
  {
    mmo.visit(CA(), CB(), "hallo", 12, false);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_STD_NS::exception &ex)
  {
    MOCKPP_STD_NS::cout << ex.what() << MOCKPP_STD_NS::endl;
  }
#endif
}


void visiter_2()
{
  MyMixedMockObject mmo;
  mmo.void_visit(CA(), CB(), "hallo", 12, true);
  mmo.activate();

  MOCKPP_TRY
  {
    mmo.void_visit(CA(), CB(), "hallo", 21, false);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_STD_NS::exception &ex)
  {
    MOCKPP_STD_NS::cout << ex.what() << MOCKPP_STD_NS::endl;
  }
#endif
}


void constraintlist_1()
{
  MOCKPP_NS::ConstraintList<MOCKPP_STL::string> list(MOCKPP_PCHAR("ConstraintListInstance"));
  list.addExpected(eq(MOCKPP_STL::string("string")));
  MOCKPP_TRY
  {
    list.addActual(MOCKPP_STL::string("no-string"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_STD_NS::exception &ex)
  {
    MOCKPP_STD_NS::cout << ex.what() << MOCKPP_STD_NS::endl;
  }
#endif
}


void expectationlist_1()
{
  MOCKPP_NS::ExpectationList<MOCKPP_STL::string> list(MOCKPP_PCHAR("ExpectationListInstance"));
  list.addExpected(MOCKPP_STL::string("string"));
  MOCKPP_TRY
  {
    list.addActual(MOCKPP_STL::string("no-string"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_STD_NS::exception &ex)
  {
    MOCKPP_STD_NS::cout << ex.what() << MOCKPP_STD_NS::endl;
  }
#endif
}


void expectationlist_2()
{
  MOCKPP_NS::ExpectationList<MOCKPP_STL::string> list(MOCKPP_PCHAR("ExpectationListInstance") );
  list.addExpected(MOCKPP_STL::string("string"));
  list.addActual(MOCKPP_STL::string("string"));
  MOCKPP_TRY
  {
    list.addActual(MOCKPP_STL::string("no-string"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_STD_NS::exception &ex)
  {
    MOCKPP_STD_NS::cout << ex.what() << MOCKPP_STD_NS::endl;
  }
#endif
}


void visitablemethod_test()
{
  MOCKPP_NS::VisitableMockObject parent(MOCKPP_PCHAR("visitparent"));
  MOCKPP_NS::VisitableMockMethod1<int, int> visiti(MOCKPP_PCHAR("int1tester"), &parent);

  parent.activate();

  MOCKPP_TRY
  {
    visiti.forward(eq(1));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_STD_NS::exception &ex)
  {
    MOCKPP_STD_NS::cout << ex.what() << MOCKPP_STD_NS::endl;
  }
#endif

  MOCKPP_TRY
  {
    visiti.forward(1);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_STD_NS::exception &ex)
  {
    MOCKPP_STD_NS::cout << ex.what() << MOCKPP_STD_NS::endl;
  }
#endif
}


int main(int /* argc */, char ** /* argv */)
{
  unsigned result = 0;
  MOCKPP_STD_NS::cout << "Starting readability tests..\n";

  MOCKPP_TRY
  {
    MOCKPP_STD_NS::cout << "--\n";

    chainer_1();
    MOCKPP_STD_NS::cout << "--\n";

    visiter_1();
    MOCKPP_STD_NS::cout << "--\n";

    visiter_2();
    MOCKPP_STD_NS::cout << "--\n";

    expectationlist_1();
    MOCKPP_STD_NS::cout << "--\n";

    expectationlist_2();
    MOCKPP_STD_NS::cout << "--\n";

    constraintlist_1();
    MOCKPP_STD_NS::cout << "--\n";

    visitablemethod_test();
    MOCKPP_STD_NS::cout << "--\n";
  }

#ifndef MOCKPP_NO_EXCEPTIONS
  catch(...)
  {
    MOCKPP_STD_NS::cout << "Caught unexpected exception\n";
    result = 1;
  }
#endif

  MOCKPP_STD_NS::cout << "Finishing readability tests\n";
  return result;
}
