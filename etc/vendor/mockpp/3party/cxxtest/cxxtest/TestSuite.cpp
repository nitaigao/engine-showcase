#ifndef __cxxtest__TestSuite_cpp__
#define __cxxtest__TestSuite_cpp__

#define CXXTEST_NEED_EXPORTS
#include <cxxtest/Flags.h>

#include <cxxtest/TestSuite.h>

CXXTEST_NS_START
    //
    // TestSuite members
    //
    TestSuite::~TestSuite() {}
    void TestSuite::setUp() {}
    void TestSuite::tearDown() {}

    //
    // Test-aborting stuff
    //
    static bool currentAbortTestOnFail = false;

    CXXTEST_API_IMPL(bool) abortTestOnFail()
    {
        return currentAbortTestOnFail;
    }

    CXXTEST_API_IMPL(void) setAbortTestOnFail( bool value )
    {
        currentAbortTestOnFail = value;
    }

    CXXTEST_API_IMPL(void) doAbortTest()
    {
#   if defined(_CXXTEST_HAVE_EH)
        if ( currentAbortTestOnFail )
            throw AbortTest();
#   endif // _CXXTEST_HAVE_EH
    }

    //
    // Max dump size
    //
    static unsigned currentMaxDumpSize = CXXTEST_MAX_DUMP_SIZE;

    CXXTEST_API_IMPL(unsigned) maxDumpSize()
    {
        return currentMaxDumpSize;
    }

    CXXTEST_API_IMPL(void) setMaxDumpSize( unsigned value )
    {
        currentMaxDumpSize = value;
    }

    //
    // Some non-template functions
    //
    CXXTEST_API_IMPL(void) doTrace( const char *file, unsigned line, const char *message )
    {
        tracker().trace( file, line, message );
    }

    CXXTEST_API_IMPL(void) doWarn( const char *file, unsigned line, const char *message )
    {
        tracker().warning( file, line, message );
    }

    CXXTEST_API_IMPL(void) doFailTest( const char *file, unsigned line, const char *message )
    {
        tracker().failedTest( file, line, message );
        TS_ABORT();
    }

    CXXTEST_API_IMPL(void) doFailAssert( const char *file, unsigned line,
                       const char *expression, const char *message )
    {
        if ( message )
            tracker().failedTest( file, line, message );
        tracker().failedAssert( file, line, expression );
        TS_ABORT();
    }

    CXXTEST_API_IMPL(bool) sameData( const void *x, const void *y, unsigned size )
    {
        if ( size == 0 )
            return true;

        if ( x == y )
            return true;

        if ( !x || !y )
            return false;

        const char *cx = (const char *)x;
        const char *cy = (const char *)y;
        while ( size -- )
            if ( *cx++ != *cy++ )
                return false;

        return true;
    }

    CXXTEST_API_IMPL(void) doAssertSameData( const char *file, unsigned line,
                           const char *xExpr, const void *x,
                           const char *yExpr, const void *y,
                           const char *sizeExpr, unsigned size,
                           const char *message )
    {
        if ( !sameData( x, y, size ) ) {
            if ( message )
                tracker().failedTest( file, line, message );
            tracker().failedAssertSameData( file, line, xExpr, yExpr, sizeExpr, x, y, size );
            TS_ABORT();
        }
    }

    CXXTEST_API_IMPL(void) doFailAssertThrows( const char *file, unsigned line,
                             const char *expr, const char *type,
                             bool otherThrown,
                             const char *message )
    {
        if ( message )
            tracker().failedTest( file, line, message );

        tracker().failedAssertThrows( file, line, expr, type, otherThrown );
        TS_ABORT();
    }

    CXXTEST_API_IMPL(void) doFailAssertThrowsNot( const char *file, unsigned line,
                                const char *expression, const char *message )
    {
        if ( message )
            tracker().failedTest( file, line, message );

        tracker().failedAssertThrowsNot( file, line, expression );
        TS_ABORT();
    }
CXXTEST_NS_END

#endif // __cxxtest__TestSuite_cpp__
