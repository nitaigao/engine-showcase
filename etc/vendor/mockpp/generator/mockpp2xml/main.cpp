/*****************************************************************
Copyright (c) 1999 Torben Weis <weis@kde.org>
Copyright (c) 2000 Matthias Ettrich <ettrich@kde.org>
Copyright (c) 2007 Ewald Arnold <mockpp@ewald-arnold.de>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modifyo, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

******************************************************************/

#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>

#include <stdio.h>

#ifdef _MSC_VER
#  include <mockpp/mockpp_config-msvc_71.h>

#elif defined(__BORLANDC__)
# include <mockpp/mockpp_config-bcb5.h>

#elif defined(__CYGWIN__)
# ifdef HAVE_CONFIG_H
#  include <config.h>
# else
#  include <mockpp/mockpp_config.h>
# endif
#else

# ifdef HAVE_CONFIG_H
#  include <config.h>
# else
#  include <mockpp/mockpp_config.h>
# endif
#endif

/*
    QByteArray arr = file.readAll();
    uint len = arr.size();
    uint j = 1;
    for (uint i = 1; i<len; i++, j++) {
        if (arr[ i-1 ] == '\r' && ((i+1)==len || arr[ i ] != '\n')) {
            // change single \r's (Mac OS line endings) to \n
            arr[ j-1 ] = '\n';
            if ((i+1)==len) //special case: cut last character
                j--;
        }
        else if (arr[ i-1 ] == '\r' && arr[ i ] == '\n') {
            // change \r\n's (win32 line endings) to \n
            arr[ j-1 ] = '\n';
            i++; //skip \n
        }
        else if (i!=j) {
            arr[ j-1 ] = arr[ i-1 ];
        }
    }
    len = j;
    arr.resize( len + 1 );
    arr[ len ] = 0;
*/

void dcopidlParse( const char *_code );

int idl_line_no;

#if YYDEBUG
extern int yydebug;
#endif

int main( int argc, char** argv )
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage : %s input_file\n", argv[0]);
        return -1;
    }

    std::ifstream file( argv[1] );
    if (!file)
    {
        fprintf(stderr, "Can't open input file %s\n", argv[1]);
        return -1;
    }

    std::ostringstream arr;
#if 1 // TODO OpenBSD
    file.unsetf(std::ios_base::skipws);
    arr.unsetf(std::ios_base::skipws);
#endif

    std::istream_iterator<char> iit(file), eof_it;
    std::ostream_iterator<char> oit(arr);
    std::copy(iit, eof_it, oit);
    arr << '\0';

/*
    QByteArray arr = file.readAll();
    uint len = arr.size();
    uint j = 1;
    for (uint i = 1; i<len; i++, j++) {
        if (arr[ i-1 ] == '\r' && ((i+1)==len || arr[ i ] != '\n')) {
            // change single \r's (Mac OS line endings) to \n
            arr[ j-1 ] = '\n';
            if ((i+1)==len) //special case: cut last character
                j--;
        }
        else if (arr[ i-1 ] == '\r' && arr[ i ] == '\n') {
            // change \r\n's (win32 line endings) to \n
            arr[ j-1 ] = '\n';
            i++; //skip \n
        }
        else if (i!=j) {
            arr[ j-1 ] = arr[ i-1 ];
        }
    }
    len = j;
    arr.resize( len + 1 );
    arr[ len ] = 0;
*/


#if YYDEBUG
    char *debug = getenv("DEBUG");
    if (debug)
	yydebug = 1;
#endif
    idl_line_no = 1;

    std::string filename = argv[1];
/*
    std::size_t pos;
    if ((pos = filename.rfind("/")) != std::string::npos)
      filename = filename.substr(pos+1);
*/
    printf("<!DOCTYPE MOCKPP-IDL>\n<MOCKPP-IDL version='%s'>\n", MOCKPP_VERSION);
    printf("<SOURCE>%s</SOURCE>\n", filename.c_str() );
    dcopidlParse(arr.str().data() );
    printf("</MOCKPP-IDL>\n");

    return 0;
}

