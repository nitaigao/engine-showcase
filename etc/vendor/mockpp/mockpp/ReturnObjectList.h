/** @file
    @brief    Handle returned objects

  $Id: ReturnObjectList.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sat Dec 21 2002
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

#ifndef MOCKPP_RETURNOBJECTLIST_H
#define MOCKPP_RETURNOBJECTLIST_H

#include <mockpp/mockpp.h> // always first

#include MOCKPP_VECTOR_H

#include <mockpp/Verifiable.h>

#include <mockpp/compat/Formatter.h>

#include <mockpp/util/AssertMo.h>


MOCKPP_NS_START


class VerifiableList;

/** This class allows a list of objects to be setup which can be used whilst.The
  * list is check to make sure that all the object in it are used and that none
  * are left over at the end of a test.</p>
  *
  * For every sucessive call to nextReturnObject the next object in the list will
  * returned.
  *
  * If the nextReturnObject method is called and there are no objects in the list
  * an assertion error will be thrown. If the verify method is called and there
  * are objects still in the list an assertion error will also be thrown.
  * \ingroup grp_basic_mo
  */
template <class T>
class ReturnObjectList : public Verifiable
{
  public:

  /** Construct a new empty list
    * @param name Label used to identify list
    * @param parent  parent verifiable
    */
    ReturnObjectList(const String &name, VerifiableList *parent)
     : Verifiable (name, parent)
     , haveDefault(false)
    {
    }


  /** Add a next object to the end of the list.
    * @param anObjectToReturn object to be added to the list
    * @return reference to itself for chaining
    */
    ReturnObjectList& addObjectToReturn(const T &anObjectToReturn)
    {
      myObjects.push_back(anObjectToReturn);
      return *this;
    }


  /** Adds a series of equal next objects to the end of the list.
    * @param anObjectToReturn  object to be added to the list
    * @param count             the count the object is added
    * @return reference to itself for chaining
    */
    ReturnObjectList& addObjectToReturn(const T &anObjectToReturn, unsigned count)
    {
      for (unsigned i = 0; i < count; ++i)
        addObjectToReturn(anObjectToReturn);
      return *this;
    }


  /** Add a sequence of next objects to the end of the list.
    * @param items    start iterator
    * @param end      terminating iterator (note: one element "behind" as always with STL)
    * @return reference to itself for chaining
    */
    template <class I>
    ReturnObjectList& addObjectToReturn(I items, I end)
    {
      for ( /* -- */; items != end; ++items)
        addObjectToReturn(*items);
      return *this;
    }


  /** Sets a default object which is returned when the list is empty.
    * Using the default object does not affect the behaviour of verify().
    * @param defaultObj default object to return
    */
    void setDefaultReturnValue(const T &defaultObj)
    {
      haveDefault = true;
      defaultObject = defaultObj;
    }


  /** Returns the next object from the list. Each object it returned in the
    * order in which they where added.
    */
    T nextReturnObject()
    {
      String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 has run out of objects."));
      fmt << getVerifiableName();

      if (myObjects.size() == 0 && haveDefault)
        return defaultObject;

      MOCKPP_ASSERT_TRUE_MESSAGE(fmt, myObjects.size() > 0);

      T temp = myObjects[0];
      myObjects.erase(myObjects.begin());
      return temp;
    }


  /** Removes all the objects from the list.
    */
    void reset()
    {
      haveDefault = false;
      myObjects.clear();
    }


  /** Transform the object's state to a human readable string.
    * The string only contains the remaining objects.
    * @return string representation
    */
    String toString() const // virtual
    {
      if (myObjects.size() == 0 && !haveDefault)
        return MOCKPP_PCHAR("[]");

      String ret;
      ret += MOCKPP_PCHAR("\n[\n");

      for(unsigned i = 0; i < myObjects.size(); ++i)
      {
        String fmt = MOCKPP_PCHAR(" - %1\n");
        fmt << myObjects[i];
        ret += fmt;
      }

      if (haveDefault)
      {
        String fmt = MOCKPP_PCHAR(" * %1\n");
        fmt << defaultObject;
        ret += fmt;
      }

      ret += MOCKPP_PCHAR("]\n");
      return ret;
    }


  /** Returns true if there are more objects to be returned.
    * @return true: there are more objects available
    */
    bool hasMoreObjects() const
    {
      return myObjects.size() != 0;
    }


  /** Verify that there are no objects left within the list.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void verify()
    {
      String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 has %2 un-used objects."));
      fmt << getVerifiableName() << myObjects.size();

      MOCKPP_ASSERT_EQUALS_MESSAGE(fmt, (size_t)0, myObjects.size());
    }

  private:

    MOCKPP_STL::vector<T>  myObjects;
    bool                   haveDefault;
    T                      defaultObject;
};


MOCKPP_NS_END

#endif // MOCKPP_RETURNOBJECTLIST_H
