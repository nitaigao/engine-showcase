#ifndef __cxxtest_StdValueTraits_h__
#define __cxxtest_StdValueTraits_h__

//
// This file defines ValueTraits for CXXTEST_STD_NS:: stuff.
// It is #included by <cxxtest/ValueTraits.h> if you
// define CXXTEST_HAVE_STD
//

#include <cxxtest/ValueTraits.h>
#include <cxxtest/StdHeaders.h>

#ifdef _CXXTEST_OLD_STD
#   define CXXTEST_STD(x) x
#else // !_CXXTEST_OLD_STD
#   define CXXTEST_STD(x) CXXTEST_STD_NS::x
#endif // _CXXTEST_OLD_STD

#ifndef CXXTEST_USER_VALUE_TRAITS

CXXTEST_NS_START
    //
    // NOTE: This should have been
    // template<class Char, class Traits, class Allocator>
    // class ValueTraits< CXXTEST_STD_NS::basic_string<Char, Traits, Allocator> > {};
    // But MSVC doesn't support it (yet).
    //

    //
    // If we have CXXTEST_STD_NS::string, we might as well use it
    //
    class StdTraitsBase
    {
    public:
        StdTraitsBase &operator<<( const CXXTEST_STL::string &s ) { _s += s; return *this; }
        const char *asString() const { return _s.c_str(); }

    private:
        CXXTEST_STL::string _s;
    };

    //
    // CXXTEST_STD_NS::string
    //
    CXXTEST_TEMPLATE_INSTANTIATION
    class ValueTraits<const CXXTEST_STL::string> : public StdTraitsBase
    {
    public:
        ValueTraits( const CXXTEST_STL::string &s )
        {
            *this << "\"";
            for ( unsigned i = 0; i < s.length(); ++ i ) {
                char c[sizeof("\\xXX")];
                charToString( s[i], c );
                *this << c;
            }
            *this << "\"";
        }
    };

    CXXTEST_COPY_CONST_TRAITS( CXXTEST_STL::string);

#ifndef _CXXTEST_OLD_STD
    //
    // CXXTEST_STD_NS::wstring
    //
    CXXTEST_TEMPLATE_INSTANTIATION
    class ValueTraits<const CXXTEST_STL::basic_string<wchar_t> > : public StdTraitsBase
    {
    public:
        ValueTraits( const CXXTEST_STL::basic_string<wchar_t> &s )
        {
            *this << "L\"";
            for ( unsigned i = 0; i < s.length(); ++ i ) {
                char c[sizeof("\\x12345678")];
                charToString( (unsigned long)s[i], c );
                *this << c;
            }
            *this << "\"";
        }
    };

    CXXTEST_COPY_CONST_TRAITS( CXXTEST_STL::basic_string<wchar_t> );
#endif // _CXXTEST_OLD_STD

    //
    // Convert a range defined by iterators to a string
    // This is useful for almost all STL containers
    //
    template<class Stream, class Iterator>
    void dumpRange( Stream &s, Iterator first, Iterator last )
    {
        s << "{ ";
        while ( first != last ) {
            s << TS_AS_STRING(*first);
            ++ first;
            s << ((first == last) ? " }" : ", ");
        }
    }

#ifdef _CXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION
    //
    // CXXTEST_STD_NS::pair
    //
    template<class First, class Second>
    class ValueTraits< CXXTEST_STL::pair<First, Second> > : public StdTraitsBase
    {
    public:
        ValueTraits( const CXXTEST_STL::pair<First, Second> &p )
        {
            *this << "<" << TS_AS_STRING( p.first ) << ", " << TS_AS_STRING( p.second ) << ">";
        }
    };

    //
    // CXXTEST_STD_NS::vector
    //
    template<class Element>
    class ValueTraits< CXXTEST_STL::vector<Element> > : public StdTraitsBase
    {
    public:
        ValueTraits( const CXXTEST_STL::vector<Element> &v )
        {
            dumpRange( *this, v.begin(), v.end() );
        }
    };

    //
    // CXXTEST_STD_NS::list
    //
    template<class Element>
    class ValueTraits< CXXTEST_STL::list<Element> > : public StdTraitsBase
    {
    public:
        ValueTraits( const CXXTEST_STL::list<Element> &l )
        {
            dumpRange( *this, l.begin(), l.end() );
        }
    };

    //
    // CXXTEST_STD_NS::set
    //
    template<class Element>
    class ValueTraits< CXXTEST_STL::set<Element> > : public StdTraitsBase
    {
    public:
        ValueTraits( const CXXTEST_STL::set<Element> &s )
        {
            dumpRange( *this, s.begin(), s.end() );
        }
    };

    //
    // CXXTEST_STD_NS::map
    //
    template<class Key, class Value>
    class ValueTraits< CXXTEST_STL::map<Key, Value> > : public StdTraitsBase
    {
    public:
        ValueTraits( const CXXTEST_STL::map<Key, Value> &m )
        {
            dumpRange( *this, m.begin(), m.end() );
        }
    };

#ifndef CXXTEST_USE_MINI_STL
    //
    // CXXTEST_STD_NS::deque
    //
    template<class Element>
    class ValueTraits< CXXTEST_STL::deque<Element> > : public StdTraitsBase
    {
    public:
        ValueTraits( const CXXTEST_STL::deque<Element> &d )
        {
            dumpRange( *this, d.begin(), d.end() );
        }
    };
#endif

#ifndef CXXTEST_USE_MINI_STL
    //
    // CXXTEST_STD_NS::multiset
    //
    template<class Element>
    class ValueTraits< CXXTEST_STL::multiset<Element> > : public StdTraitsBase
    {
    public:
        ValueTraits( const CXXTEST_STL::multiset<Element> &ms )
        {
            dumpRange( *this, ms.begin(), ms.end() );
        }
    };
#endif

    //
    // CXXTEST_STD_NS::multimap
    //
    template<class Key, class Value>
    class ValueTraits< CXXTEST_STL::multimap<Key, Value> > : public StdTraitsBase
    {
    public:
        ValueTraits( const CXXTEST_STL::multimap<Key, Value> &mm )
        {
            dumpRange( *this, mm.begin(), mm.end() );
        }
    };

    //
    // CXXTEST_STD_NS::complex
    //
    template<class Number>
    class ValueTraits< CXXTEST_STD(complex)<Number> > : public StdTraitsBase
    {
    public:
        ValueTraits( const CXXTEST_STD(complex)<Number> &c )
        {
            if ( !c.imag() )
                *this << TS_AS_STRING(c.real());
            else if ( !c.real() )
                *this << "(" << TS_AS_STRING(c.imag()) << " * i)";
            else
                *this << "(" << TS_AS_STRING(c.real()) << " + " << TS_AS_STRING(c.imag()) << " * i)";
        }
    };
#endif // _CXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION
CXXTEST_NS_END

#endif // CXXTEST_USER_VALUE_TRAITS

#endif // __cxxtest_StdValueTraits_h__
