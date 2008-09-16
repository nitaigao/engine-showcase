/** @file
    @internal
    @brief  Base class for typeless stubs

  $Id: TypelessStub.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

 **/

#ifndef MOCKPP_TypelessStub_H
#define MOCKPP_TypelessStub_H


#include <mockpp/mockpp.h> // always first

#include <mockpp/stub/Stub.h>

#include <mockpp/SelfDescribing.h>


MOCKPP_NS_START


/** An object that stubs the behaviour of an invoked method invocation on behalf of an
  * object. The return value never depends on the parameters passed to the object.
  * @internal
  */
template <typename R>  // Returntype
class TypelessStub : public SelfDescribing
{
  public:

    typedef AutoPointer<TypelessStub<R> > AP;  //!< internal shared member

  /** Destroys the object.
    */
    virtual ~TypelessStub()
    {}

  /** Mock the invocation without passing parameters.
    * @return return value from mocked invocation
    */
    virtual R typelessInvoke() = 0;

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The buffer passed to the object.
    */
    virtual String describeTo( String &buffer ) const = 0;
};


/** Base of adapters for the "translation" to typeless stub objects.
  * @internal
  */
template <typename R,  // Returntype
          typename I>  // Invocation
class TypelessStubAdapterBase : public Stub<R, I>
{
  protected:

  /** Creates the object.
    * @param  tm           pointer to stub object
    * @param  in_delete    true: delete stub when the object is destroyed
    */
    TypelessStubAdapterBase(typename TypelessStub<R>::AP tm, bool in_delete)
      : stubber(tm.release())
      , do_delete(in_delete)
    {}

  /** Destroys the object.
    */
    virtual ~TypelessStubAdapterBase()
    {
      if (do_delete)
        delete stubber;
      stubber = 0;
    }

  public:

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The buffer passed to the object.
    */
    virtual String describeTo( String &buffer ) const
    {
      return stubber->describeTo(buffer);
    }

  protected:

    TypelessStub<R> *stubber;        //!< internal shared member
    bool             do_delete;      //!< internal shared member
};


/** Adapter for the "translation" to typeless stub objects.
  * @internal
  */
template <typename R,  // Returntype
          typename I>  // Invocation
class TypelessStubAdapter : public TypelessStubAdapterBase<R, I>
{
  public:

  /** Creates the object.
    * @param  tm           pointer to stub object
    * @param  in_delete    true: delete stub when the object is destroyed
    */
    TypelessStubAdapter(typename TypelessStub<R>::AP tm, bool in_delete = true)
      : TypelessStubAdapterBase<R, I>(tm, in_delete)
    {}

  /** Mock the invocation
    * @param invocation  invocation data
    * @return return value from mocked invocation
    */
    virtual R invoke( const I &invocation )
    {
      MOCKPP_UNUSED(invocation);
      return this->stubber->typelessInvoke();
    }
};


#ifndef MOCKPP_PTI_WEAKNESS // Partial Template Instantiation Weakness


/** Partial specialisation for base of adapters for the "translation" to
  * typeless void stub objects.
  * @internal
  */
template <typename I>  // Invocation
class TypelessStubAdapter<void, I> : public TypelessStubAdapterBase<void, I>
{
  public:

  /** Creates the object.
    * @param  tm           pointer to stub object
    * @param  in_delete    true: delete stub when the object is destroyed
    */
    TypelessStubAdapter(typename TypelessStub<void>::AP tm, bool in_delete = true)
      : TypelessStubAdapterBase<void, I>(tm, in_delete)
    {}

  /** Mock the invocation
    * @param invocation  invocation data
    */
    virtual void invoke( const I &invocation )
    {
      MOCKPP_UNUSED(invocation);
      this->stubber->typelessInvoke();
    }
};


#define MOCKPP_TYPELESSSTUBADAPTER_PTI_IMPL(I)
#define MOCKPP_TYPELESSSTUBADAPTER_PTI_DECL(I)


#else // MOCKPP_PTI_WEAKNESS Partial Template Instantiation Weakness


#define MOCKPP_TYPELESSSTUBADAPTER_PTI_DECL(I) \
MOCKPP_NS_START \
template<> \
void MOCKPP_NS::TypelessStubAdapter<void, I >; \
}


#define MOCKPP_TYPELESSSTUBADAPTER_PTI_IMPL(I) \
MOCKPP_NS_START \
template<> \
class TypelessStubAdapter<void, I > : public TypelessStubAdapterBase<void, I > \
{ \
 public: \
  TypelessStubAdapter<void, I >(TypelessStub<void>::AP tm, bool in_delete = true) \
   : TypelessStubAdapterBase<void, I >(tm, in_delete) \
  {} \
  \
  void invoke( const I &invocation) \
  { \
   MOCKPP_UNUSED(invocation); \
   this->stubber->typelessInvoke(); \
  } \
}; \
MOCKPP_NS_END


#endif // MOCKPP_PTI_WEAKNESS

/** @def MOCKPP_TYPELESSSTUBADAPTER_PTI_DECL
  * Declare a specialized class for MOCKPP_NS::InvocationMocker.
  * Neccessary for compilers with weak template capabilities.
  */

/** @def MOCKPP_TYPELESSSTUBADAPTER_PTI_IMPL
  * Implement a specialized class for MOCKPP_NS::InvocationMocker.
  * Neccessary for compilers with weak template capabilities.
  */


MOCKPP_NS_END


#endif // MOCKPP_TypelessStub_H

