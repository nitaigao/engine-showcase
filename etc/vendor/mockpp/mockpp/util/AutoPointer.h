/** @file
    @internal
    @brief    an auto-pointer class

  $Id: AutoPointer.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

 **/

#ifndef MOCKPP_AUTOPOINTER_H
#define MOCKPP_AUTOPOINTER_H

#include <mockpp/mockpp.h> // always first


MOCKPP_NS_START

/** Implementation of an auto-pointer class.
  * 99% re-implemented from std::auto_ptr. The main difference is the
  * missing "explicit" in the constructor which is needed for the use of
  * my owner-change-idiom. Additionally the new class is slightly modified.
  *
  * For a complete documentation of auto-pointers read your favorite C++ book.
  *
  * @internal
  * @warning The standard implementation and its use of "explicit" and "const" have a good
  *          reason! AutoPointer is primarily for internal use as it is actually a liar.
  */
template<class X>
class AutoPointer
{
  public:

  /** Constructor
    * @internal
    * @see std::auto_ptr
    */
    AutoPointer (X* p = 0)
     : pointee(p)
    {
    }

  /** Constructor
    * @internal
    * @see std::auto_ptr
    */
    AutoPointer (const AutoPointer& a)
     :  pointee(const_cast<AutoPointer&>(a).release())
    {
    }

#if defined(_MSC_VER) && (_MSC_VER <= 1300) // at least not MSCV6
    // MAT : hmm dont know how it can actually work properly without this operator ?!
#else

  /** Constructor
    * @internal
    * @see std::auto_ptr
    */
    AutoPointer& operator= (const AutoPointer& rhs)
    {
      reset(const_cast<AutoPointer&>(rhs).release());
      return *this;
    }

#endif

  /** Constructor
    * @internal
    * @see std::auto_ptr
    */
    template <class Y>
    AutoPointer& operator= (const AutoPointer<Y>& rhs)
    {
#ifdef __BORLANDC__
      reset(const_cast<AutoPointer<Y>&>(rhs).release()));
#else
      reset(const_cast<Y*>(rhs.release()));
#endif
      return *this;
    }

  /** Converter
    * @internal
    * @see std::auto_ptr
    */
    template <class Y>
    operator AutoPointer<Y>()
    {
      return AutoPointer<Y>(release());
    }

  /** Destructor
    * @internal
    * @see std::auto_ptr
    */
    ~AutoPointer ()
    {
      delete pointee;
    }

  /** Converter
    * @internal
    * @see std::auto_ptr
    */
    X& operator*()  const
    {
      return *pointee;
    }

  /** Converter
    * @internal
    * @see std::auto_ptr
    */
    X* operator-> () const
    {
      return pointee;
    }

  /** Converter
    * @internal
    * @see std::auto_ptr
    */
    X* get() const
    {
      return pointee;
    }

  /** Releaser
    * @internal
    * @see std::auto_ptr
    */
    X* release()
    {
      X* tmp = pointee;
      pointee = 0;
      return tmp;
    }

  /** Releaser
    * @internal
    * @see std::auto_ptr
    */
    void reset (X* p = 0)
    {
      if (pointee != p)
      {
        delete pointee;
        pointee = p;
      }
    }

  private:

    X* pointee;
};


MOCKPP_NS_END


#endif // MOCKPP_AUTOPOINTER_H
