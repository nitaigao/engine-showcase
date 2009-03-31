/***************************************************************************
                       except.h  -  exception handling
                             -------------------
    begin                : Sun May 20 2007
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp@ewald-arnold.de

    $Id: xmlexcept.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#include "xmlexcept.h"


Exception::Exception(int fc, const std::string &s)
  : std::exception()
  , reason(s)
  , faultcode(fc)
{
}


Exception::~Exception() throw()
{
}


std::string Exception::why() const
{
  return reason;
}


const char * Exception::what() const throw()
{
  return reason.c_str();
}


int Exception::getFaultCode() const
{
  return faultcode;
}


/////////////////////////////////////////////////////////////////////////


XmlException::XmlException(int fc, const std::string &s, int l,
                           const std::string &err)
  : Exception(fc, s), line(l), xmlerror(err)
{
}


XmlException::~XmlException() throw()
{
}


int XmlException::getErrorLine() const
{
  return line;
}


std::string XmlException::getErrorString() const
{
  return xmlerror;
}

