/** @file
    @brief    Abstract interface for expectations

  $Id: Expectation.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_EXPECTATION_H
#define MOCKPP_EXPECTATION_H


#include <mockpp/mockpp.h> // always first

#include <mockpp/Verifiable.h>


MOCKPP_NS_START

/** Often fully featured mock objects are not appropriate for a testing problem.
  * In this case you can write your own mock objects with minimal functionality.
  * Unfortunately such objects carry little or no additional information which
  * makes it hard to find the cause and location of the problem.
  *
  * For that reason mockpp contains some classes to address such simple problems
  * which also provide some human readable information.
  *
  * Call verify() at the end of a unit test to check for missing or incomplete values.
  * If no expectations have been set on the object, then no checking will be done and
  * verify() will do nothing.
  * \defgroup grp_poor_mo Poor Man's Mock Objects
  */

class VerifiableList;

/** An Expectation is an object that we set up at the beginning of a unit test to
  * expect certain things to happen to it. If it is possible to tell, the Expectation will
  * fail as soon as an incorrect value has been set.
  *
  * Call verify() at the end of a unit test to check for missing or incomplete values.
  *
  * If no expectations have been set on the object, then no checking will be done and
  * verify() will do nothing.
  */
class MOCKPP_API_DECL0 Expectation : public Verifiable

{
  public:

  /** Constructs the expectation.
    * @param name    human readable description about the expectation
    * @param parent  parent verifiable
    */
    Expectation (const String &name, VerifiableList *parent)
     : Verifiable(name, parent)
    {}

  /**
    * Tests if any expectations have been set on this object.
    * @return true: there are expectations
    */
    virtual bool hasExpectations() const = 0;

  /**
    * Tell the object to expect nothing to happen to it, perhaps because the test is exercising
    * the handling of an error. The Expectation will fail if any actual values are set.
    *
    * Note that this is not the same as not setting any expectations, in which case verify()
    * will do nothing.
    */
    virtual void setExpectNothing() = 0;

  /**
    * If an incorrect actual value is set, defer reporting this as a failure until verify()
    * is called on this object.
    */
    virtual void setFailOnVerify() = 0;
};


/** \defgroup grp_basic_mo Basic Mock Objects
  */


MOCKPP_NS_END


#endif // MOCKPP_EXPECTATION_H
