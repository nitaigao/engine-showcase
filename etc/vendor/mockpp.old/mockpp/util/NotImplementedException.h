/** @file
    @brief Exception for not implemented stuff

  $Id: NotImplementedException.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sat Dec 21 2002
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

 **/


#ifndef MOCKPP_NOTIMPLEMENTEDEXCEPTION_H
#define MOCKPP_NOTIMPLEMENTEDEXCEPTION_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/compat/AssertionFailedError.h>


MOCKPP_NS_START


/** Signal not-yet implemented methods.
  * @ingroup grp_helper
  */
class MOCKPP_API_DECL0 NotImplementedException : public AssertionFailedError
{
  public:
  /**
    * NotImplementedException constructor comment.
    * @param srcline    the line in the sourcefile
    * @param srcfile    the name of the sourcefile
    * @param message    a human readable description about the cause
    */
    NotImplementedException(unsigned srcline, const char* srcfile, const String &message);
};


MOCKPP_NS_END


#endif // MOCKPP_NOTIMPLEMENTEDEXCEPTION_H
