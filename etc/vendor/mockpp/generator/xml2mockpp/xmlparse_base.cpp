/**************************************************************************
                     xmlparse_base.cpp  -  parse xml files
                             -------------------
    begin                : Sun May 20 2007
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp@ewald-arnold.de

    $Id: xmlparse_base.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#include "xmlparse_base.h"


XmlParserBase::XmlParserBase()
{
  complete = false;
}


XmlParserBase::~XmlParserBase()
{
}


void XmlParserBase::clearStates()
{
  while (!states.empty())
  {
    delete states.top();
    states.pop();
  }
}

bool XmlParserBase::isComplete() const
{
  return complete;
}


void XmlParserBase::setComplete(bool comp)
{
  complete = comp;
}


//////////////////////////////////////////////////////////////////////////
//

XmlParserBase::ParserState::ParserState (unsigned st)
  : state(st)
  , prevstate(eUnknown)
{
}


XmlParserBase::ParserState::~ParserState()
{
}


unsigned XmlParserBase::ParserState::getParserState() const
{
  return state;
}


unsigned XmlParserBase::ParserState::getPrevParserState() const
{
  return prevstate;
}


void XmlParserBase::ParserState::setPrevParserState(unsigned prev)
{
  prevstate = prev;
}


std::string XmlParserBase::ParserState::getStateName() const
{
  return "eUnknown";
}


void XmlParserBase::ParserState::appendCharData(const std::string &s)
{
  cdata.append(s);
}


void XmlParserBase::ParserState::appendCharData(const XML_Char *s, int len)
{
  cdata.append(s, len);
}


std::string XmlParserBase::ParserState::getCharData() const
{
  return cdata;
}

