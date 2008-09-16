/** @file
    @brief    Check maps of expectations

  $Id: ExpectationMap.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Dec 26 2002
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

#ifndef MOCKPP_EXPECTATIONMAP_H
#define MOCKPP_EXPECTATIONMAP_H

#include <mockpp/mockpp.h> // always first

#include MOCKPP_ALGORITHM_H
#include MOCKPP_MAP_H

#include <mockpp/ExpectationSet.h>


MOCKPP_NS_START


/** A class to verify maps of expectations: each expected object must occur at
  * least once.
  * \ingroup grp_basic_mo
  */
template <class Key, class Value>
class ExpectationMap : private ExpectationSet<Key>
{
  public:

  /** Constructs the expectation.
    * @param name    human readable description about the expectation
    * @param parent  parent verifiable
    */
    ExpectationMap(const String &name, VerifiableList *parent = 0)
      :  ExpectationSet<Key>(name, parent)
    {
    }


  /** Adds another expectation value to the list.
    * @param pair   key/value pair
    * @return reference to itself for chaining
    */
    ExpectationMap& addExpected(const MOCKPP_STL::pair<const Key, const Value> &pair)
    {
      ExpectationSet<Key>::addExpected(pair.first);
      myEntries.insert(pair);
      return *this;
    }


  /** Adds a sequence of expectation values to the map.
    * @param items    start iterator
    * @param end      terminating iterator (note: one element "behind" as always with STL)
    * @return reference to itself for chaining
    */
    template <class I>
    ExpectationMap& addExpected(I items, I end)
    {
      for ( /* -- */; items != end; ++items)
        addExpected(*items);
      return *this;
    }


  /** Adds another expectation value to the list.
    * @param key    key for the item
    * @param value value of the item
    * @return reference to itself for chaining
    */
    ExpectationMap& addExpected(const Key &key, const Value &value)
    {
      ExpectationSet<Key>::addExpected(key);
      myEntries.insert(MOCKPP_STL::make_pair(key, value));
      return *this;
    }


  /** Adds another expectation value to the list.
    * Only the key is added without a value.
    * @param key    key for the item
    * @return reference to itself for chaining
    */
    ExpectationMap& addExpectedMissing(const Key &key)
    {
      ExpectationSet<Key>::addExpected(key);
      return *this;
    }


  /** Gets a value from the actual values .
    * Only the key is added without a value.
    * @param key    key for the item
    */
    Value get(const Key &key)
    {
      addActual(key);
      return (*myEntries.find(key)).second;
    }


    // move from private to public
    ExpectationSet<Key>::setExpectNothing;
    ExpectationSet<Key>::addActual;
    ExpectationSet<Key>::clearActual;
    ExpectationSet<Key>::reset;
    ExpectationSet<Key>::hasExpectations;
    ExpectationSet<Key>::setFailOnVerify;
    ExpectationSet<Key>::verify;
    ExpectationSet<Key>::getVerifiableName;

  private:

    MOCKPP_STL::map<Key, Value>   myEntries;
};


MOCKPP_NS_END


#endif // MOCKPP_EXPECTATIONMAP_H
