/** @file
    @brief  Chainable Mock Methods based on templates

  $Id: ChainableMockMethod.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Oct 2 2005
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

 **/


#ifndef MOCKPP_CHAINABLEMOCKMETHOD_H
#define MOCKPP_CHAINABLEMOCKMETHOD_H

#include <mockpp/mockpp.h>

#include <mockpp/mockpp_dbc.h>
#include <mockpp/chaining/ChainableMockObject.h>
//#include <mockpp/CountedChainableMethod.h>


MOCKPP_NS_START


/** Common stuff to set up chainable mock method expectations
  * @ingroup grp_controller
  * @internal
  */
class ChainableMockMethodCommon : public MockObject
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    ChainableMockMethodCommon(const String &name, ChainableMockObject *parent )
      : MockObject(name, parent)
      , chainable(parent)
    {
      MOCKPP_PRE(parent != 0);
      MOCKPP_PRE(name.length() != 0);
    }

  /** Returns the underlying mock object.
    * @return pointer to mock object
    */
    ChainableMockObject *getChainableMockObject() const
    {
      return chainable;
    }

  /** Returns the mock object name.
    * @return mock object name
    */
    String getMethodName() const
    {
//       static CppString method_name = getVerifiableName() + MOCKPP_PCHAR(".") + m_name;
      return getVerifiableName();
    }

  private:

    ChainableMockObject   *chainable;
};


MOCKPP_NS_END


#endif // MOCKPP_CHAINABLEMOCKMETHOD_H

