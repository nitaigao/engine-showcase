/***************************************************************************
      classes_ABCDE - provide some classes for testing purposes
                             -------------------
    begin                : Sat Sep 25 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

   $Id: classes_ABCDE.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

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


#include <mockpp/mockpp.h> // always first

#include "classes_ABCDE.h"

#include <mockpp/compat/Formatter.h>


MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CA &/*o*/)
{
  return ::operator<< (formatter, MOCKPP_PCHAR("CA&"));
}


MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CA */*o*/)
{
  return ::operator<< (formatter, MOCKPP_PCHAR("CA*"));
}


MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CB &/*o*/)
{
  return ::operator<< (formatter, MOCKPP_PCHAR("CB&"));
}


MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CB */*o*/)
{
  return ::operator<< (formatter, MOCKPP_PCHAR("CB*"));
}


MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CC &/*o*/)
{
  return ::operator<< (formatter, MOCKPP_PCHAR("CC&"));
}


MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CC */*o*/)
{
  return ::operator<< (formatter, MOCKPP_PCHAR("CC*"));
}


MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CD &/*o*/)
{
  return ::operator<< (formatter, MOCKPP_PCHAR("CD&"));
}


MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CD*/*o*/)
{
  return ::operator<< (formatter, MOCKPP_PCHAR("CD*"));
}


MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CE &/*o*/)
{
  return ::operator<< (formatter, MOCKPP_PCHAR("CE&"));
}


MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CE* /*o*/)
{
  return ::operator<< (formatter, MOCKPP_PCHAR("CE*"));
}


MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CF &/*o*/)
{
  return ::operator<< (formatter, MOCKPP_PCHAR("CF&"));
}


MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const CF */*o*/)
{
  return ::operator<< (formatter, MOCKPP_PCHAR("CF*"));
}

