/** @file
    @brief    Abstract interface for verifiables

  $Id: Verifiable.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Dec 17 2002
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

#ifndef MOCKPP_VERIFIABLE_H
#define MOCKPP_VERIFIABLE_H


#include <mockpp/mockpp.h> // always first


MOCKPP_NS_START


class VerifiableList;

/**
  * A Verifiable is an object that can confirm at the end of a unit test that
  * the correct behaviour has occurred.
  *
  * @see Verifier to check all the Verifiables in an object.
  */
class MOCKPP_API_DECL0 Verifiable
{
  public:

  /** Constructs the expectation.
    * @param name    human readable description about the expectation
    * @param parent  parent verifiable
    */
    Verifiable (const String &name, VerifiableList *parent);

  /** Destructs the expectation.
    */
    virtual ~Verifiable ();

    friend class MockObject;

  /** Throws an AssertionFailedException if any expectations have not been met.
    */
    virtual void verify() = 0;

  /**
    * Sets all internal data to the state after construction.
    */
    virtual void reset() = 0;

  /** Gets the name of the current mock object.
    * Names can be arbitrarily but should be chosen carefully to be able to
    * find the according object.
    * @return  objects name
    */
    String getVerifiableName() const;

  /** Sets a pointer to the parent of this object.
    * @param  parent  pointer to parent
    */
    void setParent(VerifiableList *parent);

  /** Gets a pointer to the parent of this object.
    * @return   pointer to parent
    */
    VerifiableList *getParent() const;

  /** Gets the number of verifiable sub-objects.
    * @return   number of objects
    */
    virtual unsigned numVerifiables() const;

  protected:

  /** Gets a pointer to a verifiable sub-obect.
    * @param  idx   index of object in the list
    * @return   pointer to object    */
    virtual Verifiable *getVerifiable(unsigned idx);

  private:

    VerifiableList          *myParent;
    String                   myName;
};


MOCKPP_NS_END


#endif // MOCKPP_VERIFIABLE_H
