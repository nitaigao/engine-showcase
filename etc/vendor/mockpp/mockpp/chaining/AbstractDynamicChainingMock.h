/** @file
    @internal
    @brief  Abstract base class for chaining mock objects

$Id: AbstractDynamicChainingMock.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_ABSTRACTDYNAMICCHAININGMOCK_H
#define MOCKPP_ABSTRACTDYNAMICCHAININGMOCK_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/chaining/DynamicChainingMock.h>
#include <mockpp/chaining/DynamicChainingMockError.h>
#include <mockpp/chaining/InvocationMocker.h>
#include <mockpp/chaining/FIFOInvocationDispatcher.h>

#include <mockpp/stub/ReturnStub.h>

#include <mockpp/compat/Exception.h>


MOCKPP_NS_START


/** Implementation of a chaining mock.
  * @internal
  */
template <typename R,    // Returntype
          typename I>    // Invocation
class AbstractDynamicChainingMockImpl : public DynamicChainingMock<R, I>
{
  protected:

  /** Constructs the chaining mock
    * @param  in_invocationDispatcher pointer to invocation dispatcher
    * @param  name                    objects name
    * @param  parent                  pointer to parent verifiable
    */
    AbstractDynamicChainingMockImpl( typename InvocationDispatcher<R, I>::AP in_invocationDispatcher,
                                     const String &name,
                                     VerifiableList *parent)
        : DynamicChainingMock<R, I>(name, parent)
        , invocationDispatcher( in_invocationDispatcher )
    {}

  public:

    class Describer : public InvocationMocker<R, I>::DescriberBase
    {
      public:

      /** Does the object provide meaningful description via describeTo() ?.
        * @return true: description is available
        */
        virtual bool hasDescription()
        {
          return false;
        }


      /** Appends the description of this object to the buffer.
        * @param result   The buffer that the description is appended to.
        * @param matchers list of matcher objects
        * @param stub     pointer to the stub
        * @param name     name of the object
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
          return result;
        }

#if defined( __BORLANDC__ ) || defined( _MSC_VER )

      // also prevent internal compiler error for BCB5

      /** Appends the description of this object to the buffer.
        * @param result    the buffer that the description is appended to.
        * @param matchers  a list of matcher objects
        * @param stub      pointer to the stub
        * @param name      the object's name
        * @return The current content of the buffer data
        */
        String describeTo( String &result,
                           const typename MOCKPP_STL::vector<InvocationMatcher<I>*> &matchers,
                           TypelessStub<R>* stub,
                           const String &name ) const
        {
          return InvocationMocker<R, I>::DescriberBase::describeTo(result, matchers, adap.get(), name);
        }

#else
        using InvocationMocker<R, I>::DescriberBase::describeTo;
#endif

    };

  /** Verify that the expected value is within the allowed bounds.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void verify()
    {
      forgetFailure();

      MOCKPP_TRY
      {
        invocationDispatcher->verify();
      }

#ifndef MOCKPP_NO_EXCEPTIONS
      catch ( const AssertionFailedError &ex )
      {
        assertionFailed( __LINE__, __FILE__,
                                    MOCKPP_PCHAR( "mock object " )
                                    + getMockName()
                                    + MOCKPP_PCHAR( ": " )
                                    + ex.getMessage() );
      }
#endif // MOCKPP_NO_EXCEPTIONS
    }

  /** Transform the object's state to a human readable string.
    * @return string representation
    */
    virtual String toString() const
    {
      return this->getVerifiableName();
    }

  /** Gets the mock object's name
    * @return name of object
    */
    String getMockName() const
    {
      return this->getVerifiableName();
    }

  /** Sets the default stub for the object
    * @param newDefaultStub  pointer to the default stub
    */
    virtual void setDefaultStub( const MOCKPP_NS::StubHolder<R, I> &newDefaultStub )
    {
      invocationDispatcher->setDefaultStub( newDefaultStub );
    }

  /** Adds an invokable to the mock object
    * @param invokable  pointer to the invocable
    */
    virtual void addInvokable( typename Invokable<R, I>::AP invokable )
    {
      invocationDispatcher->add( invokable );
    }

  /** Clears all internal states
    */
    virtual void reset()
    {
      invocationDispatcher->reset();
      forgetFailure();
    }

  protected:

  /** Mocks an invocation
    * @param  invocation   the invocation data
    * @return  the mocked return value
    */
    virtual R mockInvocation( const I &invocation ) = 0;

  private:

  /** Removes the previous failure
    */
    void forgetFailure() const
    {
      failure.reset( 0 );
    }

  protected:

    mutable AutoPointer<AssertionFailedError>  failure;               //!< internal shared member
    typename InvocationDispatcher<R, I>::AP    invocationDispatcher;  //!< internal shared member
};


/** General template of a chaining mock.
  * @internal
  */
template <typename R,    // Returntype
          typename I>    // Invocation
class AbstractDynamicChainingMock : public AbstractDynamicChainingMockImpl<R, I>
{
  public:

  /** Constructs the chaining mock
    * @param  in_invocationDispatcher pointer to invocation dispatcher
    * @param  name                    pointer to invocation dispatcher
    * @param  parent                  pointer to parent verifiable
    */
    AbstractDynamicChainingMock( typename InvocationDispatcher<R, I>::AP in_invocationDispatcher,
                                 const String &name,
                                 VerifiableList *parent)
      : AbstractDynamicChainingMockImpl<R,I>(in_invocationDispatcher, name, parent)
    {}

  /** Constructs the chaining mock
    * @param  name                 pointer to invocation dispatcher
    * @param  parent               pointer to parent verifiable
    */
    AbstractDynamicChainingMock( const String &name,
                                 VerifiableList *parent)
      : AbstractDynamicChainingMockImpl<R,I>(new FIFOInvocationDispatcher<R,I>, name, parent)
    {}

  protected:

  /** Mocks an invocation
    * @param  invocation   the invocation data
    * @return  the mocked return value
    */
    virtual R mockInvocation( const I &invocation )
    {
      if ( this->failure.get() != 0 )
        assertionFailed(this->failure->getSrcLine(),
                        this->failure->getSrcFile(),
                        this->failure->getMessage());

      MOCKPP_TRY
      {
        return this->invocationDispatcher->dispatch( invocation );
      }

#ifndef MOCKPP_NO_EXCEPTIONS
      catch ( const AssertionFailedError & assertion )
      {
        DynamicChainingMockError<R, I> dme( this,
                                            invocation,
                                            this->invocationDispatcher.get(),
                                            assertion.getMessage() );
        this->failure.reset( new AssertionFailedError(__LINE__, __FILE__, dme.getMessage()));
        assertionFailed(this->failure->getSrcLine(),
                        this->failure->getSrcFile(),
                        this->failure->getMessage());
      }
      return R();
#endif // MOCKPP_NO_EXCEPTIONS
    }
};


#ifndef MOCKPP_PTI_WEAKNESS // Partial Template Instantiation Weakness


/** Partial specialisation of a chaining mock for void methods.
  * @internal
  */
template <typename I>    // Invocation
class AbstractDynamicChainingMock<void, I> : public AbstractDynamicChainingMockImpl<void, I>
{
  public:

  /** Constructs the chaining mock
    * @param  in_invocationDispatcher pointer to invocation dispatcher
    * @param  name                    pointer to invocation dispatcher
    * @param  parent                  pointer to parent verifiable
    */
    AbstractDynamicChainingMock( typename InvocationDispatcher<void, I>::AP in_invocationDispatcher,
                                 const String &name,
                                 VerifiableList *parent)
      : AbstractDynamicChainingMockImpl<void, I>(in_invocationDispatcher, name, parent)
    {}

  /** Constructs the chaining mock
    * @param  name                 pointer to invocation dispatcher
    * @param  parent               pointer to parent verifiable
    */
    AbstractDynamicChainingMock( const String &name,
                                 VerifiableList *parent)
      : AbstractDynamicChainingMockImpl<void, I>(new FIFOInvocationDispatcher<void,I>, name, parent)
    {}

  protected:

  /** Mocks an invocation
    * @param  invocation   the invocation data
    */
    virtual void mockInvocation( const I &invocation )
    {
      if ( this->failure.get() != 0 )
        MOCKPP_THROW(*this->failure.get());

      MOCKPP_TRY
      {
        this->invocationDispatcher->dispatch( invocation );
      }

#ifndef MOCKPP_NO_EXCEPTIONS
      catch ( const AssertionFailedError & assertion )
      {
        DynamicChainingMockError<void, I> dme( this,
                                               invocation,
                                               this->invocationDispatcher.get(),
                                               assertion.getMessage() );
        this->failure.reset( new AssertionFailedError(__LINE__, __FILE__, dme.getMessage()));
        MOCKPP_THROW(*this->failure.get());
      }
#endif // MOCKPP_NO_EXCEPTIONS
    }
};


#define MOCKPP_ABSTRACTDYNAMICCHAININGMOCK_PTI_IMPL(I)
#define MOCKPP_ABSTRACTDYNAMICCHAININGMOCK_PTI_DECL(I)


#else // MOCKPP_PTI_WEAKNESS Partial Template Instantiation Weakness


#define MOCKPP_ABSTRACTDYNAMICCHAININGMOCK_PTI_DECL(I) \
MOCKPP_NS_START \
template<> \
void MOCKPP_NS::AbstractDynamicChainingMock<void, I >; \
}


#define MOCKPP_ABSTRACTDYNAMICCHAININGMOCK_PTI_IMPL(I) \
MOCKPP_NS_START \
template<> \
class AbstractDynamicChainingMock<void, I> : public AbstractDynamicChainingMockImpl<void, I> \
{ \
 public: \
  AbstractDynamicChainingMock( InvocationDispatcher<void, I>::AP in_invocationDispatcher, \
                               const String &name, \
                               VerifiableList *parent) \
    : AbstractDynamicChainingMockImpl<void, I>(in_invocationDispatcher, name, parent) \
  {} \
  \
  AbstractDynamicChainingMock( const String &name, \
                               VerifiableList *parent) \
    : AbstractDynamicChainingMockImpl<void, I>(new FIFOInvocationDispatcher<void,I>, name, parent) \
  {} \
  \
  void mockInvocation( const I &invocation ) \
  { \
    if ( this->failure.get() != 0 ) \
      MOCKPP_THROW(*this->failure.get()); \
  \
    try \
    { \
      this->invocationDispatcher->dispatch( invocation ); \
    } \
  \
    catch ( const AssertionFailedError & assertion ) \
    { \
      DynamicChainingMockError<void, I > dme( this, \
                                            invocation, \
                                            this->invocationDispatcher.get(), \
                                            assertion.getMessage() ); \
      this->failure.reset( new AssertionFailedError(__LINE__, __FILE__, dme.getMessage())); \
      MOCKPP_THROW(*this->failure.get()); \
    } \
  } \
}; \
MOCKPP_NS_END


#endif


/** @def MOCKPP_ABSTRACTDYNAMICCHAININGMOCK_PTI_DECL
  * Declare a specialized class for MOCKPP_NS::AbstractDynamicChainingMock.
  * Neccessary for compilers with weak template capabilities.
  */

/** @def MOCKPP_ABSTRACTDYNAMICCHAININGMOCK_PTI_IMPL
  * Implement a specialized class for MOCKPP_NS::AbstractDynamicChainingMock.
  * Neccessary for compilers with weak template capabilities.
  */


MOCKPP_NS_END


#endif // MOCKPP_ABSTRACTDYNAMICCHAININGMOCK_H
