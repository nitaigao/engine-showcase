/** @file
    @brief     A collection of verifiables

  $Id: VerifiableList.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_VERIFIABLELIST_H
#define MOCKPP_VERIFIABLELIST_H


#include <mockpp/mockpp.h> // always first

#include <mockpp/Verifiable.h>

#include MOCKPP_VECTOR_H


MOCKPP_NS_START


/**
  * A collection of verifiable objects which are verify()ed together.
  */
class MOCKPP_API_DECL0 VerifiableList : public Verifiable
{
  public:

  /** Constructs the expectation.
    * @param name    human readable description about the expectation
    * @param parent  parent verifiable
    */
    VerifiableList (const String &name, VerifiableList *parent);

  /**
    * Throws an AssertionFailedException if any expectations have not been met.
    */
//    virtual void verify() const = 0;

  protected:

  friend class Verifier;
  friend class Verifiable;

  /** Adds another verifiable object to the list.
    * @param vf   pointer to the verifiable object
    */
    virtual void addVerifiable (Verifiable* vf);

  /** Removes a verifiable object from the list.
    * @param vf   pointer to the verifiable object
    */
    void removeVerifiable (Verifiable* vf);

  /** Tests of a verifiable object is in the list.
    * @param vf   pointer to the verifiable object
    */
    bool hasVerifiable (Verifiable* vf) const;

  /** Gets the number of verifiable sub-objects in the list.
    * @return   number of objects
    */
    virtual unsigned numVerifiables() const;

  /** Gets a pointer to a verifiable sub-object.
    * @param  idx   index of object in the list
    * @return   pointer to object or 0 when index too high
    */
    virtual Verifiable *getVerifiable(unsigned idx);

  /** Removes all verifiable fields from the list.
    */
    virtual void clearVerifiables();

  private:

    VerifiableList (const VerifiableList &);    // forbid
    VerifiableList& operator=(VerifiableList&);

    MOCKPP_STL::vector<Verifiable*>  verifiables;
};


MOCKPP_NS_END


#endif // MOCKPP_VERIFIABLELIST_H
