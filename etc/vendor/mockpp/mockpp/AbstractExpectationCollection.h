/** @file
    @brief    Check collections of expectations

  $Id: AbstractExpectationCollection.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_ABSTRACTEXPECTATIONCOLLECTION_H
#define MOCKPP_ABSTRACTEXPECTATIONCOLLECTION_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/AbstractExpectation.h>


MOCKPP_NS_START


/** Base class for expectation collections.
  */
template <class T>
class AbstractExpectationCollection : public AbstractExpectation<T>
{
  public:

  /** Constructs the expectation.
    * @param name    human readable description about the expectation
    * @param parent  parent verifiable
    */
    AbstractExpectationCollection(const String &name, VerifiableList *parent)
      : AbstractExpectation<T>(name, parent)
    {
    }


  protected:

  /**
    * Clears the
    * the handling of an error. The Expectation will fail if any actual values are set.
    *
    * Note that this is not the same as not setting any expectations, in which case verify()
    * will do nothing.
    */
    virtual void clearExpectation() = 0;


  /**
    * Resets the internal state completely.
    */
    virtual void reset() = 0;


  /**
    * Checks if the actual value matches the expectation.
    * Note the semantics of the containers of the underlying container of descendants
    * of this class.
    */
    virtual void checkImmediateValue(const T &actualItem) const = 0;
};


MOCKPP_NS_END


#endif // MOCKPP_ABSTRACTEXPECTATIONCOLLECTION_H
