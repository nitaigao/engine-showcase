/** @file
    @internal
    @brief  Build an identity for an invocation

$Id: IdentityBuilder.h 1437 2008-03-08 20:12:53Z ewald-arnold $

***************************************************************************/

/**************************************************************************

   begin                : Sun Aug 22 2004
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

   *
   *  Original Java Sources:  Copyright (c) 2000-2004, jMock.org
   *

 **/

#ifndef MOCKPP_IDENTITYBUILDER_H
#define MOCKPP_IDENTITYBUILDER_H


#include <mockpp/mockpp.h> // always first


MOCKPP_NS_START


/** Builder class for identity related purposes.
  * @internal
  */
class MOCKPP_API_DECL0 IdentityBuilder
{
  public:

  /** Destruct the item.
    * Also makes destructor virtual.
    */
    virtual ~IdentityBuilder();

  /** Sets the identifier for the expectation.
    * @param id  identifier for the expectation
    */
    virtual void id( const String &id ) = 0;
};


MOCKPP_NS_END


#endif // MOCKPP_IDENTITYBUILDER_H
