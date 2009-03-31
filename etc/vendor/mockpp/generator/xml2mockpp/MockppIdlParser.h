/***************************************************************************
             MockppIdlParser.h  -  parse mockpp idl files
                             -------------------
    begin                : Sun May 20 2007
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp@ewald-arnold.de

    $Id: MockppIdlParser.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_IDL_PARSER_H
#define MOCKPP_IDL_PARSER_H

#include "xmlparser.h"
#include "MockppIdlClass.h"


/** An xml parser for mockpp idl files
  */
class MockppIdlParser : public XmlParser
{
 public:

 /** Constructs a parser.
   */
   MockppIdlParser();

 /** Destroys the parser.
   */
   virtual ~MockppIdlParser();

   unsigned numClasses() const;

   MockppIdlClass getClass(unsigned i) const;

 protected:

  enum IdlState
  {
    eIdlParserFirst = XmlParserBase::eXmlParserLast,
    eMockppIdl,
    eSource,
    eInclude,
    eClass,
    eName,
    eLinkScope,
    eSuper,
    eMethod,
    eConstructor,
    eType,
    eArg,
    eIdlParserLast
  };

 /** Parses the current opening XML tag.
   * Used ONLY internally as callback from expat.
   * @param  name  the name of the current tag
   * @param  pointer to the current attributs (unused in XML-RPC)
   */
   virtual void startElement(const XML_Char *name,
                             const XML_Char **atts);

 /** Parses the current closing XML tag.
   * Used ONLY internally as callback from expat.
   * @param  name  the name of the current tag
   * @param  pointer to the current attributs (unused in XML-RPC)
   */
   virtual void endElement(const XML_Char* name);

 /** Tests if the current opening tag is to be parsed by this
   * inheritance level or by the parent.
   * Used ONLY internally.
   * @param  name  the name of the current tag
   * @param  atts  pointer to the current attributs (unused in XML-RPC)
   */
   bool testStartElement(const XML_Char *name,
                         const XML_Char **atts);

 /** Tests if the current closing tag is to be parsed by this
   * inheritance level or by the parent.
   * Used ONLY internally.
   * @param  name  the name of the current tag
   * @param  pointer to the current attributs
   */
   bool testEndElement(const XML_Char *name);

 private:

   bool            in_arg;
   bool            in_method;

   MockppIdlClass  theClass;
   Method          method;
   Argument        arg;
   Type            type;

   std::vector<MockppIdlClass>  classList;
};


#endif // MOCKPP_IDL_PARSER_H
