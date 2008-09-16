/** @file
    @brief    Signal failed assertions

  $Id: AssertionFailedError.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Mon Dec 23 2002
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

#ifndef MOCKPP_ASSERTIONFAILEDERROR_H
#define MOCKPP_ASSERTIONFAILEDERROR_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/compat/Exception.h>


MOCKPP_NS_START


/**
  * Thrown when an assertion failed.
  */
class MOCKPP_API_DECL0 AssertionFailedError : public Exception
{
  public:

  /** Constructs an exception.
    * @param srcline    the line in the sourcefile
    * @param srcfile    the name of the sourcefile
    * @param message    a human readable description about the cause
    */
    AssertionFailedError (unsigned srcline, const char* srcfile,
                          const String &message);

  /** Destroy the exception.
    */
    virtual ~AssertionFailedError() throw();
};


/** Throws an AssertionFailedError.
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  * @param message    a human readable description about the cause
  */
MOCKPP_API_DECL(void) assertionFailed(unsigned srcline, const char* srcfile,
                                 const String &message);


/** Throws an AssertionFailedError.
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  * @param message    a human readable description about the cause
  */
MOCKPP_API_DECL(void) assertionFailed(unsigned srcline, const String& srcfile,
                                 const String &message);


MOCKPP_NS_END


#endif // MOCKPP_ASSERTIONFAILED_H

