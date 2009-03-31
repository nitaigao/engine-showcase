#ifndef __cxxtest__StdioPrinter_h__
#define __cxxtest__StdioPrinter_h__

//
// The StdioPrinter is an StdioFilePrinter which defaults to stdout.
// This should have been called StdOutPrinter or something, but the name
// has been historically used.
//

#include <cxxtest/StdioFilePrinter.h>

CXXTEST_NS_START
    class StdioPrinter : public StdioFilePrinter
    {
    public:
        StdioPrinter( FILE *o = stdout, const char *preLine = ":", const char *postLine = "" ) :
            StdioFilePrinter( o, preLine, postLine ) {}
    };
CXXTEST_NS_END

#endif // __cxxtest__StdioPrinter_h__
