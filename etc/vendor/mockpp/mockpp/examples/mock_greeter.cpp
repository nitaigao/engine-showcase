/***************************************************************************
          mock_greeter.cpp  -  mock greeting into a stream

        heavily inspired by an example from the Ruby/Mock package

                             -------------------
    begin                : Wed Jan 1 2003
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: mock_greeter.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#include <mockpp/framework/SelectUnittestFramework.h>

#include <iostream>

#if defined(MOCKPP_USE_CXXTEST)

#include <cxxtest/TestRunner.h>

#include <mockpp/framework/CxxTestRunner.h>

#if defined(GUI_QT)
# include <cxxtest/StdioPrinter.h>
# include <cxxtest/QtGui.h>
#elif defined(GUI_X11)
# include <cxxtest/StdioPrinter.h>
# include <cxxtest/X11Gui.h>
#elif defined(GUI_WIN32)
# include <cxxtest/StdioPrinter.h>
# include <cxxtest/Win32Gui.h>
#else
# include <cxxtest/ErrorPrinter.h>
#endif

#elif defined(MOCKPP_USE_CPPUNIT)

# include <cppunit/extensions/HelperMacros.h>
# include <cppunit/extensions/TestFactoryRegistry.h>
# include <cppunit/ui/text/TestRunner.h>
# include <cppunit/extensions/HelperMacros.h>

#endif

#include <mockpp/MockObject.h>
#include <mockpp/ExpectationValue.h>
#include <mockpp/ExpectationCounter.h>
#include <mockpp/ReturnObjectList.h>


class StorageInterface
{
  public:
    virtual ~StorageInterface() {}
    virtual void open() = 0;
    virtual void write(const MOCKPP_NS::String &msg) = 0;
    virtual MOCKPP_NS::String read() = 0;
    virtual void close() = 0;
};

// The class under test.  It writes a greeting into persistent storage.
// To test it, we will need to mock the storage object.
//
// For simplicity the code contains some conditionals to simulate common bugs.
//
class HelloWriter
{
  public:
    enum BugType
    {
       BugNone,
       BugMissingOpen,
       BugMissingCloseOnError,
       BugMissingCloseOnEnd
    };


    HelloWriter(BugType bt, const MOCKPP_NS::String who = MOCKPP_PCHAR("world") )
    {
      bugType = bt;
      who_is = who;
    }


    void do_hello(StorageInterface &storage)
    {
      if (bugType != BugMissingOpen)
        storage.open();
      try
      {
        MOCKPP_NS::String greet = MOCKPP_PCHAR("hello, %1");
        greet << who_is;
        storage.write( greet );
      }
      catch(...)
      {
        if (bugType != BugMissingCloseOnError)
          storage.close();
        throw;
      }
      if (bugType != BugMissingCloseOnEnd)
        storage.close();
    }


    void do_listen(StorageInterface &storage)
    {
      if (bugType != BugMissingOpen)
        storage.open();
      try
      {
        MOCKPP_NS::String s;
        s = storage.read();
      }
      catch(...)
      {
        if (bugType != BugMissingCloseOnError)
          storage.close();
        throw;
      }
      if (bugType != BugMissingCloseOnEnd)
        storage.close();
    }


  private:
    MOCKPP_NS::String  who_is;
    BugType         bugType;
};


class StorageMockWriter : public MOCKPP_NS::MockObject,
                          public StorageInterface
{
  public:

    StorageMockWriter()
      : MOCKPP_NS::MockObject(MOCKPP_PCHAR("mockWriter")),
        open_cnt (MOCKPP_PCHAR("storage open() counter"), this),
        write_cnt(MOCKPP_PCHAR("storage write() counter"), this),
        close_cnt(MOCKPP_PCHAR("storage close() counter"), this),
        greet_str(MOCKPP_PCHAR("input value for write()"), this),
        o_state  (MOCKPP_PCHAR("state indicating stream-open"), this)
    {
      // expect: open, write, [write (fails)], close
      open_cnt.setExpected(1);
      write_cnt.setExpected(1, 2);
      close_cnt.setExpected(1);

      o_state.setExpected(false);
      o_state.setFailOnVerify();
      o_state.setActual(false);

      greet_str.setExpected(MOCKPP_PCHAR("hello, world"));  // expect correct greeting
    }


    void open()
    {
      o_state.setActual(true);
      open_cnt.inc();
    }


    MOCKPP_NS::String read()
    {
      MOCKPP_NOT_IMPLEMENTED(MOCKPP_GET_STRING(MOCKPP_FUNC_MACRO));
      return MOCKPP_PCHAR("");
    }


    void write(const MOCKPP_NS::String &s)
    {
      write_cnt.inc();
      greet_str.setActual(s);
    }


    void close()
    {
      close_cnt.inc();
      o_state.setActual(false);
    }


  private:

    MOCKPP_NS::ExpectationCounter                open_cnt;
    MOCKPP_NS::ExpectationCounter                write_cnt;
    MOCKPP_NS::ExpectationCounter                close_cnt;

    MOCKPP_NS::ExpectationValue<MOCKPP_NS::String>  greet_str;
    MOCKPP_NS::ExpectationValue<bool>            o_state;
};


class StorageMockReader : public MOCKPP_NS::MockObject,
                          public StorageInterface
{
  public:

    StorageMockReader()
      : MOCKPP_NS::MockObject(MOCKPP_PCHAR("mockReader")),
        open_cnt (MOCKPP_PCHAR("storage open() counter"), this),
        read_cnt (MOCKPP_PCHAR("storage read() counter"), this),
        close_cnt(MOCKPP_PCHAR("storage close() counter"), this),
        ret_vals (MOCKPP_PCHAR("return values for read()"), this),
        o_state  (MOCKPP_PCHAR("state indicating stream-open"), this)
    {
      // expect: open, read, [read (fails)], close
      open_cnt.setExpected(1);
      read_cnt.setExpected(1, 2);
      close_cnt.setExpected(1);

      o_state.setExpected(false);
      o_state.setFailOnVerify();

      ret_vals.addObjectToReturn(MOCKPP_PCHAR("good bye, mock turtle")); // provide one return val
    }


    void open()
    {
      o_state.setActual(true);
      open_cnt.inc();
    }


    void write(const MOCKPP_NS::String &)
    {
      MOCKPP_NOT_IMPLEMENTED(MOCKPP_GET_STRING(MOCKPP_FUNC_MACRO));
    }


    MOCKPP_NS::String read()
    {
      read_cnt.inc();
      return ret_vals.nextReturnObject();
    }


    void close()
    {
      close_cnt.inc();
      o_state.setActual(false);
    }


  private:

    MOCKPP_NS::ExpectationCounter                open_cnt;
    MOCKPP_NS::ExpectationCounter                read_cnt;
    MOCKPP_NS::ExpectationCounter                close_cnt;
    MOCKPP_NS::ReturnObjectList<MOCKPP_NS::String>  ret_vals;
    MOCKPP_NS::ExpectationValue<bool>            o_state;
};


class Streamer_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( Streamer_test );

 CPPUNIT_TEST(test_write_ok);
 CPPUNIT_TEST(test_read_ok);
 CPPUNIT_TEST(fail_wrong_name);
 CPPUNIT_TEST(fail_missing_open);
 CPPUNIT_TEST(fail_missing_read);
 CPPUNIT_TEST(fail_missing_close);
 CPPUNIT_TEST(fail_missing_close_on_error);

CPPUNIT_TEST_SUITE_END();

#endif // MOCKPP_USE_CPPUNIT

  public:

    void test_write_ok();
    void test_read_ok();

    void fail_wrong_name();
    void fail_missing_open();
    void fail_missing_read();
    void fail_missing_close();
    void fail_missing_close_on_error();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( Streamer_test );

MOCKPP_CXXTEST(Streamer_test, test_write_ok);
MOCKPP_CXXTEST(Streamer_test, test_read_ok);
MOCKPP_CXXTEST(Streamer_test, fail_wrong_name);
MOCKPP_CXXTEST(Streamer_test, fail_missing_open);
MOCKPP_CXXTEST(Streamer_test, fail_missing_read);
MOCKPP_CXXTEST(Streamer_test, fail_missing_close);
MOCKPP_CXXTEST(Streamer_test, fail_missing_close_on_error);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION(Streamer_test);

#endif // MOCKPP_USE_CPPUNIT


void Streamer_test::test_write_ok()
{
  HelloWriter hw(HelloWriter::BugNone, MOCKPP_PCHAR("world"));
  StorageMockWriter sm;
  hw.do_hello(sm);
  sm.verify();
}


void Streamer_test::test_read_ok()
{
  HelloWriter hw(HelloWriter::BugNone, MOCKPP_PCHAR("world"));
  StorageMockReader sm;
  hw.do_listen(sm);
  sm.verify();
}


void Streamer_test::fail_missing_read()
{
  HelloWriter hw(HelloWriter::BugNone, MOCKPP_PCHAR("world"));
  StorageMockReader sm;
  // no read here
  bool thrown = false;
  MOCKPP_TRY
  {
    sm.verify();
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void Streamer_test::fail_wrong_name()
{
  HelloWriter hw(HelloWriter::BugNone, MOCKPP_PCHAR("alice")); // wrong name
  StorageMockWriter sm;
  hw.do_hello(sm);
  bool thrown = false;
  MOCKPP_TRY
  {
    sm.verify();
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void Streamer_test::fail_missing_open()
{
  HelloWriter hw(HelloWriter::BugMissingOpen, MOCKPP_PCHAR("world"));
  StorageMockWriter sm;
  hw.do_hello(sm);
  bool thrown = false;
  MOCKPP_TRY
  {
    sm.verify();
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void Streamer_test::fail_missing_close()
{
  HelloWriter hw(HelloWriter::BugMissingCloseOnEnd, MOCKPP_PCHAR("world"));
  StorageMockWriter sm;
  hw.do_hello(sm);
  bool thrown = false;
  MOCKPP_TRY
  {
    sm.verify();
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void Streamer_test::fail_missing_close_on_error()
{
  HelloWriter hw(HelloWriter::BugMissingCloseOnError, MOCKPP_PCHAR("world"));
  StorageMockReader sm;
  hw.do_listen(sm);
  hw.do_listen(sm);
  bool thrown = false;
  MOCKPP_TRY
  {
    sm.verify();
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}

#if defined(MOCKPP_USE_CXXTEST)

int run_cxxtest(int argc, char **argv)
{
#if defined(GUI_QT)
  return CXXTEST_NS::GuiTuiRunner<CXXTEST_NS::QtGui, CXXTEST_NS::StdioPrinter>( argc, argv ).run();
#elif defined(GUI_X11)
  return CXXTEST_NS::GuiTuiRunner<CXXTEST_NS::X11Gui, CXXTEST_NS::StdioPrinter>( argc, argv ).run();
#elif defined(GUI_WIN32)
  return CXXTEST_NS::GuiTuiRunner<CXXTEST_NS::Win32Gui, CXXTEST_NS::StdioPrinter>( argc, argv ).run();
#else
   CXXTEST_NS::ErrorPrinter ep;
   MOCKPP_NS::CxxTestRunner::runAllTests(ep);
   return CXXTEST_NS::tracker().failedTests();
   //return CXXTEST_NS::ErrorPrinter().run();
#endif
}

#elif defined(MOCKPP_USE_CPPUNIT)

int run_cppunit(int argc, char **argv)
{
  CppUnit::TextUi::TestRunner runner;
  CppUnit::TestFactory &registry = CppUnit::TestFactoryRegistry::getRegistry();
  runner.addTest(registry.makeTest());
  return runner.run() ? 0 : 1;
}

#endif

int main(int argc, char ** argv)
{
  int ret = 1;

#if defined(MOCKPP_USE_CXXTEST)

  ret = run_cxxtest(argc, argv);

#elif defined(MOCKPP_USE_CPPUNIT)

  ret = run_cppunit(argc, argv);

  MOCKPP_STD_NS::cout << "\n";

#else
  MOCKPP_STD_NS::cout << MOCKPP_NS::getLatin1(mockpp_i18n("Test framework was not available at compile time!\n")).c_str();
  MOCKPP_STD_NS::cout << MOCKPP_NS::getLatin1(mockpp_i18n("Will use mockpp features only.\n\n")).c_str();
#endif

  try
  {
    MOCKPP_STD_NS::cout << MOCKPP_NS::getLatin1(mockpp_i18n("Now running some tests with just mockpp features:\n\n")).c_str();
    Streamer_test st;

    st.test_write_ok();
    st.test_read_ok();

    bool thrown = false;
    try
    {
      st.fail_missing_close();
    }
    catch (MOCKPP_NS::AssertionFailedError & /*ae*/)
    {
      thrown = true;
    }
    MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("Missing close() not detected."), thrown == true);

    thrown = false;
    try
    {
      st.fail_missing_close_on_error();
    }
    catch (MOCKPP_NS::AssertionFailedError & /*ae*/)
    {
      thrown = true;
    }
    MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("Missing close() after error not detected."), thrown == true);

    ret = 0;
  }
  catch(MOCKPP_NS::Exception &ex)
  {
    MOCKPP_STD_NS::cout << "file: " << MOCKPP_NS::getLatin1(ex.getSrcFile()).c_str() << MOCKPP_STD_NS::endl
              << "line: " << ex.getSrcLine() << MOCKPP_STD_NS::endl
              << "msg : " << MOCKPP_NS::getLatin1(ex.getMessage()).c_str() << MOCKPP_STD_NS::endl;
    ret = 1;
  }
  MOCKPP_STD_NS::cout << MOCKPP_NS::getLatin1(mockpp_i18n("Finished.\n")).c_str();
  return ret;
}

