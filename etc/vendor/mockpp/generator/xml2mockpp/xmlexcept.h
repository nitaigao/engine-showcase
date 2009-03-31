/***************************************************************************
                      except.h  -  exception handling
                             -------------------
    begin                : Sun May 20 2007
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp@ewald-arnold.de

    $Id: xmlexcept.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_EXCEPT_H
#define MOCKPP_EXCEPT_H

#include <string>
#include <exception>


/** Base class for exceptions.
  */
class Exception : public std::exception
{
 public:

 /** Constructs an exception.
   * @param fc number representing the error class
   * @param s  the reason for the exception
   */
   Exception(int fc, const std::string &s);

 /** Destroys the exception.
   */
   virtual ~Exception()  throw();

 /** Gets the reason for the exception.
   * @return   the reason
   */
   virtual std::string why() const;

 /** Gets a number representing the error.
   * @return   the fault code
   */
   virtual int getFaultCode() const;

  /** Returns the exception description.
    * Provided for compatibility with std::exception. If Unicode is
    * activated you might get corrupt information if you are beyond
    * latin1 characters.
    * @return   description as pointer to a latin1 string.
    */
    virtual const char *what() const throw();

 private:

  std::string reason;
  int         faultcode;
};


/** Exception due to a faulty xml structure.
  */
class XmlException : public Exception
{
 public:

 /** Constructs an xml exception.
   * @param fc   number representing the error class
   * @param s    the reason for the exception
   * @param l    the faulty line (relative to the body start!)
   * @param err  a human readable error string
   */
   XmlException(int fc, const std::string &s, int l, const std::string &err);

 /** Destroys the exception.
   */
   virtual ~XmlException()  throw();

 /** Gets the line number of the error.
   * @return the line number
   */
   int getErrorLine() const;

 /** Gets the error string.
   * @return the string
   */
   std::string getErrorString() const;

 private:

  int          line;
  std::string  xmlerror;
};


#endif // MOCKPP_EXCEPT_H

