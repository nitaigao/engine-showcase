/***************************************************************************
          visitmock.cpp  -  solve problem with visitable mocks

                             -------------------
    begin                : Sun 2 Jan 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: visitmock.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

#define MOCKPP_IMPORT_ABBREVIATED
#include <mockpp/mockpp.h> // always first

#include <mockpp/visiting/VisitableMockObject.h>

#include <mockpp/chaining/ChainingMockObjectSupport.h>

#include "interface.h"
#include "consumer.h"

#include <exception>
#include <iostream>


class VisitMock : public Interface
                , public MOCKPP_NS::VisitableMockObject
{
  public:

    VisitMock()
      : MOCKPP_NS::VisitableMockObject(MOCKPP_PCHAR("VisitMock"), 0)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT1(open, ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE0(read)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT1(write, ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT1(calculate, ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE0(close)
    {}

    MOCKPP_VOID_VISITABLE_EXT1(VisitMock, open, const MOCKPP_STL::string &,
                                          ext,  MOCKPP_STL::string);

    MOCKPP_VISITABLE0(VisitMock, MOCKPP_STL::string, read);

    MOCKPP_VOID_VISITABLE_EXT1(VisitMock, write, const MOCKPP_STL::string &,
                                          ext,   MOCKPP_STL::string);

    MOCKPP_VISITABLE_EXT1(VisitMock, unsigned, calculate, unsigned,
                                     unsigned, ext,       unsigned);

    MOCKPP_VOID_VISITABLE0(VisitMock, close);
};


int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    VisitMock mock;
    MOCKPP_CONTROLLER_FOR(VisitMock, read) read_controller (&mock);
    MOCKPP_CONTROLLER_FOR_EXT(VisitMock, calculate, ext) calculate_controller (&mock);

    // record program flow while reading data
    mock.open("file1.lst");
    mock.read();
    mock.read();
    mock.read();
    mock.close();

    // provide return values for read()
    read_controller.addReturnValue("record-1");
    read_controller.addReturnValue("record-2");
    read_controller.addReturnValue("record-3");

    // processing is not exactly defined
#if defined(_MSC_VER) && _MSC_VER <= 1300
    mock.calculate(new MOCKPP_NS::IsCloseTo<unsigned>(5, 5));
    mock.calculate(new MOCKPP_NS::IsCloseTo<unsigned>(5, 5));
    mock.calculate(new MOCKPP_NS::IsCloseTo<unsigned>(5, 5));
    calculate_controller.addResponseValue(10, new MOCKPP_NS::IsCloseTo<unsigned>(2, 2));
    calculate_controller.addResponseValue(20, new MOCKPP_NS::IsCloseTo<unsigned>(4, 2));
    calculate_controller.addResponseValue(30, new MOCKPP_NS::IsCloseTo<unsigned>(6, 2));
#else
    mock.calculate(eq<unsigned>(5, 5));
    mock.calculate(eq<unsigned>(5, 5));
    mock.calculate(eq<unsigned>(5, 5));
    calculate_controller.addResponseValue(10, eq<unsigned>(2, 2));
    calculate_controller.addResponseValue(20, eq<unsigned>(4, 2));
    calculate_controller.addResponseValue(30, eq<unsigned>(6, 2));
#endif

    // record program flow while writing data
    mock.open("file1.lst");
    mock.write("record-1/processed");
    mock.write(stringContains(MOCKPP_STL::string("processed")));
    mock.write(stringContains(MOCKPP_STL::string("processed")));
    mock.close();

    // activate mock object
    mock.activate();

    // Run Consumer object
    MOCKPP_STD_NS::cout << "Tests starting" << MOCKPP_STD_NS::endl;

    Consumer consumer(&mock);
    consumer.load();
    consumer.process();
    consumer.save();

    MOCKPP_STD_NS::cout << "Tests finished" << MOCKPP_STD_NS::endl;

    // Check pending expectations
    mock.verify();

    MOCKPP_STD_NS::cout << "All tests have passed successfully" << MOCKPP_STD_NS::endl;
  }
  catch(MOCKPP_STD_NS::exception &ex)
  {
    MOCKPP_STD_NS::cout << MOCKPP_STD_NS::endl
              << "Error occured.\n" << ex.what() << MOCKPP_STD_NS::endl
              << MOCKPP_STD_NS::endl;
    return 1;
  }

  return 0;
}

