/** @file
    @brief  Stub returning a limited sequence of values

  $Id: TypelessStubSequence.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Fri Oct 22 2004
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

#ifndef MOCKPP_TypelessStubSequence_H
#define MOCKPP_TypelessStubSequence_H

#include <mockpp/mockpp.h> // always first

#include MOCKPP_VECTOR_H

#include <mockpp/stub/TypelessStub.h>
#include <mockpp/compat/Asserter.h>


MOCKPP_NS_START


/** Use one stub after the other from a sequence.
  * @ingroup grp_stub
  * @see mockpp::throwException
  */
template <typename R>  // Invocation
class TypelessStubSequence : public TypelessStub<R>
{
  public:

  /** Constructs the object.
    * @param  start interator for starting element
    * @param  end   interator for trailing element
    */
    template<typename P>
    TypelessStubSequence(P start, P end)
      : stubs(start, end)
    {}

  /** Destroys the object.
    */
    virtual ~TypelessStubSequence()
    {
      while (!stubs.empty() )
      {
        delete stubs[0];
        stubs.erase(stubs.begin());
      }
    }

  /** Mock the invocation without passing parameters.
    * @return return value from mocked invocation
    */
    virtual R typelessInvoke()
    {
        if (!stubs.empty())
        {
            MOCKPP_STD_NS::auto_ptr< TypelessStub<R> > curr (stubs[0]);
            stubs.erase(stubs.begin());
            return curr->typelessInvoke();
        }
        assertionFailed(__LINE__, __FILE__, MOCKPP_PCHAR("no more stubs available"));
        return R();
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
        for (unsigned i = 0; i < stubs.size(); i++)
        {
            if (i > 0)
              buffer += MOCKPP_PCHAR(", and then ");

            buffer = stubs[i]->describeTo(buffer);
        }

        return buffer;
    }

  private:

    // forbid due to pointers
    TypelessStubSequence(const TypelessStubSequence &);
    TypelessStubSequence &operator =(const TypelessStubSequence &);

    MOCKPP_STL::vector<TypelessStub<R>* >  stubs;
};


MOCKPP_NS_END


#endif // MOCKPP_TypelessStubSequence_H

