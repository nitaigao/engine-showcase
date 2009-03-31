#ifndef __cxxtest__DummyDescriptions_h__
#define __cxxtest__DummyDescriptions_h__

//
// DummyTestDescription, DummySuiteDescription and DummyWorldDescription
//

#include <cxxtest/Descriptions.h>
#include <cxxtest/Flags.h>

CXXTEST_NS_START
    class CXXTEST_API_DECL0 DummyTestDescription : public TestDescription
    {
    public:
        DummyTestDescription();

        const char *file() const;
        unsigned line() const;
        const char *testName() const;
        const char *suiteName() const;
        bool setUp();
        void run();
        bool tearDown();

        TestDescription *next();
        const TestDescription *next() const;
    };

    class CXXTEST_API_DECL0 DummySuiteDescription : public SuiteDescription
    {
    public:
        DummySuiteDescription();

        const char *file() const;
        unsigned line() const;
        const char *suiteName() const;
        TestSuite *suite() const;
        unsigned numTests() const;
        const TestDescription &testDescription( unsigned ) const;
        SuiteDescription *next();
        TestDescription *firstTest();
        const SuiteDescription *next() const;
        const TestDescription *firstTest() const;
        void activateAllTests();
        bool leaveOnly( const char * /*testName*/ );

        bool setUp();
        bool tearDown();

    private:
        DummyTestDescription _test;
    };

    class CXXTEST_API_DECL0 DummyWorldDescription : public WorldDescription
    {
    public:
        DummyWorldDescription();

        unsigned numSuites( void ) const;
        unsigned numTotalTests( void ) const;
        const SuiteDescription &suiteDescription( unsigned ) const;
        SuiteDescription *firstSuite();
        const SuiteDescription *firstSuite() const;
        void activateAllTests();
        bool leaveOnly( const char * /*suiteName*/, const char * /*testName*/ = 0 );

        bool setUp();
        bool tearDown();

    private:
        DummySuiteDescription _suite;
    };
CXXTEST_NS_END

#endif // __cxxtest__DummyDescriptions_h__

