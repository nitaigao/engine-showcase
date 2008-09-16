/** @file
    @brief    Support throwing exceptions

  $Id: Throwable.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_THROWABLE_H
#define MOCKPP_THROWABLE_H

#include <mockpp/mockpp.h> // always first


MOCKPP_NS_START


/** A virtual base class to handle throwable objects
  * \ingroup grp_basic_mo
  */
class Throwable
{
  public:

  /**
    * Throws the object.
    */
    virtual void throw_me() = 0;

  /**
    * Queries if the object has been thrown.
    * @return true: object has been thrown at least once
    */
    virtual bool hasThrown() const = 0;

  /**
    * Destruct the Throwable.
    */
    virtual ~Throwable()
    {}
};


/**
  * A helper template to wrap a throwable object in a uniform fashion.
  */
template <class T>
class ThrowableWrapper : public Throwable
{
  public:

  /**
    * Throws the object.
    */
    virtual void throw_me()
    {
       ++thrown;
       MOCKPP_THROW(obj);
    }


  /**
    * Queries if the object has been thrown.
    * @return true: object has been thrown at least once
    */
    virtual bool hasThrown() const
    {
      return thrown > 0;
    }


  /**
    * Resets the counter to 0.
    */
    void reset()
    {
      thrown = 0;
    }


  /**
    * Contructs the ThrowableWrapper.
    * @param o  object to throw
    */
    ThrowableWrapper(const T &o)
     : obj(o),
       thrown(0)
    {}

  private:

    T        obj;
    unsigned thrown;
};


/**
  * Helper template to create a Throwable from an ordinary object.
  * @param  w   the object to throw
  * @return a pointer to the throwable object
  */
template <class T>
Throwable *make_throwable(const T &w)
{
  return new ThrowableWrapper<T> (w);
}


/**
  * A throwable object. The object is completely managed which also includes
  * the destruction of the object by the destructor of this container.
  * The behaviour when copying is similar to std::auto_ptr<T>: the ownership
  * changes.
  */
class MOCKPP_API_DECL0 ThrowableItem
{
  public:

  /**
    * Construct the item.
    * @param  it  pointer to the throwable object
    */
    ThrowableItem(Throwable *it = 0);

  /**
    * Construct the item from another.
    * @param  it  Another object. The internal pointers ownership is moved
    *             and the other object loses the object.
    */
    ThrowableItem(const ThrowableItem &it);

  /**
    * Copy the item from another.
    * @param  it  Another object. The internal pointers ownership is moved
    *             and the other object loses the object.
    */
    ThrowableItem &operator = (const ThrowableItem &it);

  /**
    * Destruct the item.
    */
    virtual ~ThrowableItem() throw();

  /**
    * Set the throwable item.
    * Attention: In case you pass "0" to clear the object, don't forget to
    * cast it to "Throwable", otherwise the template method will be taken!
    * @param  it  pointer to the throwable object
    */
    void take(Throwable *it);

  /**
    * Set the throwable item.
    * @param  w  the object to throw
    */
    template <class T>
    void take(const T &w)
    {
      take(make_throwable(w));
    }

  /**
    * Releases the internal pointer.
    * @return pointer to the throwable
    */
    Throwable *release();

  /**
    * Gets the throwable item.
    * @return pointer to the throwable
    */
    Throwable *get() const;

  /**
    * Sets the object into a clean and unused state
    */
    void reset();

  /**
    * Queries if the throwable object was taken via get().
    * There must also be an object set for this being true.
    * @return true: object was taken at least once
    */
    bool wasUsed() const;

  private:

    Throwable  *item;
};


MOCKPP_NS_END


#endif // MOCKPP_THROWABLE_H
