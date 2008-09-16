/** @file
    @brief    Base class for MockObject exceptions

  $Id: Exception.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Tue Dec 31 2002
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

#ifndef MOCKPP_EXCEPTION
#define MOCKPP_EXCEPTION

#include <mockpp/mockpp.h> // always first

#ifdef MOCKPP_USE_STD_EXCEPTION
#include <exception>
#endif


MOCKPP_NS_START


/** Base class for all mockpp exceptions. The main purpose of such exceptions
  * is to provide information about the location and the cause of failed actions.
  */
class MOCKPP_API_DECL0 Exception
#ifdef MOCKPP_USE_STD_EXCEPTION
   : public MOCKPP_STD_NS::exception
#endif
{
  public:

  /** Constructs an exception.
    * @param srcline    the line in the sourcefile
    * @param srcfile    the name of the sourcefile
    * @param message    a human readable description about the cause
    */
    Exception (unsigned srcline, const char* srcfile, const String &message);

  /** Destroys the exception.
    * Mainly here to enforce virtual destructors.
    */
    virtual ~Exception() throw();

  /** Returns the exception description.
    * @return   descriptin
    */
    virtual String getMessage() const;

  /** Returns the source line where the exception occured.
    * @return   source line
    */
    unsigned getSrcLine() const;

  /** Returns the source file name where the exception occured.
    * @return   source file name
    */
    String getSrcFile() const;

#ifdef MOCKPP_USE_STD_EXCEPTION
  /** Returns the exception description.
    * Provided for compatibility with std::exception. If Unicode is
    * activated you might get corrupt information if you are beyond
    * latin1 characters.
    * @return   description as pointer to a latin1 string.
    */
    virtual const char *what() const throw();
#endif

  private:

    String       message;
    unsigned     srcline;
    String       srcfile;
#ifdef MOCKPP_USE_STD_EXCEPTION
    mutable MOCKPP_STL::string  what_helper;
#endif
};


MOCKPP_NS_END


#endif // MOCKPP_ASSERTIONFAILED_H

