/***************************************************************************
    poormock-se.cpp  -  solve problem with simple mocks, Second Edition

                             -------------------
    begin                : Fri Feb 18 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: poormock-se.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

#include <mockpp/mockpp.h> // always first

#include "interface.h"
#include "consumer.h"

#include <iostream>

#include <mockpp/TrackingCounter.h>
#include <mockpp/MockObject.h>


class PoorMockSecondEdition : public Interface
                            , public MOCKPP_NS::MockObject
{
  private:

    MOCKPP_NS::TrackingCounterMaster counter;

  public:

    PoorMockSecondEdition()
      : MOCKPP_NS::MockObject(MOCKPP_PCHAR("MockObject"))
      , counter(MOCKPP_PCHAR("counter"), this)
      , open_counter(MOCKPP_PCHAR("open_counter"), counter)
      , read_counter(MOCKPP_PCHAR("read_counter"), counter)
      , write_counter(MOCKPP_PCHAR("write_counter"), counter)
      , close_counter(MOCKPP_PCHAR("close_counter"), counter)
    {
    }

    virtual void open(const MOCKPP_STL::string & /* name */)
    {
      open_counter++;
    }

    virtual MOCKPP_STL::string read()
    {
      read_counter++;
      return "dummy";
    }

    virtual void write(const MOCKPP_STL::string & /* s */)
    {
      write_counter++;
    }

    virtual unsigned calculate(unsigned input)
    {
      return input + 1;
    }

    virtual void close()
    {
      close_counter++;
    }

    MOCKPP_NS::TrackingCounterClient open_counter;
    MOCKPP_NS::TrackingCounterClient read_counter;
    MOCKPP_NS::TrackingCounterClient write_counter;
    MOCKPP_NS::TrackingCounterClient close_counter;
};


int main(int /*argc*/, char ** /*argv*/)
{
  PoorMockSecondEdition mock;

  MOCKPP_STD_NS::cout << "Tests starting" << MOCKPP_STD_NS::endl;

  try
  {
    // Run Consumer object
    Consumer consumer(&mock);
    mock.open_counter.setExpected(1);
    mock.read_counter.setExpected(4);
    mock.close_counter.setExpected(5);
    consumer.load();
    mock.verify();

    consumer.process();
    mock.open_counter.setExpected(6);
    mock.write_counter.setExpected(9);
    mock.close_counter.setExpected(10);
    consumer.save();
    mock.verify();

    MOCKPP_STD_NS::cout << "Tests finished" << MOCKPP_STD_NS::endl;

  }
  catch(MOCKPP_STD_NS::exception &ex)
  {
    MOCKPP_STD_NS::cout << MOCKPP_STD_NS::endl
              << "Error occured.\n" << ex.what() << MOCKPP_STD_NS::endl
              << MOCKPP_STD_NS::endl;
    return 1;
  }

  MOCKPP_STD_NS::cout << "All tests have passed successfully" << MOCKPP_STD_NS::endl;
  return 0;
}

