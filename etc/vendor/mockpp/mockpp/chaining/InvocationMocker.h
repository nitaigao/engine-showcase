/** @file
    @internal
    @brief  Mock invocations

$Id: InvocationMocker.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   *
   *  Original Java Sources:  Copyright (c) 2000-2004, jMock.org
   *

 **/

#ifndef MOCKPP_INVOCATIONMOCKER_H
#define MOCKPP_INVOCATIONMOCKER_H


#include <mockpp/mockpp.h> // always first

#include MOCKPP_VECTOR_H

#include <mockpp/chaining/StubMatchersCollection.h>
#include <mockpp/chaining/Invokable.h>
#include <mockpp/chaining/InvocationMocker.h>

#include <mockpp/matcher/ArgumentsMatcher.h>

#include <mockpp/compat/AssertionFailedError.h>

#include <mockpp/stub/DefaultResultStub.h>
#include <mockpp/stub/VoidStub.h>
#include <mockpp/stub/StubHolder.h>


MOCKPP_NS_START

/** Base class for helper objects to mock invocations.
  * @internal
  */
template <typename R,     // Returntype
          typename I>     // Invocation
class InvocationMockerBase : public Invokable<R, I>,
                             public StubMatchersCollection<R, I>
{
  public:

    class DescriberBase;

  protected:

  /** Constructs the object
    * @param  in_describer  pointer to describer object
    * @param  defaultStub   pointer to default stub object
    */
    InvocationMockerBase( AutoPointer<DescriberBase> in_describer,
                          const StubHolder<R, I> &defaultStub)
      : stub(defaultStub)
      , describer(in_describer)
    {
    }

  public:

#ifdef MOCKPP_NO_TYPENAME_FOR_STL_NS
    typedef  MOCKPP_STL::vector<InvocationMatcher<I>*> List;           //!< internal shorthand
#else
    typedef typename MOCKPP_STL::vector<InvocationMatcher<I>*> List;   //!< internal shorthand
#endif
    typedef typename List::iterator Iterator;                          //!< internal shorthand
    typedef typename List::const_iterator ConstIterator;               //!< internal shorthand

  /** Destroys the object
    */
    virtual ~InvocationMockerBase()
    {
      reset();
    }

  /** Removes all the objects from the list.
    */
    virtual void reset()
    {
      for ( Iterator it = matchers.begin(); it != matchers.end(); ++it )
        delete (*it);
      matchers.clear();
    }

  /** Checks if an invocation matches the expectaton
    * @param  invocation  the invocation data
    * @return true: the invocation matches
    */
    virtual bool matches( const I &invocation )
    {
      for ( Iterator it = matchers.begin(); it != matchers.end(); ++it )
      {
        if ( !( *it ) ->matches( invocation ) )
          return false;
      }
      return true;
    }

  /** Verify that the expected value is within the allowed bounds.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void verify()
    {
      MOCKPP_TRY
      {
        for ( ConstIterator it = matchers.begin(); it != matchers.end(); ++it )
          ( *it ) ->verify();
      }

#ifndef MOCKPP_NO_EXCEPTIONS
      catch ( const AssertionFailedError & error )
      {
        assertionFailed( __LINE__, __FILE__,
                                    error.getMessage() + MOCKPP_PCHAR( "\n" ) + this->toString() );
      }
#endif // MOCKPP_NO_EXCEPTIONS
    }

  /** Sets the object's name
    */
    virtual void setName( const String &in_name )
    {
      name = in_name;
    }

  /** Adds another matcher.
    * @param holder the matcher object
    * @return the builder object
    */
    virtual void addMatcher(  const MatcherHolder<I> &holder )
    {
      typename InvocationMatcher<I>::AP matcher = holder;
      matchers.push_back( matcher.release() );
    }

  /** Sets the stub data for the invocation.
    * @param in_stub  the stub object
    */
    virtual void setStub( const StubHolder<R, I> &in_stub )
    {
      stub = in_stub;
    }

  /** Does the object provide meaningful description via describeTo() ?.
    * @return true: description is available
    */
    virtual bool hasDescription()
    {
      return describer->hasDescription();
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      return describer->describeTo( buffer, matchers, stub.get(), name );
    }

    ////////////////////////////////////////////////////
    //

  /** Base for describer objects
    */
    class DescriberBase
    {
      public:

        typedef AutoPointer<DescriberBase> AP;   //!< internal shorthand

      /** Destroys the object
        */
        virtual ~DescriberBase()
        {
        }

      /** Does the object provide meaningful description via describeTo() ?.
        * @return true: description is available
        */
        virtual bool hasDescription() = 0;

      /** Appends the description of this object to the buffer.
        * @param result    the buffer that the description is appended to.
        * @param matchers  a list of matcher objects
        * @param stub      pointer to the stub
        * @param name      the object's name
        * @return The current content of the buffer data
        */
        virtual String describeTo( String &result,
#ifdef MOCKPP_NO_TYPENAME_FOR_STL_NS
                                   const MOCKPP_STL::vector<InvocationMatcher<I>*> &matchers,
#else
                                   const typename MOCKPP_STL::vector<InvocationMatcher<I>*> &matchers,
#endif
                                   Stub<R, I> *stub,
                                   const String &name ) const = 0;

      /** Appends the description of this object to the buffer.
        * @param result    the buffer that the description is appended to.
        * @param matchers  a list of matcher objects
        * @param stub      pointer to the stub
        * @param name      the object's name
        * @return The current content of the buffer data
        */
        String describeTo( String &result,
#ifdef MOCKPP_NO_TYPENAME_FOR_STL_NS
                           const MOCKPP_STL::vector<InvocationMatcher<I>*> &matchers,
#else
                           const typename MOCKPP_STL::vector<InvocationMatcher<I>*> &matchers,
#endif
                           TypelessStub<R>* stub,
                           const String &name ) const
        {
          typename Stub<R, I>::AP adap = new TypelessStubAdapter<R, I> (stub, false);
          return describeTo(result, matchers, adap.get(), name);
        }
    };

    ////////////////////////////////////////////////////
    ///

  /** This encapsulates knowledge about how the InvocationMockerBuilder class
    * adds matchers to an InvocationMocker.  Therefore it should be considered
    * to be part of the implementation of the InvocationMockerBuilder class.
    */
    class Describer : public DescriberBase
    {
      public:

      /** Does the object provide meaningful description via describeTo() ?.
        * @return true: description is available
        */
        virtual bool hasDescription()
        {
          return true;
        }

      /** Appends the description of this object to the buffer.
        * @param result    the buffer that the description is appended to.
        * @param matchers  a list of matcher objects
        * @param stub      pointer to the stub
        * @param name      the object's name
        * @return The current content of the buffer data
        */
        virtual String describeTo( String &result,
#ifdef MOCKPP_NO_TYPENAME_FOR_STL_NS
                                   const MOCKPP_STL::vector<InvocationMatcher<I>*> &matchers,
#else
                                   const typename MOCKPP_STL::vector<InvocationMatcher<I>*> &matchers,
#endif
                                   Stub<R, I> *stub,
                                   const String &name ) const
        {
          const String SEP = MOCKPP_PCHAR( ", " );
          bool needSeparator = false;

          for ( ConstIterator it = matchers.begin(); it != matchers.end(); ++it )
          {
            InvocationMatcher<I>* invmatcher = *it;

            if ( !invmatcher->hasDescription() )
              continue;

            if ( needSeparator )
              result += SEP;

            invmatcher->describeTo( result );
            needSeparator = true;
          }

          if ( needSeparator )
            result += SEP;

          stub->describeTo( result );

          if ( name.length() != 0 )
            result += MOCKPP_PCHAR( " [" ) + name + MOCKPP_PCHAR( "]" );

          return result;
        }
    };

    ////////////////////////////////////////////////////
    //

  /** Default describer
    */
    class DefaultDescriber : public DescriberBase
    {
      public:

        virtual bool hasDescription()
        {
          return true;
        }

      /** Appends the description of this object to the buffer.
        * @param result    the buffer that the description is appended to.
        * @param matchers  a list of matcher objects
        * @param stub      pointer to the stub
        * @param name      the object's name
        * @return The current content of the buffer data
        */
        String describeTo( String &result,
#ifdef MOCKPP_NO_TYPENAME_FOR_STL_NS
                           const MOCKPP_STL::vector<InvocationMatcher<I>*> &matchers,
#else
                           const typename MOCKPP_STL::vector<InvocationMatcher<I>*> &matchers,
#endif
                           Stub<R, I> *stub,
                           const String &name ) const
        {
          typename InvocationMockerBase<R, I>::ConstIterator it;
          for (it = matchers.begin(); it != matchers.end(); ++it )
          {
            InvocationMatcher<I>* matcher = *it;

            if ( matcher->hasDescription() )
            {
              matcher->describeTo( result );
              result += MOCKPP_PCHAR( ", " );
            }
          }

          stub->describeTo( result );

          if ( name.length() != 0 )
            result += MOCKPP_PCHAR( " [" ) + name + MOCKPP_PCHAR( "]" );
          return result;
        }
    };

  protected:

    List                      matchers;  //!< internal shorthand
    typename Stub<R, I>::AP   stub;      //!< internal shorthand

  private:

    String                      name;
    AutoPointer<DescriberBase>  describer;
};


/** Helper object to mock invocations.
  * @internal
  */
template <typename R,     // Returntype
          typename I>     // Invocation
class InvocationMocker : public InvocationMockerBase<R, I>
{
    typedef typename InvocationMockerBase<R, I>::Iterator Iterator;                //!< internal shorthand
    typedef typename InvocationMockerBase<R, I>::ConstIterator ConstIterator;      //!< internal shorthand

  public:

    typedef typename InvocationMockerBase<R, I>::Describer Describer;                 //!< internal shorthand
    typedef typename InvocationMockerBase<R, I>::DescriberBase DescriberBase;         //!< internal shorthand
    typedef typename InvocationMockerBase<R, I>::DefaultDescriber DefaultDescriber;   //!< internal shorthand

  /** Creates the object
    */
    InvocationMocker()
      : InvocationMockerBase<R, I>( new typename InvocationMockerBase<R, I>::DefaultDescriber(),
                                    new TypelessStubAdapter<R, I>(new DefaultResultStub<R> ))
    {
    }

  /** Creates the object
    * @param  in_describer  pointer to describer object
    */
    InvocationMocker( AutoPointer<DescriberBase> in_describer )
        : InvocationMockerBase<R, I>( in_describer,
                                      new TypelessStubAdapter<R, I>(new DefaultResultStub<R> ))
    {
    }

  /** Mock the invocation
    * @param invocation  invocation data
    * @return return value from mocked invocation
    */
    virtual R invoke( const I &invocation )
    {
      for ( Iterator it = this->matchers.begin(); it != this->matchers.end(); ++it )
        ( *it ) ->incInvoked( invocation );
      return this->stub->invoke( invocation );
    }
};


#ifndef MOCKPP_PTI_WEAKNESS // Partial Template Instantiation Weakness


/** Partial specialisation for helper objects to mock void invocations.
  * @internal
  */
template <typename I>     // Invocation
class InvocationMocker<void, I> : public InvocationMockerBase<void, I>
{
    typedef typename InvocationMockerBase<void, I>::Iterator Iterator;            //!< internal shorthand
    typedef typename InvocationMockerBase<void, I>::ConstIterator ConstIterator;  //!< internal shorthand

  public:

    typedef typename InvocationMockerBase<void, I>::Describer Describer;                //!< internal shorthand
    typedef typename InvocationMockerBase<void, I>::DescriberBase DescriberBase;        //!< internal shorthand
    typedef typename InvocationMockerBase<void, I>::DefaultDescriber DefaultDescriber;  //!< internal shorthand

  /** Creates the object
    */
    InvocationMocker<void, I>()
        : InvocationMockerBase<void, I>( new DefaultDescriber(),
                                         new TypelessStubAdapter<void, I>(new VoidStub))
    {
    }

  /** Creates the object
    * @param  in_describer  pointer to describer object
    */
    InvocationMocker<void, I>( AutoPointer<DescriberBase> in_describer )
        : InvocationMockerBase<void, I>( in_describer,
                                         new TypelessStubAdapter<void, I>(new VoidStub))
    {
    }

  /** Mock the invocation
    * @param invocation  invocation data
    * @return
    */
    virtual void invoke( const I &invocation )
    {
      for ( Iterator it = this->matchers.begin(); it != this->matchers.end(); ++it )
        ( *it ) ->incInvoked( invocation );
      this->stub->invoke( invocation );
    }
};


#define MOCKPP_INVOCATIONMOCKER_PTI_IMPL(I)
#define MOCKPP_INVOCATIONMOCKER_PTI_DECL(I)


#else // MOCKPP_PTI_WEAKNESS Partial Template Instantiation Weakness


#define MOCKPP_INVOCATIONMOCKER_PTI_DECL(I) \
MOCKPP_NS_START \
template<> \
class InvocationMocker<void, I>; \
}


#define MOCKPP_INVOCATIONMOCKER_PTI_IMPL(I) \
MOCKPP_NS_START \
template<> \
class InvocationMocker<void, I> : public InvocationMockerBase<void, I> \
{ \
 public: \
  InvocationMocker<void, I >() \
      : InvocationMockerBase<void, I >( new InvocationMockerBase<void, I >::DefaultDescriber(), \
                                        new TypelessStubAdapter<void, I >(new VoidStub)) \
  {} \
  \
  InvocationMocker<void, I >(\
    AutoPointer<MOCKPP_NS::InvocationMockerBase<void, I >::DescriberBase> in_describer ) \
      : InvocationMockerBase<void, I >( in_describer, \
                                        new TypelessStubAdapter<void, I >(new VoidStub)) \
  {}  \
  \
  void invoke( const I &invocation ) \
  { \
    for ( Iterator it = this->matchers.begin(); it != this->matchers.end(); ++it ) \
      ( *it ) ->incInvoked( invocation ); \
    this->stub->invoke( invocation ); \
  } \
}; \
MOCKPP_NS_END


#endif // MOCKPP_PTI_WEAKNESS


MOCKPP_NS_END

/** @def MOCKPP_INVOCATIONMOCKER_PTI_DECL
  * Declare a specialized class for MOCKPP_NS::InvocationMocker.
  * Neccessary for compilers with weak template capabilities.
  */

/** @def MOCKPP_INVOCATIONMOCKER_PTI_IMPL
  * Implement a specialized class for MOCKPP_NS::InvocationMocker.
  * Neccessary for compilers with weak template capabilities.
  */

#endif // MOCKPP_INVOCATIONMOCKER_H
