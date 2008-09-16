/** @file
    @brief MockObject that merges all the advanced mock objects

  $Id: MixedMockObject.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sat Mar 05 2005
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

#ifndef MOCKPP_MixedMOCKOBJECT_H
#define MOCKPP_MixedMOCKOBJECT_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/visiting/VisitableMockObject.h>

#include <mockpp/chaining/ChainableMockObject.h>


MOCKPP_NS_START


/** A MockObject that merges all the advanced mock objects.
  * \ingroup grp_advanced_mo
  */
class MOCKPP_API_DECL0 MixedMockObject : public MockObject
					   	     		   , public VisitableMockObjectBase
									   , public ChainableMockObjectBase
{
  public:

  /** Construct the item.
    * @param name    human readable description about the object
    * @param parent  parent verifiable
    */
    MixedMockObject(const String &name, VerifiableList *parent = 0);

  /** Destruct the item.
    */
    virtual ~MixedMockObject();

  /** Frees all internal data.
    */
    virtual void reset();

  /** Verifies the object and the mock objects it contains.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void verify();
};


MOCKPP_NS_END


#endif // MOCKPP_MixedMOCKOBJECT_H

