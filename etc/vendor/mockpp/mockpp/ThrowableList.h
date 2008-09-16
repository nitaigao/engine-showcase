/** @file
    @brief    Support throwing lists of exceptions

  $Id: ThrowableList.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Mon Jan 20 2003
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

#ifndef MOCKPP_THROWABLELIST_H
#define MOCKPP_THROWABLELIST_H

#include <mockpp/mockpp.h> // always first

#include MOCKPP_VECTOR_H

#include <mockpp/Verifiable.h>
#include <mockpp/Throwable.h>


MOCKPP_NS_START


/** A list of throwable objects. The objects are completely managed which also includes
  * the destruction of the objects by the destructor of this list.
  * \ingroup grp_basic_mo
  */
class MOCKPP_API_DECL0 ThrowableList : public Verifiable
{
  public:

  /** Construct a new empty list
    * @param aName   Label used to identify list
    * @param parent  parent verifiable
    */
    ThrowableList(const String &aName, VerifiableList *parent);

  /** Destruct the list.
    */
    virtual ~ThrowableList() throw();

  /** Add a next object to the end of the list.
    * The objects remains from now on in the responsibiliy of this list until
    * the list itself is destroyed! This is valid even for objects taken via
    * nextThrowableObject()!
    * Don't add the same object (the same pointer!) more than once via this method.
    * @deprecated
    * @param t pointer to object to be added to the list
    */
    void push_back(Throwable* t);

  /** Add a next object to the end of the list.
    * The objects remains from now on in the responsibiliy of this list until
    * the list itself is destroyed! This is valid even for objects taken via
    * nextThrowableObject()!
    * Don't add the same object (the same pointer!) more than once via this method.
    * @param t pointer to object to be added to the list
    */
    void addObjectToThrow(Throwable* t);

  /** Add a next object to the end of the list.
    * The objects remains from now on in the responsibiliy of this list until
    * the list itself is destroyed! This is valid even for objects taken via
    * nextThrowableObject()!
    * @deprecated
    * @param w  object to be added to the list
    */
    template <class T>
    void push_back(const T &w)
    {
      addObjectToThrow(w);
    }

  /** Add a next object to the end of the list.
    * The objects remains from now on in the responsibiliy of this list until
    * the list itself is destroyed! This is valid even for objects taken via
    * nextThrowableObject()!
    * @param w  object to be added to the list
    */
    template <class T>
    void addObjectToThrow(const T &w)
    {
      push_back(make_throwable(w));
    }

  /** Returns the next object from the list. Each object it returned in the
    * order in which they where added.
    */
    Throwable *nextThrowableObject();

  /** Returns true if there are more objects to be returned.
    */
    bool hasMoreObjects() const;

  /** Returns a pointer to the specified object in the list.
    * The list is not changed!
    * @param  i   index of the object
    */
    Throwable * at(unsigned i);

  /** Verify that there are no objects left within the list.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void verify();

  /** Sets all internal objects to the state after construction.
    */
    virtual void reset();

  /** Returns the amount of response values.
    * The count covers already used and still unused obbjects!
    * @return number of response values
    */
    unsigned size() const;

  private:

    ThrowableList (const ThrowableList &);    // forbid
    ThrowableList& operator=(ThrowableList&);

    MOCKPP_STL::vector<Throwable *>  list;
    MOCKPP_STL::vector<Throwable *>  already_used;
};


MOCKPP_NS_END


#endif // MOCKPP_THROWABLELIST_H
