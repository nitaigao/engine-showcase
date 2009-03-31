/***************************************************************************
                     expatwrap.h  -  c++ wrapper for expat
                             -------------------
    begin                : Sun May 20 2007
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp@ewald-arnold.de

    $Id: expatwrap.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_EXPATWRAP_H
#define MOCKPP_EXPATWRAP_H

#include <map>

#include "xmlparse_base.h"


/** A wrapper for expat
  */
class ExpatWrapper : public XmlParserBase
{
 public:

   enum
   {
     NotWellformedError       = 1,
     InvalidCharacterError    = 2,
     UnsupportedEncodingError = 3
   };

    class AttributeList;

  /** Constructs an expat  parser.
    * @param createParser  create a new parser instance
    */
    ExpatWrapper(bool createParser=true);

  /** Destroys the parser.
    */
    virtual ~ExpatWrapper();

  /** Parse a pice of xml data.
    * @param buffer   pointer start of next data chunk
    * @param len      len of this chunk
    * @param isFinal  true: last call to parser
    * @return error condition, 0 = ok
    */
    virtual int parse(const char* buffer, int len, int isFinal);

  /** Gets the code for the current error.
    * @return error code
    */
    virtual unsigned getErrorCode() const;

  /** Gets the description for an error code
    * @param code  error code
    * @return  pointer to description
    */
    virtual std::string getErrorString(unsigned code) const;

  /** Gets the line number in the xml data
    * @return  line number
    */
    virtual int getCurrentLineNumber() const;

  /** Maps expat error codes to xml-rpc error codes.
    * @param  xpatcode   error code from expat
    * @return  the according xml-rpc error
    */
    virtual int mapToFaultCode(int xpatcode) const;

 protected:

  /** Gets the parser instance.
    */
    operator XML_Parser() const;

  /** C++ callback for an opening XML tag.
    * Used ONLY internally as callback from expat.
    * @param  name  the name of the current tag
    * @param  atts  pointer to the current attributs (unused in XML-RPC)
    */
    virtual void startElement(const XML_Char* name, const XML_Char** atts);

  /** C++ callback for a closing XML tag.
    * Used ONLY internally as callback from expat.
    * @param  name  the name of the current tag
    */
    virtual void endElement(const XML_Char *name);

  /** C++ callback for regular text from expat.
    * Used ONLY internally.
    * @param  s        starting buffer with more data
    * @param  len      lenth of buffer
    */
    virtual void charData(const XML_Char *s, int len);

  /** C-style callback for a closing XML tag from expat.
    * Used ONLY internally.
    * @param  userData pointer to the actual C++-object
    * @param  name     the name of the current tag
    * @param  atts     to the current attributs (unused in XML-RPC)
    */
    static void startElementCallback(void *userData, const XML_Char* name, const XML_Char** atts);

  /** C-style callback for an ending XML tag from expat.
    * Used ONLY internally.
    * @param  userData pointer to the actual C++-object
    * @param  name     the name of the current tag
    */
    static void endElementCallback(void *userData, const XML_Char* name);

  /** C-style callback for regular text from expat.
    * Used ONLY internally.
    * @param  userData pointer to the actual C++-object
    * @param  s        starting buffer with more data
    * @param  len      lenth of buffer
    */
    static void charDataCallback(void *userData, const XML_Char* s, int len);

 private:

  /** Sets the callback handlers.
    */
    void setHandler();

  /** Resets the parser.
    */
    void reset();

 private:

    XML_Parser expatParser;
};



class ExpatWrapper::AttributeList
{
  public:

  /** Construct list from expat pointer
    * @param  parser  pointer to current parser object
    * @param  atts    pointer to the current attributs (unused in XML-RPC)
    */
    AttributeList(ExpatWrapper *parser, const XML_Char** atts);

  /** Tests if an attribute is available
    * @param  name  name of attribute
    * @return true: attribute is available
    */
    bool hasAttribute(const std::string &name) const;

  /** Get the content of an attribute
    * @param  name  name of attribute
    * @return content of attribute
    * @return throw exception if attribute is not in list
    */
    std::string getAttribute(const std::string &name) const;

  private:

    std::map<std::string, std::string>  attributes;
    ExpatWrapper                       *parser;
};


#endif // MOCKPP_EXPATWRAP_H

