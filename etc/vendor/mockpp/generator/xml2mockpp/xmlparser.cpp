/**************************************************************************
                       xmlparse.cpp  -  parse xml files
                             -------------------
    begin                : Sun May 20 2007
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp@ewald-arnold.de

    $Id: xmlparser.cpp 1442 2008-05-09 14:41:02Z ewald-arnold $

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

#include "xmlparser.h"
#include "xmlexcept.h"

#include <cstring>


XmlParser::XmlParser()
  : ExpatWrapper()
{
}


XmlParser::~XmlParser()
{
}


void XmlParser::charData(const XML_Char *s, int len)
{
  states.top()->appendCharData(s, len);
}


bool XmlParser::testStartElement(const XML_Char *name, const XML_Char ** /*atts*/)
{
  throw XmlException(NotWellformedError,
                     "Problem while parsing xml structure",
                     getCurrentLineNumber(),
                     std::string("unexpected opening tag: ") + name);
}


bool XmlParser::testEndElement(const XML_Char *name)
{
  throw XmlException(NotWellformedError,
                     "Problem while parsing xml structure",
                     getCurrentLineNumber(),
                     std::string("unexpected closing tag: ")  + name);
}


void XmlParser::assertEndElement(const char *current, const char *expected)
{
  if (0 != strcmp(current, expected))
    throw XmlException(NotWellformedError,
                       std::string("Unexpected xml tag: ")
                       + current
                       + std::string(", wanted: ")
                       + expected,
                       getCurrentLineNumber(),
                       std::string("Document not wellformed"));
}

