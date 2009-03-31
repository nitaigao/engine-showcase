/** @file
    @brief Base class for invocation dispatchers

$Id: AbstractInvocationDispatcher.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

   *
   *  Original Java Sources:  Copyright (c) 2000-2004, jMock.org
   *

 **/

#ifndef MOCKPP_ABSTRACTINVOCATIONDISPATCHER_H
#define MOCKPP_ABSTRACTINVOCATIONDISPATCHER_H

#include <mockpp/mockpp.h> // always first

#include MOCKPP_VECTOR_H

#include <mockpp/chaining/InvocationDispatcher.h>
#include <mockpp/chaining/Invokable.h>

#include <mockpp/stub/StubHolder.h>

#include <mockpp/stub/TestFailureStub.h>


MOCKPP_NS_START


/** Base class for invocation dispatchers.
  */
template <typename R,   // Returntype
          typename I>   // Invocation
class AbstractInvocationDispatcher : public InvocationDispatcher<R, I>
{
  protected:

    typedef Invokable<R, I>    Item;             //!< internal shorthand
    typedef MOCKPP_STL::vector<Item*> List;      //!< internal shorthand

    typedef typename List::iterator          Iterator;         //!< internal shorthand
    typedef typename List::reverse_iterator  ReverseIterator;  //!< internal shorthand
    typedef typename List::const_iterator    ConstIterator;    //!< internal shorthand

  public:

  /** Constructs the invocation dispatcher.
    */
    AbstractInvocationDispatcher()
        : defaultStub( new TestFailureStub<R, I>( MOCKPP_PCHAR( "unexpected invocation" ) ) )
    {}

  /** Destroys the invocation dispatcher.
    */
    virtual ~AbstractInvocationDispatcher()
    {
      for ( unsigned i = 0; i < invokables.size(); ++i )
        delete invokables[ i ];
      invokables.clear();
    }

  /** Sets the default stub for the object
    * @param in_defaultStub  pointer to the default stub
    */
    virtual void setDefaultStub( const MOCKPP_NS::StubHolder<R, I> &in_defaultStub )
    {
      defaultStub = in_defaultStub;
    }

  /** Adds an invokable to the mock object
    * @param invokable  pointer to the invocable
    */
    virtual void add( typename Invokable<R, I>::AP invokable )
    {
      invokables.push_back( invokable.release() );
    }

  /** Verify that the expected value is within the allowed bounds.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void verify()
    {
      for ( unsigned i = 0; i < invokables.size(); ++i )
        invokables[ i ] ->verify();
    }

  /** Clears all internal states
    */
    virtual void reset()
    {
      for ( unsigned i = 0; i < invokables.size(); ++i )
        delete invokables[ i ];
      invokables.clear();
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      if ( anyInvokableHasDescription() )
        writeInvokablesTo( buffer );
      else
        buffer += MOCKPP_PCHAR( "no expectations set" );

      return buffer;
    }

  private:

  /** Transforms the invokables to a human readable string.
    * @param  out_result reference to the output string
    */
    void writeInvokablesTo( String &out_result ) const
    {
      unsigned lines = 0;
      String result;
      for ( unsigned i = 0; i < invokables.size(); ++i )
      {
        if ( invokables[ i ] ->hasDescription() )
        {
          ++lines;
          String s;
          invokables[ i ] ->describeTo( s );
          if (lines > 1)
            s.insert(0, MOCKPP_PCHAR(" - "));
          result += s + MOCKPP_PCHAR( "\n" );
        }
      }

      if (lines > 1)
        out_result += MOCKPP_PCHAR("\n - ");
      out_result += result;
    }

  /** Checks if any invokable has a description.
    * @return true: at least one invokable has a description
    * @return true: none of the invokables have a description
    */
    bool anyInvokableHasDescription() const
    {
      for ( unsigned i = 0; i < invokables.size(); ++i )
        if ( invokables[ i ] ->hasDescription() )
          return true;
      return false;
    }

  protected:

    typename Stub<R, I>::AP defaultStub;  //!< internal shared member
    List                    invokables;   //!< internal shared member
};


MOCKPP_NS_END


#endif // MOCKPP_ABSTRACTINVOCATIONDISPATCHER_H

