/***************************************************************************
       classes_ABCDE - provide some classes for testing purposes
                             -------------------
    begin                : Sat Sep 25 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

   $Id: classes_ABCDE.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 ***************************************************************************/

#ifndef MOCKPP_CLASSES_ABCDE_H
#define MOCKPP_CLASSES_ABCDE_H


class CA
{
    char c;

  public:

    CA(char ic) : c( ic ) {}

    CA() : c( 'A' ) {}

    bool operator==( const CA &o ) const
    {
      return c == o.c;
    }

    bool operator<( const CA &/*o*/ ) const
    {
      return true;
    }
};

MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CA &/*o*/);

MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CA */*o*/);


class CB
{
    char c;

  public:

    CB(char ic) : c( ic ) {}

    CB() : c( 'B' ) {}

    bool operator==( const CB &o ) const
    {
      return c == o.c;
    }

    bool operator<( const CB &/*o*/ ) const
    {
      return true;
    }
};

MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CB &/*o*/);

MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CB */*o*/);


class CC
{
    char c;

  public:

    CC(char ic) : c( ic ) {}

    CC() : c( 'C' ) {}

    bool operator==( const CC &o ) const
    {
      return c == o.c;
    }

    bool operator<( const CC &/*o*/ ) const
    {
      return true;
    }
};

MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CC &/*o*/);

MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CC */*o*/);


class CD
{
    char c;

  public:

    CD(char ic) : c( ic ) {}

    CD() : c( 'D' ) {}

    bool operator==( const CD &o ) const
    {
      return c == o.c;
    }

    bool operator<( const CD &/*o*/ ) const
    {
      return true;
    }
};

MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CD &/*o*/);

MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CD */*o*/);


class CE
{
    char c;

  public:

    CE(char ic) : c( ic ) {}

    CE() : c( 'E' ) {}

    bool operator==( const CE &o ) const
    {
      return c == o.c;
    }

    bool operator<( const CE &/*o*/ ) const
    {
      return true;
    }
};

MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CE &/*o*/);

MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CE */*o*/);


class CF
{
    char c;

  public:

    CF(char ic) : c( ic ) {}

    CF() : c( 'F' ) {}

    bool operator==( const CF &o ) const
    {
      return c == o.c;
    }
};

MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CF &/*o*/);

MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CF */*o*/);


#endif // MOCKPP_CLASSES_ABCDE_H
