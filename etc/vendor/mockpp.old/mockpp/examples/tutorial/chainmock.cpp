/***************************************************************************
          chainmock.cpp  -  solve problem with chaining mocks

                             -------------------
    begin                : Sun 2 Jan 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: chainmock.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

#include <mockpp/mockpp.h>

#include "interface.h"
#include "consumer.h"

#include <mockpp/chaining/mockpp_pti.h>
#include <mockpp/chaining/ChainableMockObject.h>
#include <mockpp/chaining/ChainingMockObjectSupport.h>

#include <exception>
#include <iostream>

USING_NAMESPACE_MOCKPP


typedef MOCKPP_NS::Invocation1<MOCKPP_STL::string> InvocationString;
typedef MOCKPP_NS::Invocation1<unsigned> InvocationUInt;

/*
MOCKPP_PTI_IMPLEMENT(MOCKPP_NS::Invocation0);
MOCKPP_PTI_IMPLEMENT(InvocationString);
MOCKPP_PTI_IMPLEMENT(InvocationUInt);
*/

class ChainMock : public Interface
                , public MOCKPP_NS::ChainableMockObject
{
  public:

    ChainMock()
      : MOCKPP_NS::ChainableMockObject(MOCKPP_PCHAR("ChainMock"), 0)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE_EXT1(open, ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE0(read)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE_EXT1(write, ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT1(calculate, ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE0(close)
    {}

    MOCKPP_VOID_CHAINABLE_EXT1(ChainMock, open, const MOCKPP_STL::string &,
                                          ext,  MOCKPP_STL::string);

    MOCKPP_CHAINABLE0(ChainMock, MOCKPP_STL::string, read);

    MOCKPP_VOID_CHAINABLE_EXT1(ChainMock, write, const MOCKPP_STL::string &,
                                          ext,   MOCKPP_STL::string);

    MOCKPP_CHAINABLE_EXT1(ChainMock, unsigned, calculate, unsigned,
                                     unsigned, ext,       unsigned);

    MOCKPP_VOID_CHAINABLE0(ChainMock, close);
};


int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    ChainMock mock;
    MOCKPP_CHAINER_FOR_EXT(ChainMock, open, ext) open_chainer (&mock);
    MOCKPP_CHAINER_FOR(ChainMock, read) read_chainer (&mock);
    MOCKPP_CHAINER_FOR_EXT(ChainMock, write, ext) write_chainer (&mock);
    MOCKPP_CHAINER_FOR(ChainMock, close) close_chainer (&mock);
    MOCKPP_CHAINER_FOR_EXT(ChainMock, calculate, ext) calculate_chainer (&mock);

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



