/** @file
    @internal NOT INTENDED FOR PUBLIC INCLUSION, USE @p VisitableMockObject.h INSTEAD
    @brief    Keep ugly templates away from header

  $Id: VisitableMockObject_template.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sat Feb 15 2003
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

#ifndef MOCKPP_VISITABLEMOCKOBJECT_TEMPLATE_H
#define MOCKPP_VISITABLEMOCKOBJECT_TEMPLATE_H

#include <mockpp/constraint/ConstraintHolder.h>
#include <mockpp/constraint/IsEqual.h>


MOCKPP_NS_START


#ifndef DOXYGEN_SHOULD_SKIP_THIS


/** Base class for all objects that return objects or throwables
  * depending on the parameters passed to a method.
  */
class ResponseVectorBase : public Verifiable
{
  public:

  /** Construct a new empty vector
    * @param name    Label used to identify vector
    * @param parent  parent verifiable
    */
    ResponseVectorBase(const String &name, VerifiableList *parent)
      : Verifiable(name, parent),
        tvec(name+MOCKPP_PCHAR("/tvec") , 0)  // <== NO PARENT
    {
    }

  /** Verify that all expectations are met.
    */
    virtual void verify()
    {
//    tvec.verify();   Not this one because it NEVER runs empty, use counter[] instead

      unsigned unused = 0;
      MOCKPP_STL::vector<unsigned>::const_iterator start = counter.begin();
      MOCKPP_STL::vector<unsigned>::const_iterator end = counter.end();
      for ( ; start != end; ++start)
        if (*start != MOCKPP_UNLIMITED)  // @todo ouput signatures of unused objects
          unused += *start;

      String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 has a total of %2 un-used response objects."));
      fmt << getVerifiableName() << unused;

      MOCKPP_ASSERT_EQUALS_MESSAGE(fmt, (unsigned)0, unused);
    }


  /** Returns the amount of response values.
    * @return number of response values
    */
    unsigned size() const
    {
      return counter.size();
    }


  protected:

    ThrowableList         tvec;
    MOCKPP_STL::vector<unsigned> counter;
};


#endif // DOXYGEN_SHOULD_SKIP_THIS


MOCKPP_NS_END


#include "ResponseVector1.h"
#include "ResponseVector2.h"
#include "ResponseVector3.h"
#include "ResponseVector4.h"
#include "ResponseVector5.h"


#endif // MOCKPP_VISITABLEMOCKOBJECT_TEMPLATE_H

