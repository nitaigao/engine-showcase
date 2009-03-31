#ifndef MSTRING_H
#define MSTRING_H

/** Quick-hack to port QT-based dcopidl to standard basic_string.
  * Copyright (c) 2007 Ewald Arnold <mockpp@ewald-arnold.de>
  */

#include <string>

class MString : public std::string
{
  public:

    MString ()
        : std::string()
    {}


    MString ( const char *cstr )
        : std::string ( cstr )
    {}


    MString ( const char *cstr, int len )
        : std::string ( cstr, len )
    {}


    MString ( char chr )
        : std::string ( 1, chr )
    {}


    MString ( const MString &str )
        : std::string ( str )
    {}


    MString ( const std::string &str )
        : std::string ( str )
    {}


    const char *latin1() const
    {
      return c_str();
    }


    std::size_t findRev ( const MString &str, int pos )
    {
      return rfind ( str, this->length() +pos );
    }


    MString left ( unsigned cnt )
    {
      return substr ( 0, cnt );
    }


    MString arg ( const MString& a, int fieldwidth = 0 ) const
    {
      int pos, len;
      MString r = *this;

      if ( !findArg ( pos, len ) )
        return a;

      r.replace ( pos, len, a );
      if ( fieldwidth < 0 )
      {

        MString s;
        while ( ( unsigned int )-fieldwidth > a.length() )
        {
          s += ' ';
          fieldwidth++;
        }
        r.insert ( pos + a.length(), s );
      }
      else if ( fieldwidth )
      {
        MString s;
        while ( ( unsigned int ) fieldwidth > a.length() )
        {
          s += ' ';
          fieldwidth--;
        }
        r.insert ( pos, s );
      }

      return r;
    }

  private:

    bool findArg ( int& pos, int& len ) const
    {
      char lowest=0;
      for ( unsigned int i=0; i<length(); i++ )
      {
        if ( at ( i ) == '%' && i+1<length() )
        {
          char dig = at ( i+1 );
          if ( dig >= '0' && dig <= '9' )
          {
            if ( !lowest || dig < lowest )
            {
              lowest = dig;
              pos = i;
              len = 2;
            }
          }
        }
      }
      return lowest != 0;
    }
};


#endif // MSTRING_H

