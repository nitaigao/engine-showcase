/***************************************************************************
          chainmock.cpp  -  solve problem with chaining mocks

                             -------------------
    begin                : Sun 2 Jan 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: chainmock2.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

#include <mockpp/mockpp.h>

#include "interface.h"
#include "consumer.h"

#include <mockpp/chaining/ChainableMockObject.h>

#include <mockpp/chaining/CountedChainableMethod.h>
#include <mockpp/chaining/ChainingMockObjectSupport.h>

#include <exception>
#include <iostream>

USING_NAMESPACE_MOCKPP

class ChainMock : public Interface
                , public MOCKPP_NS::ChainableMockObject
{
  public:

    ChainMock()
      : MOCKPP_NS::ChainableMockObject(MOCKPP_PCHAR("ChainMock"), 0)
      , open_mocker(MOCKPP_PCHAR("open"), this)
      , read_mocker(MOCKPP_PCHAR("read"), this)
      , write_mocker(MOCKPP_PCHAR("write"), this)
      , close_mocker(MOCKPP_PCHAR("close"), this)
      , calculate_mocker(MOCKPP_PCHAR("calculate"), this)
    {}

    void open(const MOCKPP_STL::string &filename)
    {
      open_mocker.forward(filename);
    }

    MOCKPP_STL::string read()
    {
      return read_mocker.forward();
    }

    void write(const MOCKPP_STL::string &data)
    {
      write_mocker.forward(data);
    }

    unsigned calculate(unsigned input)
    {
      return calculate_mocker.forward(input);
    }

    void close()
    {
      close_mocker.forward();
    }

    MOCKPP_NS::ChainableMockMethod<void, MOCKPP_STL::string>   open_mocker;
    MOCKPP_NS::ChainableMockMethod<MOCKPP_STL::string>         read_mocker;
    MOCKPP_NS::ChainableMockMethod<void, MOCKPP_STL::string>   write_mocker;
    MOCKPP_NS::ChainableMockMethod<void>                close_mocker;
    MOCKPP_NS::ChainableMockMethod<unsigned, unsigned>  calculate_mocker;
};


int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    ChainMock mock;

    MOCKPP_NS::ChainableMockMethod<void, MOCKPP_STL::string>  &open_chainer  (mock.open_mocker);
    MOCKPP_NS::ChainableMockMethod<MOCKPP_STL::string>        &read_chainer  (mock.read_mocker);
    MOCKPP_NS::ChainableMockMethod<void, MOCKPP_STL::string>  &write_chainer (mock.write_mocker);
    MOCKPP_NS::ChainableMockMethod<void>               &close_chainer (mock.close_mocker);
    MOCKPP_NS::ChainableMockMethod<unsigned, unsigned> &calculate_chainer (mock.calculate_mocker);

    // Expectations for reading the file
    open_chainer.expects(once())
                .with(eq(MOCKPP_STL::string("file1.lst")))
                .before(MOCKPP_PCHAR("reader"));

    read_chainer.stubs()
                .will(onConsecutiveCalls(new ReturnStub<MOCKPP_STL::string>("record-1"),
                                         new ReturnStub<MOCKPP_STL::string>("record-2"),
                                         new ReturnStub<MOCKPP_STL::string>("record-3")))
                .id(MOCKPP_PCHAR("reader"));

    close_chainer.expects(once())
                 .after(MOCKPP_PCHAR("reader"));


    // Expectations for processing the file
    calculate_chainer.expects(atLeast(3))
                     .with(eq<unsigned>(5, 5))
                     .after(MOCKPP_PCHAR("reader"))
                     .will(returnValue<unsigned>(1));

    // Expectations for writing the file back
    open_chainer.expects(once())
                .with(eq(MOCKPP_STL::string("file1.lst")))
                .before(MOCKPP_PCHAR("writer"));

    write_chainer.expects(once())
                 .with(eq(MOCKPP_STL::string("record-1/processed")));
    write_chainer.expects(once())
                 .with(eq(MOCKPP_STL::string("record-2/processed")));
    write_chainer.expects(once())
                 .with(stringContains(MOCKPP_STL::string("processed")))
                 .id(MOCKPP_PCHAR("writer"));

    close_chainer.expects(once())
                 .after(MOCKPP_PCHAR("writer"));


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



