
/***************************************************************************
                        xmlparse.h  -  parse xml files
                             -------------------
    begin                : Sun May 20 2007
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp@ewald-arnold.de

    $Id: xmlparser.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_XMLPARSE_H
#define MOCKPP_XMLPARSE_H

#include "expatwrap.h"


/** Base class for XML parsing with expat.
  */
class XmlParser : public ExpatWrapper
{
 public:

 /** Constructs a parser.
   */
   XmlParser();

 /** Destructs a parser.
   */
   virtual ~XmlParser();

 protected:

 /** Checks if current and expected tag are the same.
   * @param  current   the name of the current tag
   * @param  expected  name of the expected tag
   */
   void assertEndElement(const char *current, const char *expected);

 /** Tests if the current opening tag is to be parsed by this
   * inheritance level or by the parent.
   * Used ONLY internally.
   * @param  name  the name of the current tag
   * @param  atts  pointer to the current attributes (unused in XML-RPC)
   * @return true: element has been handled
   */
   bool testStartElement(const XML_Char *name, const XML_Char **atts);

 /** Tests if the current closing tag is to be parsed by this
   * inheritance level or by the parent.
   * Used ONLY internally.
   * @param  name  the name of the current tag
   * @return true: element has been handled
   */
   bool testEndElement(const XML_Char *name);

 private:

 /** Parses the content of the current xml element.
   * Used ONLY internally as callback from expat.
   * The text from expat is encoded in UTF8.
   * @param  s   the current chunk of text
   * @param  len valid len.
   */
   virtual void charData(const XML_Char *s, int len);
};


#endif // MOCKPP_XMLPARSE_H
