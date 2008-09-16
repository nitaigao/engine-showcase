/** @file
    @brief    Base class for mock objects

  $Id: MockObject.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Fri Dec 18 2002
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

#ifndef MOCKPP_MOCKOBJECTS_H
#define MOCKPP_MOCKOBJECTS_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/VerifiableList.h>


MOCKPP_NS_START

/** Advanced Mock Objects provide functionality for some of the common
  * tasks to set up behaviour for a mock object.
  * \defgroup grp_advanced_mo Advanced Mock Objects
  */

/** Base class for setting up mock objcts that are able to
  * emulate a real world object.
  * warning: One MockObject should never have another MockObject as child!
  *          Otherwise there might be collisons when verify()-ing the objects.
  * \ingroup grp_basic_mo
  * \ingroup grp_advanced_mo
  */
class MOCKPP_API_DECL0 MockObject : public VerifiableList
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent verifiable
    */
    MockObject(const String &name, VerifiableList *parent = 0);

  /** Throws a NotImplementedException with the actual objects typeid().
    */
    void notImplemented();

  /** Verifies the object and the mock objects it contains.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void verify();

  /** Sets all internal objects to the state after construction.
    */
    virtual void reset();

  private:

    MockObject (const MockObject &);    // forbid
    MockObject& operator=(MockObject&);

    typedef MOCKPP_STL::vector<const Verifiable*> VerifierVec;

  /**
    * Verifies all the fields of type Verifiable in the given object, including
    * those inherited from superclasses but excluding the object itself
    * @param anObject The object to be verified.
    */
    static void verifyAnObject(VerifiableList* anObject);

  /** Verifies all fields in an object
    * @param anObject          The object to be verified.
    * @param alreadyProcessed  contain already processed objects (check only once)
    */
    static void verifySubFields(Verifiable  *anObject,
                                VerifierVec &alreadyProcessed);

  /** Verifies a single field from an object
    * @param anObject          The object to be verified.
    * @param alreadyProcessed  contain already processed objects (check only once)
    */
    static void verifyField(Verifiable  *anObject,
                            VerifierVec &alreadyProcessed);

  /** Checks if an object is derived from Verifiable
    * @param anObject   The object to be verified.
    * @return true: The object is verifiable
    */
    static bool isVerifiable(const Verifiable *anObject);

    String  myName;
};


MOCKPP_NS_END


#endif // MOCKPP_MOCKOBJECTS_H
