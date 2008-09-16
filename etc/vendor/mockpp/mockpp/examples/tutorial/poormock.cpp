/***************************************************************************
          poormock.cpp  -  solve problem with really simple mocks

                             -------------------
    begin                : Sun 2 Jan 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: poormock.cpp 1444 2008-05-09 15:13:05Z ewald-arnold $

 ***************************************************************************/

#include "interface.h"
#include "consumer.h"

#include <iostream>

#include <cstdlib>

#ifndef _MSC_VER
using MOCKPP_STD_NS::exit;
#endif

/** Quick-n-dirty asserter.
  * If the condition fails the condition is printed
  * and the application terminates immediately.
  */
#define ASSERTER(x) \
   if (!(x) ) { \
     MOCKPP_STD_NS::cout << MOCKPP_STD_NS::endl \
               << "Error occured.\n" << #x << MOCKPP_STD_NS::endl \
               << MOCKPP_STD_NS::endl \
               << MOCKPP_STD_NS::flush; \
     exit(1); \
   }


class PoorMock : public Interface
{
  public:

    PoorMock()
    {
      counter = 0;
      open_counter = 0;
      read_counter = 0;
      write_counter = 0;
      close_counter = 0;
    }

    virtual void open(const MOCKPP_STL::string & /* name */)
    {
      open_counter = ++counter;
    }

    virtual MOCKPP_STL::string read()
    {
      read_counter = ++counter;
      return "dummy";
    }

    virtual void write(const MOCKPP_STL::string & /* s */)
    {
      write_counter = ++counter;
    }

    virtual unsigned calculate(unsigned input)
    {
      return input + 1;
    }

    virtual void close()
    {
      close_counter = ++counter;
    }

    unsigned counter;
    unsigned open_counter;
    unsigned read_counter;
    unsigned write_counter;
    unsigned close_counter;
};


int main(int /*argc*/, char ** /*argv*/)
{
  PoorMock mock;

  MOCKPP_STD_NS::cout << "Tests starting" << MOCKPP_STD_NS::endl;

  // Run Consumer object
  Consumer consumer(&mock);
  consumer.load();

  ASSERTER(mock.open_counter == 1);
  ASSERTER(mock.read_counter == 4);
  ASSERTER(mock.close_counter == 5);

  consumer.process();
  consumer.save();

  ASSERTER(mock.open_counter == 6);
  ASSERTER(mock.write_counter == 9);
  ASSERTER(mock.close_counter == 10);

  MOCKPP_STD_NS::cout << "Tests finished" << MOCKPP_STD_NS::endl;

  MOCKPP_STD_NS::cout << "All tests have passed successfully" << MOCKPP_STD_NS::endl;
  return 0;
}

