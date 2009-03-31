/***************************************************************************
          basicmock.cpp  -  solve problem with basic mocks

                             -------------------
    begin                : Sun 2 Jan 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: basicmock.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

#include <mockpp/mockpp.h> // always first

#include <mockpp/MockObject.h>
#include <mockpp/ExpectationList.h>
#include <mockpp/ReturnObjectList.h>
#include <mockpp/ExpectationCounter.h>

#include <mockpp/constraint/ConstraintList.h>

#include <mockpp/chaining/ChainingMockObjectSupport.h>

#include "interface.h"
#include "consumer.h"

#include <exception>
#include <iostream>

USING_NAMESPACE_MOCKPP


class BasicMock : public Interface
                , public MOCKPP_NS::MockObject
{
  public:

    BasicMock()
      : MOCKPP_NS::MockObject(MOCKPP_PCHAR("BasicMock"), 0)
      , open_name(MOCKPP_PCHAR("BasicMock/open_name"), this)
      , close_counter(MOCKPP_PCHAR("BasicMock/close_counter"), this)
      , write_data(MOCKPP_PCHAR("BasicMock/write_data"), this)
      , read_data(MOCKPP_PCHAR("BasicMock/read_data"), this)
      , calculate_output(MOCKPP_PCHAR("BasicMock/calculate_output"), this)
      , calculate_input(MOCKPP_PCHAR("BasicMock/calculate_input"), this)
    {
    }

    virtual void open(const MOCKPP_STL::string &name)
    {
      open_name.addActual(name);
    }

    virtual MOCKPP_STL::string read()
    {
      return read_data.nextReturnObject();
    }

    virtual void write(const MOCKPP_STL::string &s)
    {
      write_data.addActual(s);
    }

    virtual unsigned calculate(unsigned input)
    {
      calculate_input.addActual(input);
      return calculate_output.nextReturnObject();
    }

    virtual void close()
    {
      close_counter.inc();
    }

    MOCKPP_NS::ExpectationList<MOCKPP_STL::string>  open_name;
    MOCKPP_NS::ExpectationCounter            close_counter;
    MOCKPP_NS::ExpectationList<MOCKPP_STL::string>  write_data;
    MOCKPP_NS::ReturnObjectList<MOCKPP_STL::string> read_data;
    MOCKPP_NS::ReturnObjectList<unsigned>    calculate_output;
    MOCKPP_NS::ConstraintList<unsigned >     calculate_input;
};


int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    BasicMock mock;

    // open() on reading and writing
    mock.open_name.addExpected("file1.lst");
    mock.open_name.addExpected("file1.lst");

    // close() on reading and writing
    mock.close_counter.setExpected(2);

    // Read exactly 3 lines
    mock.read_data.addObjectToReturn("record-1");
    mock.read_data.addObjectToReturn("record-2");
    mock.read_data.addObjectToReturn("record-3");

    // Write exactly 3 lines
    mock.write_data.addExpected("record-1/processed");
    mock.write_data.addExpected("record-2/processed");
    mock.write_data.addExpected("record-3/processed");

    // processing is not exactly defined
    mock.calculate_input.addExpected(eq<unsigned>(5,5));
    mock.calculate_input.addExpected(eq<unsigned>(5,5));
    mock.calculate_input.addExpected(eq<unsigned>(5,5));
    mock.calculate_output.addObjectToReturn(10);
    mock.calculate_output.addObjectToReturn(20);
    mock.calculate_output.addObjectToReturn(30);

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
