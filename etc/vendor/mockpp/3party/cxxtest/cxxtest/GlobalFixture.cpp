#ifndef __cxxtest__GlobalFixture_cpp__
#define __cxxtest__GlobalFixture_cpp__

#define CXXTEST_NEED_EXPORTS
#include <cxxtest/Flags.h>

#include <cxxtest/GlobalFixture.h>

CXXTEST_NS_START
List GlobalFixture::_list = { 0, 0 };

    bool GlobalFixture::setUpWorld() { return true; }
    bool GlobalFixture::tearDownWorld() { return true; }
    bool GlobalFixture::setUp() { return true; }
    bool GlobalFixture::tearDown() { return true; }

    GlobalFixture::GlobalFixture() { attach( _list ); }
    GlobalFixture::~GlobalFixture() { detach( _list ); }

    GlobalFixture *GlobalFixture::firstGlobalFixture() { return (GlobalFixture *)_list.head(); }
    GlobalFixture *GlobalFixture::lastGlobalFixture() { return (GlobalFixture *)_list.tail(); }
    GlobalFixture *GlobalFixture::nextGlobalFixture() { return (GlobalFixture *)next(); }
    GlobalFixture *GlobalFixture::prevGlobalFixture() { return (GlobalFixture *)prev(); }
CXXTEST_NS_END

#endif // __cxxtest__GlobalFixture_cpp__

