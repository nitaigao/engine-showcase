/***************************************************************************
             MockppIdlParser.cpp  -  parse mockpp idl files
                             -------------------
    begin                : Sun May 20 2007
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp@ewald-arnold.de

    $Id: MockppIdlParser.cpp 1442 2008-05-09 14:41:02Z ewald-arnold $

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

#include "MockppIdlParser.h"
#include "xmlexcept.h"

#include <cstring>

/*
 MOCKPP-IDL : SOURCE
            | INCLUDE
            | CLASS
            ;

 CLASS : NAME
       | LINK_SCOPE
       | SUPER
       | METHOD
       | CONSTRUCTOR
       ;

  METHOD | CONSTRUCTOR : TYPE
                       | NAME
                       | ARG
                       ;

  ARG : TYPE
      | NAME
      ;

*/

#define  TAG_MOCKPP_IDL    "MOCKPP-IDL"

#define  TAG_SOURCE        "SOURCE"
#define  TAG_INCLUDE       "INCLUDE"
#define  TAG_CLASS         "CLASS"

#define  TAG_NAME          "NAME"
#define  TAG_LINK_SCOPE    "LINK_SCOPE"
#define  TAG_SUPER         "SUPER"
#define  TAG_METHOD        "METHOD"
#define  TAG_CONSTRUCTOR   "CONSTRUCTOR"

#define  TAG_TYPE          "TYPE"
#define  TAG_ARG           "ARG"


MockppIdlParser::MockppIdlParser()
  : XmlParser()
{
  states.push(new ParserState(eNone));
}


MockppIdlParser::~MockppIdlParser()
{
  while (states.size() != 0)
  {
    delete states.top();
    states.pop();
  }
}


void MockppIdlParser::startElement(const XML_Char* name, const char** atts)
{
  if (!testStartElement(name, atts))
    XmlParser::testStartElement(name, atts);
}


bool MockppIdlParser::testStartElement(const XML_Char* name, const char** atts)
{
  AttributeList attributes (this, atts);

  switch(states.top()->getParserState() )
  {
    case eNone:
      if(strcmp(name, TAG_MOCKPP_IDL) == 0)
      {
        in_arg = false;
        in_method = false;
        theClass.setSource("");
        setComplete (false);
        states.push(new ParserState(eMockppIdl));
      }

      else
        return false;
    break;

    case eMockppIdl:
      if(strcmp(name, TAG_SOURCE) == 0)
        states.push(new ParserState(eSource));

      else if(strcmp(name, TAG_INCLUDE) == 0)
        states.push(new ParserState(eInclude));

      else if(strcmp(name, TAG_CLASS) == 0)
      {
        theClass.clear();
        states.push(new ParserState(eClass));
      }

      else
        return false;

    break;

    case eSuper:
      if(strcmp(name, TAG_TYPE) == 0)
        states.push(new ParserState(eType));

      else
        return false;

    break;

    case eClass:

      if(strcmp(name, TAG_NAME) == 0)
        states.push(new ParserState(eName));

      else if(strcmp(name, TAG_LINK_SCOPE) == 0)
        states.push(new ParserState(eLinkScope));

      else if(strcmp(name, TAG_SUPER) == 0)
        states.push(new ParserState(eSuper));

      else if(strcmp(name, TAG_METHOD) == 0)
      {
        in_method = true;
        method.clear();

        AttributeList attribs(this, atts);

        bool virt = false;
        if (attribs.hasAttribute("virtual"))
          virt = "1" == attribs.getAttribute("virtual");

        bool cons = false;
        if (attribs.hasAttribute("qual"))
          cons = "const" == attribs.getAttribute("qual");

        std::string thro;
        if (attribs.hasAttribute("throw"))
          thro = attribs.getAttribute("throw");

        method.setProperty(false, virt, cons, thro);

        states.push(new ParserState(eMethod));
      }

      else if(strcmp(name, TAG_CONSTRUCTOR) == 0)
      {
        in_method = true;
        method.clear();

        AttributeList attribs(this, atts);

        bool virt = false;
        if (attribs.hasAttribute("virtual"))
          virt = "1" == attribs.getAttribute("virtual");

        bool cons = false;
        if (attribs.hasAttribute("qual"))
          cons = "const" == attribs.getAttribute("qual");

        std::string thro;
        if (attribs.hasAttribute("throw"))
          thro = attribs.getAttribute("throw");

        method.setProperty(true, virt, cons, thro);

        states.push(new ParserState(eConstructor));
      }

      else
        return false;

    break;

    case eMethod:
    case eConstructor: // fallthrough
      if(strcmp(name, TAG_TYPE) == 0)
      {
        AttributeList attribs(this, atts);

        std::string left;
        if (attribs.hasAttribute("qleft"))
          left = attribs.getAttribute("qleft");

        std::string right;
        if (attribs.hasAttribute("qright"))
          right = attribs.getAttribute("qright");

        type.setProperty(left, right);

        states.push(new ParserState(eType));
      }

      else if(strcmp(name, TAG_NAME) == 0)
        states.push(new ParserState(eName));

      else if(strcmp(name, TAG_ARG) == 0)
      {
        arg.clear();
        in_arg = true;
        states.push(new ParserState(eArg));
      }

      else
        return false;

    break;

    case eArg:
      if(strcmp(name, TAG_TYPE) == 0)
      {
        type.clear();

        AttributeList attribs(this, atts);

        std::string left;
        if (attribs.hasAttribute("qleft"))
          left= attribs.getAttribute("qleft");

        std::string right;
        if (attribs.hasAttribute("qright"))
          right = attribs.getAttribute("qright");

        type.setProperty(left, right);

        states.push(new ParserState(eType));
      }

      else if(strcmp(name, TAG_NAME) == 0)
        states.push(new ParserState(eName));

      else
        return false;

    break;

    default:
        return false;
  }

  return true;
}


void  MockppIdlParser::endElement(const XML_Char *name)
{
  if (!testEndElement(name))
    XmlParser::testEndElement(name);
}


bool  MockppIdlParser::testEndElement(const XML_Char *name)
{
  if (states.size() <= 1)
    throw XmlException(NotWellformedError,
                      "Problem while parsing xml structure",
                      getCurrentLineNumber(),
                      std::string("abnormal program behaviour: MockppIdlParser::testEndElement() had no states left: "));

  ParserState *curr = states.top();
  states.pop();

  switch(curr->getParserState() )
  {
    case eMockppIdl:
      setComplete(true);
      assertEndElement(name, TAG_MOCKPP_IDL);
    break;

    case eSource:
      assertEndElement(name, TAG_SOURCE);
      theClass.setSource(curr->getCharData());
    break;

    case eInclude:
      assertEndElement(name, TAG_INCLUDE);
    break;

    case eClass:
      assertEndElement(name, TAG_CLASS);
      classList.push_back(theClass);
    break;

    case eName:
      assertEndElement(name, TAG_NAME);

      if (in_arg)
        arg.setName(curr->getCharData());

      else if (in_method)
        method.setName(theClass.getName(), curr->getCharData());

      else
        theClass.setName(curr->getCharData());
    break;

    case eLinkScope:
      assertEndElement(name, TAG_LINK_SCOPE);
    break;

    case eSuper:
      assertEndElement(name, TAG_SUPER);
    break;

    case eMethod:
      assertEndElement(name, TAG_METHOD);

      theClass.addMethod(method);
      in_method = false;
    break;

    case eConstructor:
      assertEndElement(name, TAG_CONSTRUCTOR);

      theClass.addMethod(method);
      in_method = false;
    break;

    case eType:
      assertEndElement(name, TAG_TYPE);

      type.setName(curr->getCharData());

      if (in_arg)
        arg.setType(type);

      else if (in_method)
        method.setType(type);

      // else  eSuper
    break;

    case eArg:
      assertEndElement(name, TAG_ARG);
      in_arg = false;
      method.addArgument(arg);
    break;

    default:
      states.push(curr);   // put back, someone else will process
      return false;
  }

  delete curr;
  return true;
}


unsigned MockppIdlParser::numClasses() const
{
  return classList.size();
}


MockppIdlClass MockppIdlParser::getClass(unsigned i) const
{
  return classList[i];
}

