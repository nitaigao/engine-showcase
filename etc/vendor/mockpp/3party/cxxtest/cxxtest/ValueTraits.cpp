#ifndef __cxxtest__ValueTraits_cpp__
#define __cxxtest__ValueTraits_cpp__

#define CXXTEST_NEED_EXPORTS
#include <cxxtest/Flags.h>

#include <cxxtest/ValueTraits.h>

CXXTEST_NS_START
    //
    // Non-inline functions from ValueTraits.h
    //

    CXXTEST_API_IMPL(char) digitToChar( unsigned digit )
    {
        if ( digit < 10 )
            return (char)('0' + digit);
        if ( digit <= 10 + 'Z' - 'A' )
            return (char)('A' + digit - 10);
        return '?';
    }

    CXXTEST_API_IMPL(const char *) byteToHex( unsigned char byte )
    {
        static char asHex[3];
        asHex[0] = digitToChar( byte >> 4 );
        asHex[1] = digitToChar( byte & 0x0F );
        asHex[2] = '\0';
        return asHex;
    }

    CXXTEST_API_IMPL(char *) copyString( char *dst, const char *src )
    {
        while ( (*dst = *src) != '\0' ) {
            ++ dst;
            ++ src;
        }
        return dst;
    }

    CXXTEST_API_IMPL(bool) stringsEqual( const char *s1, const char *s2 )
    {
        char c;
        while ( (c = *s1++) == *s2++ )
            if ( c == '\0' )
                return true;
        return false;
    }

    CXXTEST_API_IMPL(char *) charToString( unsigned long c, char *s )
    {
        switch( c ) {
        case '\\': return copyString( s, "\\\\" );
        case '\"': return copyString( s, "\\\"" );
        case '\'': return copyString( s, "\\\'" );
        case '\0': return copyString( s, "\\0" );
        case '\a': return copyString( s, "\\a" );
        case '\b': return copyString( s, "\\b" );
        case '\n': return copyString( s, "\\n" );
        case '\r': return copyString( s, "\\r" );
        case '\t': return copyString( s, "\\t" );
        }
        if ( c >= 32 && c <= 127 ) {
            s[0] = (char)c;
            s[1] = '\0';
            return s + 1;
        }
        else {
            s[0] = '\\';
            s[1] = 'x';
            if ( c < 0x10 ) {
                s[2] = '0';
                ++ s;
            }
            return numberToString( c, s + 2, 16UL );
        }
    }

    CXXTEST_API_IMPL(char *) charToString( char c, char *s )
    {
        return charToString( (unsigned long)(unsigned char)c, s );
    }

    CXXTEST_API_IMPL(char *) bytesToString( const unsigned char *bytes, unsigned numBytes, unsigned maxBytes, char *s )
    {
        bool truncate = (numBytes > maxBytes);
        if ( truncate )
            numBytes = maxBytes;

        s = copyString( s, "{ " );
        for ( unsigned i = 0; i < numBytes; ++ i, ++ bytes )
            s = copyString( copyString( s, byteToHex( *bytes ) ), " " );
        if ( truncate )
            s = copyString( s, "..." );
        return copyString( s, " }" );
    }

#ifndef CXXTEST_USER_VALUE_TRAITS
    CXXTEST_API_IMPL(unsigned) ValueTraits<const double>::requiredDigitsOnLeft( double t )
    {
        unsigned digits = 1;
        for ( t = (t < 0.0) ? -t : t; t > 1.0; t /= BASE )
            ++ digits;
        return digits;
    }

    CXXTEST_API_IMPL(char *) ValueTraits<const double>::doNegative( double &t )
    {
        if ( t >= 0 )
            return _asString;
        _asString[0] = '-';
        t = -t;
        return _asString + 1;
    }

    CXXTEST_API_IMPL(void) ValueTraits<const double>::hugeNumber( double t )
    {
        char *s = doNegative( t );
        s = doubleToString( t, s, 0, 1 );
        s = copyString( s, "." );
        s = doubleToString( t, s, 1, DIGITS_ON_RIGHT );
        s = copyString( s, "E" );
        s = numberToString( requiredDigitsOnLeft( t ) - 1, s );
    }

    CXXTEST_API_IMPL(void) ValueTraits<const double>::normalNumber( double t )
    {
        char *s = doNegative( t );
        s = doubleToString( t, s );
        s = copyString( s, "." );
        for ( unsigned i = 0; i < DIGITS_ON_RIGHT; ++ i )
            s = numberToString( (unsigned)(t *= BASE) % BASE, s );
    }

    CXXTEST_API_IMPL(char *) ValueTraits<const double>::doubleToString( double t, char *s, unsigned skip, unsigned max )
    {
        return numberToString<double>( t, s, BASE, skip, max );
    }
#endif // !CXXTEST_USER_VALUE_TRAITS
CXXTEST_NS_END

#endif // __cxxtest__ValueTraits_cpp__
