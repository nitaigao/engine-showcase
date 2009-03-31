#ifndef __cxxtest__GlobalFixture_h__
#define __cxxtest__GlobalFixture_h__

#include <cxxtest/LinkedList.h>
#include <cxxtest/Flags.h>

CXXTEST_NS_START
    class CXXTEST_API_DECL0 GlobalFixture : public Link
    {
    public:
        virtual bool setUpWorld();
        virtual bool tearDownWorld();
        virtual bool setUp();
        virtual bool tearDown();

        GlobalFixture();
        ~GlobalFixture();

        static GlobalFixture *firstGlobalFixture();
        static GlobalFixture *lastGlobalFixture();
        GlobalFixture *nextGlobalFixture();
        GlobalFixture *prevGlobalFixture();

    private:
        static List _list;
    };
CXXTEST_NS_END

#endif // __cxxtest__GlobalFixture_h__

