/***************************************************************************
                             IncCounter.cpp
                                 -
                    Increments a counter value for testing purposes
                             -------------------
    begin                : Thu Oct 2 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: IncCounter.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Oct 3 2005
   copyright            : (C) 2002-2008 by Ewald Arnold
   email                : mockpp at ewald-arnold dot de

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

 */

#ifndef MOCKPP_IncCounter_H
#define MOCKPP_IncCounter_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/stub/TypelessStub.h>


template <typename T>
class IncCounter : public MOCKPP_NS::TypelessStub<void>
{
  public:

    IncCounter( T &arg )
      : back_ref(arg)
    {
    }

    virtual ~IncCounter()
    {}

    virtual void typelessInvoke()
    {
      back_ref++;
    }

    virtual MOCKPP_NS::String describeTo( MOCKPP_NS::String &buffer ) const
    {
      MOCKPP_NS::String fmt = MOCKPP_PCHAR("inc counter %1");
      buffer += fmt;
      return buffer;
    }

  private:

    T&  back_ref;
};


#endif // MOCKPP_IncCounter_H

