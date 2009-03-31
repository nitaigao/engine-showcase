/** @file
    @brief    reverse iterator for minimalistic stl

  $Id: reverse_iterator.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef __ministl__reverse_iterator_h
#define __ministl__reverse_iterator_h


MINISTL_NS_START


template<typename ITER>
class reverse_iterator
{
  public:

    typedef  typename ITER::difference_type  difference_type;
    typedef  typename ITER::pointer          pointer;
    typedef  typename ITER::reference        reference;
    typedef  ITER                            iterator_type;
    typedef  typename ITER::value_type       value_type;

    reverse_iterator()
        : current()
    { }

    explicit
    reverse_iterator(iterator_type x)
        : current(x)
    { }

    explicit
    reverse_iterator(pointer x)
        : current(raw_iterator<value_type>(x))
    { }

    reverse_iterator(const reverse_iterator& x)
        : current(x.current)
    { }

    template<typename IT>
    reverse_iterator(const reverse_iterator<IT>& x)
        : current(x.current)
    { }

    iterator_type base() const
    {
      return current;
    }

    reference
    operator*() const
    {
      pointer tmp = current.base()-1;
      return *tmp;
    }

    pointer
    operator->() const
    {
      return &(operator*());
    }

    reverse_iterator&
    operator++()
    {
      --current;
      return *this;
    }

    reverse_iterator
    operator++(int)
    {
      reverse_iterator tmp = *this;
      --current;
      return tmp;
    }

    reverse_iterator&
    operator--()
    {
      ++current;
      return *this;
    }

    reverse_iterator operator--(int)
    {
      reverse_iterator tmp = *this;
      ++current;
      return tmp;
    }

    reverse_iterator
    operator+(difference_type n) const
    {
      return reverse_iterator(current - n);
    }

    reverse_iterator&
    operator+=(difference_type n)
    {
      current -= n;
      return *this;
    }

    reverse_iterator
    operator-(difference_type n) const
    {
      return reverse_iterator(current + n);
    }

    reverse_iterator&
    operator-=(difference_type n)
    {
      current += n;
      return *this;
    }

    reference
    operator[](difference_type n) const
    {
      return *(*this + n);
    }

  private:

    ITER current;
};

template<typename ITER>
inline bool
operator==(const reverse_iterator<ITER>& x,
           const reverse_iterator<ITER>& y)
{
  return x.base() == y.base();
}


template<typename ITER>
inline bool
operator<(const reverse_iterator<ITER>& x,
          const reverse_iterator<ITER>& y)
{
  return y.base() < x.base();
}


template<typename ITER>
inline bool
operator!=(const reverse_iterator<ITER>& x,
           const reverse_iterator<ITER>& y)
{
  return !(x == y);
}


MINISTL_NS_END


#endif // __ministl__reverse_iterator_h


