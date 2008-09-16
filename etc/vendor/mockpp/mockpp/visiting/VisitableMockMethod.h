/** @file
    @brief  Visitable Mock Methods based on templates

  $Id: VisitableMockMethod.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Oct 2 2005
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


#ifndef MOCKPP_VisitableMockMethod_H
#define MOCKPP_VisitableMockMethod_H

#include <mockpp/mockpp.h>

#include <mockpp/visiting/VisitableMockObject.h>
//#include <mockpp/CountedVisitableMethod.h>


MOCKPP_NS_START


/** Base of all mock methods.
  * @internal
  */
class MOCKPP_API_DECL0 VisitableMockMethodBase : public MockObject
{
  public:

  /** Constructs the mock object.
    * @param name    the method name to mock
    * @param parent  parent Visitable mock object
    */
    VisitableMockMethodBase(const String &name, VisitableMockObject *parent);

  /** Gets the name of the associated method.
  * @param fully  true: return fully qualified name with parent similar to variable access syntax
    * @return method name
    */
    String getMethodName(bool fully = false) const;

  /** Gets the method identifier.
    * @return method identifier
    */
    MOCKPP_STL::string getMethodIdentifier() const;

  /** Gets the parent mock object.
    * @return pointer to mock object
    */
    VisitableMockObject *getVisitableMockObject() const;

  protected:

  /** Throws an exception if available.
    */
    void throwAvailableException() const
    {
      bool do_throw = true;
      if (throwablesInline)
      {
        unsigned num = throwableInsteadReturn.size();
        if (num != 0)
        {
          do_throw = throwableInsteadReturn[0];
          throwableInsteadReturn.erase(throwableInsteadReturn.begin());
        }
      }

      if (do_throw && throwables.hasMoreObjects() != 0)
      {
        Throwable *thr = throwables.nextThrowableObject();
        if (thr != 0)
          thr->throw_me();
      }

      if (defaultThrowable.get() != 0)
        defaultThrowable.get()->throw_me();
    }

    //-------------------------------------------------------------

  public:

  /** Switches exception handling back to pre-1.2 behaviour.
    * @see VisitableMockObject::Controller::unsetThrowablesInline()
    */
    void unsetThrowablesInline()
    {
      MOCKPP_ASSERT_FALSE(this->getVisitableMockObject()->isActivated());
      this->throwablesInline = false;
    };

  /** Adds another throwable to the method.
    * The throwables are thrown one after the other when the method is
    * called after activating until all of them are used.
    * @param  t   a pointer to the throwable
    */
    void addThrowable(Throwable *t)
    {
      MOCKPP_STD_NS::auto_ptr<Throwable> at (t);
      MOCKPP_ASSERT_FALSE(this->getVisitableMockObject()->isActivated());
      this->throwables.push_back(at.release());
      this->throwableInsteadReturn.push_back(true);
    }

  /** Adds another throwable to the method.
    * The throwables are thrown one after the other when the method is
    * called after activating until all of them are used.
    * @param  w      the value to throw
    * @param  count  how often to throw the value
    */
    template <class T>
    void addThrowable(const T &w, unsigned count = 1)
    {
      for ( ; count > 0; --count)
        addThrowable(make_throwable(w));
    }

  /** Sets the default throwable.
    * Once the list of throwables is empty this one is thrown. This also
    * means that your method never processes the parameters passed to it.
    * @param  t   a pointer to the throwable
    */
    void setDefaultThrowable(Throwable *t)
    {
      MOCKPP_STD_NS::auto_ptr<Throwable> at (t);
      MOCKPP_ASSERT_FALSE(this->getVisitableMockObject()->isActivated());
      this->defaultThrowable.take(at.release());
    }

  /** Clears all expectations and puts the object in record mode again.
    */
    virtual void reset()
    {
      this->defaultThrowable.reset();
      this->throwables.reset();
      this->throwableInsteadReturn.clear();
      this->throwablesInline = true;
    }

  /**
    * Verify the conditions which are not in the verify() chain of the
    * visitable mock object. Located here instead of the method for traditional
    * reasons.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void verify()
    {
      Throwable *dt = this->defaultThrowable.get();
      if (dt != 0)
      {
        String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 is unused."));
        fmt << (this->getMethodName() + MOCKPP_PCHAR("/defaultThrowable"));
        MOCKPP_ASSERT_TRUE_MESSAGE(fmt, dt->hasThrown());
      }
    }

  protected:

    VisitableMockObject         *visitable;                     //!< shared internal member.
    mutable ThrowableItem        defaultThrowable;              //!< shared internal member
    mutable ThrowableList        throwables;                    //!< shared internal member
    mutable bool                 throwablesInline;              //!< shared internal member
    mutable MOCKPP_STL::vector<bool>    throwableInsteadReturn; //!< shared internal member
};


//////////////////////////////////////////////////////////////////////////////////////////////


/** Base of all mock methods returning some value.
  * @internal
  */
template <typename R>
class VisitableMockReturningMethodBase : public VisitableMockMethodBase
{
  public:

  /** Constructs the mock object.
    * @param name    the method name to mock
    * @param parent  parent Visitable mock object
    */
    VisitableMockReturningMethodBase(const String &name, VisitableMockObject *parent)
      : VisitableMockMethodBase(name, parent)
      , returnValues(getMethodName() + MOCKPP_PCHAR("/returnValues"), this)
    {
      reset();
    }

  /** Sets the default return value.
    * This value is returned if there are no throwables available and if there
    * are no other possible return values in a list.
    * @param  rv      the default return value
    */
    void setDefaultReturnValue(const R &rv)
    {
      MOCKPP_ASSERT_FALSE(this->getVisitableMockObject()->isActivated());
      this->haveDefaultReturnValue = true;
      this->defaultReturnValue = rv;
    }

  /** Adds another return value.
    * The values from this list are return one after the other unless there
    * are no other throwables or response values available.
    * @param  rv      the next return value
    * @param  count   the number of times this value shall be returned
    */
    void addReturnValue(const R &rv, unsigned count = 1)
    {
      MOCKPP_ASSERT_FALSE(this->getVisitableMockObject()->isActivated());
      for ( ; count > 0; --count)
      {
        this->returnValues.addObjectToReturn(rv);
        this->throwableInsteadReturn.push_back(false);
      }
    }

  /** Sets all internal objects to the state after construction.
    */
    virtual void reset()
    {
      VisitableMockMethodBase::reset();
      this->returnValues.reset();
      this->haveDefaultReturnValue = false;
      this->defaultReturnValueUsed = false;
    }

  /** Verify that the expected and actual values are equal.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void verify()
    {
      VisitableMockMethodBase::verify();
      if (this->haveDefaultReturnValue)
      {
        String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 is unused."));
        fmt << (this->getMethodName() + MOCKPP_PCHAR("/defaultReturnValueUsed"));
        MOCKPP_ASSERT_TRUE_MESSAGE(fmt, this->defaultReturnValueUsed);
      }
    }

  protected:

  /** Determines the current return value.
    * The values from the default value or the list are returned one after
    * the other.
    */
    R determineReturnValue() const
    {
//      throwableInsteadReturn.erase(throwableInsteadReturn.begin());
      if (returnValues.hasMoreObjects())
        return returnValues.nextReturnObject();

      MOCKPP_ASSERT_TRUE_MESSAGE(this->getMethodName(true) + MOCKPP_PCHAR("haveDefaultReturnValue != true"),
                                 haveDefaultReturnValue == true);
      defaultReturnValueUsed = true;
      return defaultReturnValue;
    }

  protected:

    mutable ReturnObjectList<R>         returnValues;              //!< shared internal member
    bool                                haveDefaultReturnValue;    //!< shared internal member
    mutable bool                        defaultReturnValueUsed;    //!< shared internal member
    R                                   defaultReturnValue;        //!< shared internal member
};


/** Base of all mock methods returning void.
  * @internal
  */
template <>
class VisitableMockReturningMethodBase<void> : public VisitableMockMethodBase
{
  public:

  /** Constructs the mock object.
    * @param name    the method name to mock
    * @param parent  parent Visitable mock object
    */
    VisitableMockReturningMethodBase(const String &name, VisitableMockObject *parent)
      : VisitableMockMethodBase(name, parent)
    {}
};


MOCKPP_NS_END


#endif // MOCKPP_VisitableMockMethod_H

