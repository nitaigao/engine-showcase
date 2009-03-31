/** @file
    @brief    Raw iterator for minimalistic stl

  $Id: raw_iterator.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Dec 1 2005
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

#ifndef __ministl__raw_iterator_h
#define __ministl__raw_iterator_h

MINISTL_NS_START

template<typename T>
class raw_iterator
{
  public:

    typedef  MINISTL_STD_NS::size_t   difference_type;
    typedef  T*            pointer;
    typedef  const T*      const_pointer;
    typedef  T&            reference;
    typedef  const T&      const_reference;
    typedef  T             value_type;

    raw_iterator ()
        : current()
    { }

    //    explicit
    raw_iterator (pointer x)
        : current(x)
    { }

    raw_iterator (const raw_iterator & x)
        : current(x.current)
    { }

    template<typename IT>
    raw_iterator (const raw_iterator <IT>& x)
        : current(x.base())
    { }

    pointer base() const
    {
      return current;
    }

    pointer
    operator*() const
    {
      return current;
    }

    pointer
    operator->() const
      { return &(operator*()); }

    raw_iterator &
    operator++()
    {
      ++current;
      return *this;
    }

    raw_iterator
    operator++(int)
    {
      raw_iterator  tmp = *this;
      ++current;
      return tmp;
    }

    raw_iterator &
    operator--()
    {
      --current;
      return *this;
    }

    raw_iterator  operator--(int)
    {
      raw_iterator  tmp = *this;
      --current;
      return tmp;
    }

    raw_iterator
    operator+(difference_type n) const
    {
      return raw_iterator (current + n);
    }

    raw_iterator &
    operator+=(difference_type n)
    {
      current += n;
      return *this;
    }

    raw_iterator
    operator-(difference_type n) const
    {
      return raw_iterator (current - n);
    }

    raw_iterator &
    operator-=(difference_type n)
    {
      current -= n;
      return *this;
    }

    reference
    operator[](difference_type n) const
    {
      return *(*this + n);
    }

  private:

    pointer  current;
};


template<typename T>
inline bool
operator==(const raw_iterator <T>& x,
           const raw_iterator <T>& y)
{
  return x.base() == y.base();
}

template<typename T>
inline bool
operator<(const raw_iterator <T>& x,
          const raw_iterator <T>& y)
{
  return y.base() < x.base();
}

template<typename T>
inline bool
operator!=(const raw_iterator <T>& x,
           const raw_iterator <T>& y)
{
  return !(x == y);
}


MINISTL_NS_END


#endif // __ministl__raw_iterator_h
