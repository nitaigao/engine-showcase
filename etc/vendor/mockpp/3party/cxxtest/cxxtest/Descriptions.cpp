#ifndef __cxxtest__Descriptions_cpp__
#define __cxxtest__Descriptions_cpp__

#define CXXTEST_NEED_EXPORTS
#include <cxxtest/Flags.h>

#include <cxxtest/Descriptions.h>
#include <cxxtest/ValueTraits.h>

CXXTEST_NS_START
    CXXTEST_API_IMPL0 TestDescription::~TestDescription() {}
    CXXTEST_API_IMPL0 SuiteDescription::~SuiteDescription() {}
    CXXTEST_API_IMPL0 WorldDescription::~WorldDescription() {}

    //
    // Convert total tests to string
    //
#ifndef _CXXTEST_FACTOR
    char *WorldDescription::strTotalTests( char *s ) const
    {
        numberToString( numTotalTests(), s );
        return s;
    }
#else // _CXXTEST_FACTOR
    char *WorldDescription::strTotalTests( char *s ) const
    {
        char *p = numberToString( numTotalTests(), s );

        if ( numTotalTests() <= 1 )
            return s;

        unsigned n = numTotalTests();
        unsigned numFactors = 0;

        for ( unsigned factor = 2; (factor * factor) <= n; factor += (factor == 2) ? 1 : 2 ) {
            unsigned power;

            for ( power = 0; (n % factor) == 0; n /= factor )
                ++ power;

            if ( !power )
                continue;

            p = numberToString( factor, copyString( p, (numFactors == 0) ? " = " : " * " ) );
            if ( power > 1 )
                p = numberToString( power, copyString( p, "^" ) );
            ++ numFactors;
        }

        if ( n > 1 ) {
            if ( !numFactors )
                copyString( p, tracker().failedTests() ? " :(" : tracker().warnings() ? " :|" : " :)" );
            else
                numberToString( n, copyString( p, " * " ) );
        }
        return s;
    }
#endif // _CXXTEST_FACTOR

CXXTEST_NS_END

#endif // __cxxtest__Descriptions_cpp__

