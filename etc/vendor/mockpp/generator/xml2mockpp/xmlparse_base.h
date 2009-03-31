/***************************************************************************
                       xmlparse_base.h  -  parse xml files
                             -------------------
    begin                : Sun May 20 2007
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp@ewald-arnold.de

    $Id: xmlparse_base.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_XMLPARSE_BASE
#define MOCKPP_XMLPARSE_BASE

#include <stack>
#include <string>

#ifdef HAVE_XMLPARSE_H
#include "expat/xmlparse.h"
#else
#include "expat/expat.h"
#undef  XMLPARSEAPI
#define XMLPARSEAPI
#endif

/** Base class for XML parsing.
  */
class XmlParserBase
{
 public:

 /** Constructs a parser.
   */
   XmlParserBase();

 /** Destroys the parser.
   * The derived class is responsible for cleaning up stack<ParserState*>.
   */
   virtual ~XmlParserBase();

 /** Tests if parsing has completed.
   * Completed means that all opening tags have been correctly closed.
   * @return true if completed
   */
   bool isComplete() const;

 /** Sets the complete state.
   * @param  comp true if file read
   */
   void setComplete(bool comp);

 /** Parse a pice of xml data.
   * @param buffer   pointer start of next data chunk
   * @param len      len of this chunk
   * @param isFinal  true: last call to parser
   * @return error condition, 0 = ok
   */
   virtual int parse(const char* buffer, int len, int isFinal) = 0;

  /** Gets the code for the current error.
   * @return error code
   */
   virtual unsigned getErrorCode() const = 0;

 /** Gets the description for an error code
   * @param code  error code
   * @return  pointer to description
   */
   virtual std::string getErrorString(unsigned code) const = 0;

 /** Gets the line number in the xml data.
   * Because the binary data has nothing like a line number, the occurence number
   * of the previous tag is returned.
   * @return  line number
   */
   virtual int getCurrentLineNumber() const = 0;

 /** Maps expat error codes to xml-rpc error codes.
   * @param  xpatcode   error code from expat
   * @return  the according xml-rpc error
   */
   virtual int mapToFaultCode(int xpatcode) const = 0;

   enum State
   {
     eNone,               //!<  state after start
     eUnknown,            //!<  unknwon state when an error occured
     eXmlParserLast       //!<  used to chain next parser class
   };

/** Helper class to represent the data of the current parsing step.
  */
  class ParserState
  {
   public:

   /** Constructs a ParserState.
     * @param  st  the actual ParserState
     */
     ParserState (unsigned st);

   /** Destroys the ParserState.
     */
     virtual ~ParserState();

   /** Gets the ParserState of this ParserState
     * @return the actual ParserState
     */
     unsigned getParserState() const;

   /** Gets the privious ParserState
     * @return the previous ParserState
     */
     unsigned getPrevParserState() const;

   /** Sets the privious ParserState
     * @param  prev the previous ParserState
     */
     void setPrevParserState(unsigned prev);

   /** Gets the name of the ParserState.
     * Useful only for debugging.
     * @return the name of actual ParserState
     */
     virtual std::string getStateName() const;

   /** Appends some characters of the ParserState.
     * This is a part of the data of an xml rpc element.
     * @param  s   the current chunk of text
     * @param  len valid len.
     */
     void appendCharData(const XML_Char *s, int len);

   /** Appends some characters of the ParserState.
     * This is a part of the data of an xml rpc element.
     * @param  s   the current chunk of text
     */
     void appendCharData(const std::string &s);

   /** Gets the characters of the ParserState.
     * @return  the data element
     */
     std::string getCharData() const;

   private:

     ParserState(const ParserState&); // forbid this
     ParserState& operator= (const ParserState&);

     std::string  cdata;
     unsigned     state;
     unsigned     prevstate;
  };

 protected:

 /** Removes all states from the state stack.
   */
   void clearStates();

 protected:

   std::stack<ParserState*>  states;

 private:

   bool complete;
};


#endif // MOCKPP_XMLPARSE_BASE
