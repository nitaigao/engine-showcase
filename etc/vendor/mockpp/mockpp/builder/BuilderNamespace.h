/** @file
    @internal
    @brief  Interface class to build a namespace

$Id: BuilderNamespace.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_BUILDERNAMESPACE_H
#define MOCKPP_BUILDERNAMESPACE_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/chaining/Invocation.h>

#include <mockpp/matcher/InvokedRecorder.h>


MOCKPP_NS_START


class MatchBuilderAdapterBase;

/** Store the objects needed to build mock objects.
  * @internal
  */
class MOCKPP_API_DECL0 BuilderNamespace
{
  public:

  /** Construct the item.
    * @param name    human readable description about the object
    */
    BuilderNamespace(const String &name);

  /** Destruct the item.
    * Also makes destructor virtual.
    */
    virtual ~BuilderNamespace();

  /** Returns the human readable identifier.
    * @return identifier
    */
    String getIdent() const;

  /** Registers another match builder under its unique identifier.
    * @internal
    * @param   id       builder's identifier
    * @param   builder  pointer to builder object
    */
    virtual void registerUniqueID( const String &id, MatchBuilderAdapterBase *builder ) = 0;

  /** Stores a builder object.
    * @internal
    * @param   builder  pointer to builder object
    */
    virtual void storeBuilder(AutoPointer<MatchBuilderAdapterBase> builder) = 0;

  /** Searches a name.
    * @internal
    * @param id   item's name
    * @return   if name was found: pointer to matcher
    * @return   if name was not found: 0
    */
    virtual MatchBuilderAdapterBase *lookupID( const String &id ) const = 0;

  /** Frees all internal data.
    * @internal
    */
    virtual void reset() = 0;

  /** Stores another recorder object for later use.
    * @internal
    * @param   id        recorder's identifier
    * @param   recorder  pointer to recorder object
    */
    virtual void addPendingRecorder(const String &id, InvokedRecorder::AP recorder) = 0;

  private:

    String  ns_name;
};


MOCKPP_NS_END


#endif // MOCKPP_BUILDERNAMESPACE_H
