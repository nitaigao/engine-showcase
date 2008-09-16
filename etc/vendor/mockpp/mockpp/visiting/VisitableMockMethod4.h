/** @file
    @brief  Visitable Mock Method with 4 parameters.
            Generated with gen_visitablemethod_N.pl.

  $Id: VisitableMockMethod4.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Oct 22 2005
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

#ifndef MOCKPP_VisitableMockMethod4_H
#define MOCKPP_VisitableMockMethod4_H

#include <mockpp/mockpp.h>

#include <mockpp/visiting/VisitableMockObject.h>
#include <mockpp/visiting/VisitableMockMethod.h>
#include <mockpp/visiting/ResponseVector4.h>


MOCKPP_NS_START


/** Common stuff to set up visitable mock method expectations with 4 parameters.
  * @ingroup grp_controller
  */
template <typename R, typename P1, typename P2, typename P3, typename P4>
class VisitableMockMethod4Common : public VisitableMockReturningMethodBase<R>
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent Visitable mock object
    */
    VisitableMockMethod4Common(const String &name, VisitableMockObject *parent)
      : VisitableMockReturningMethodBase<R>(name, parent)
      , responseThrowables(this->getMethodName() + MOCKPP_PCHAR("/responseThrowables") , this)
      , parameter1(this->getMethodName() + MOCKPP_PCHAR("/parameter1"), this)
      , parameter2(this->getMethodName() + MOCKPP_PCHAR("/parameter2"), this)
      , parameter3(this->getMethodName() + MOCKPP_PCHAR("/parameter3"), this)
      , parameter4(this->getMethodName() + MOCKPP_PCHAR("/parameter4"), this)
    {
    }


  /** Set up expectations with constraints.
    * @param p1 mock method parameter 1
    * @param p2 mock method parameter 2
    * @param p3 mock method parameter 3
    * @param p4 mock method parameter 4
    */
    void forward (const ConstraintHolder<P1> &p1, const ConstraintHolder<P2> &p2, const ConstraintHolder<P3> &p3, const ConstraintHolder<P4> &p4) const
    {
      MOCKPP_ASSERT_FALSE_MESSAGE(this->getVisitableMockObject()->getVerifiableName() + MOCKPP_PCHAR(".isActivated() != true"),
                                  this->getVisitableMockObject()->isActivated());
      this->getVisitableMockObject()->addExpectedMethod(this->getMethodIdentifier());
      parameter1.addExpected(p1);
      parameter2.addExpected(p2);
      parameter3.addExpected(p3);
      parameter4.addExpected(p4);
    }

  protected:

  /** Perform the internals to verify a mocked method or setup expectations.
    * @param p1 mock method parameter 1
    * @param p2 mock method parameter 2
    * @param p3 mock method parameter 3
    * @param p4 mock method parameter 4
    */
    void forward_param(const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4) const
    {
      if (!this->getVisitableMockObject()->isActivated() )
      {
        this->getVisitableMockObject()->addExpectedMethod(this->getMethodIdentifier());
        parameter1.addExpected(p1);
        parameter2.addExpected(p2);
        parameter3.addExpected(p3);
        parameter4.addExpected(p4);
      }

      else
      {
        try
        {
          this->getVisitableMockObject()->addActualMethod(this->getMethodIdentifier());

          Throwable *t;
          if (this->responseThrowables.find(t, p1, p2, p3, p4))
             t->throw_me();

          this->throwAvailableException();
        }

        catch(...)
        {
          parameter1.balanceActual();
          parameter2.balanceActual();
          parameter3.balanceActual();
          parameter4.balanceActual();
          MOCKPP_RETHROW;
        }

        parameter1.addActual(p1);
        parameter2.addActual(p2);
        parameter3.addActual(p3);
        parameter4.addActual(p4);
      }
    }

  public:

  /** Adds another response throwable.
    * Response values are determined on the parameters you pass. This way the
    * object returns a value that is totally based on the input.
    * @param t       the throwable object
    * @param p1 mock method parameter 1
    * @param p2 mock method parameter 2
    * @param p3 mock method parameter 3
    * @param p4 mock method parameter 4
    * @param count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseThrowable(Throwable *t, const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, unsigned count = MOCKPP_UNLIMITED)
    {
      MOCKPP_ASSERT_FALSE(this->getVisitableMockObject()->isActivated());
      this->responseThrowables.add(t, p1, p2, p3, p4, count);
    }

  /** Adds another response throwable.
    * Response values are determined on the parameters you pass. This way the
    * object returns a value that is totally based on the input.
    * @param t       the throwable object
    * @param p1 mock method parameter 1
    * @param p2 mock method parameter 2
    * @param p3 mock method parameter 3
    * @param p4 mock method parameter 4
    * @param count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseThrowable(Throwable *t, const ConstraintHolder<P1> &p1, const ConstraintHolder<P2> &p2, const ConstraintHolder<P3> &p3, const ConstraintHolder<P4> &p4, unsigned count = MOCKPP_UNLIMITED)
    {
      MOCKPP_ASSERT_FALSE(this->getVisitableMockObject()->isActivated());
      this->responseThrowables.add(t, p1, p2, p3, p4, count);
    }

  private:

    mutable ResponseThrowableVector4<P1, P2, P3, P4>   responseThrowables;

    mutable ConstraintList<P1>             parameter1;
    mutable ConstraintList<P2>             parameter2;
    mutable ConstraintList<P3>             parameter3;
    mutable ConstraintList<P4>             parameter4;
};


/** Set up visitable mock method expectations with 4 parameters.
  * @ingroup grp_controller
  */
template <typename R, typename P1, typename P2, typename P3, typename P4>
class VisitableMockMethod4
  : public VisitableMockMethod4Common<R, P1, P2, P3, P4>
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent Visitable mock object
    */
    VisitableMockMethod4(const String &name, VisitableMockObject *parent)
      : VisitableMockMethod4Common<R, P1, P2, P3, P4>(name, parent)
      , responseValues(this->getMethodName() + MOCKPP_PCHAR("/responseValues") , this)
    {
    }

  /** Actually verifies the mocked method.
    * Must be called by the client code.
    * @param p1 mock method parameter 1
    * @param p2 mock method parameter 2
    * @param p3 mock method parameter 3
    * @param p4 mock method parameter 4
    */
    R forward(const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4) const
    {
      this->forward_param(p1, p2, p3, p4);

      if (this->getVisitableMockObject()->isActivated() )
      {
        R ret_val;
        if (this->responseValues.find(ret_val, p1, p2, p3, p4))
          return ret_val;

        return this->determineReturnValue();
      }
      else
        return R(); // only dummy value to make the compiler happy
    }

#if defined(__BORLANDC__) // ==> BCB5.5.1 ?? F1004 Internal compiler error at 0x12548c1 with base 0x1200000
  /** Set up expectations with constraints.
    * @param p1 mock method parameter 1
    * @param p2 mock method parameter 2
    * @param p3 mock method parameter 3
    * @param p4 mock method parameter 4
    */
    void forward (const ConstraintHolder<P1> &p1, const ConstraintHolder<P2> &p2, const ConstraintHolder<P3> &p3, const ConstraintHolder<P4> &p4) const
    {
       VisitableMockMethod4Common<R, P1, P2, P3, P4>::forward(p1, p2, p3, p4);
    }
#else
    using VisitableMockMethod4Common<R, P1, P2, P3, P4>::forward;
#endif

  /** Adds another response value.
    * Response values are determined on the parameters you pass. This way the
    * object returns a value that is totally based on the input.
    * @param rv      the return value
    * @param p1 mock method parameter 1
    * @param p2 mock method parameter 2
    * @param p3 mock method parameter 3
    * @param p4 mock method parameter 4
    * @param count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseValue(const R &rv, const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, unsigned count = MOCKPP_UNLIMITED)
    {
      MOCKPP_ASSERT_FALSE(this->getVisitableMockObject()->isActivated());
      this->responseValues.add(rv, p1, p2, p3, p4, count);
    }

  /** Adds another response value.
    * Response values are determined on the parameters you pass. This way the
    * object returns a value that is totally based on the input.
    * @param rv      the return value
    * @param p1 mock method parameter 1
    * @param p2 mock method parameter 2
    * @param p3 mock method parameter 3
    * @param p4 mock method parameter 4
    * @param count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseValue(const R &rv, const ConstraintHolder<P1> &p1, const ConstraintHolder<P2> &p2, const ConstraintHolder<P3> &p3, const ConstraintHolder<P4> &p4, unsigned count = MOCKPP_UNLIMITED)
    {
      MOCKPP_ASSERT_FALSE(this->getVisitableMockObject()->isActivated());
      this->responseValues.add(rv, p1, p2, p3, p4, count);
    }

  private:

    mutable ResponseVector4<R, P1, P2, P3, P4>   responseValues;
};


/** Set up visitable mock method expectations with 4 parameters.
  * Partial specialisation for a void return value.
  * @ingroup grp_controller
  */
template <typename P1, typename P2, typename P3, typename P4>
class VisitableMockMethod4<void, P1, P2, P3, P4>
   : public VisitableMockMethod4Common<void, P1, P2, P3, P4>
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent Visitable mock object
    */
    VisitableMockMethod4(const String &name, VisitableMockObject *parent)
      : VisitableMockMethod4Common<void, P1, P2, P3, P4>(name, parent)
    {
    }

  /** Actually verifies the mocked method.
    * Must be called by the client code.
    * @param p1 mock method parameter 1
    * @param p2 mock method parameter 2
    * @param p3 mock method parameter 3
    * @param p4 mock method parameter 4
    */
    void forward(const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4) const
    {
       this->forward_param(p1, p2, p3, p4);
    }

#if defined(__BORLANDC__) // ==> BCB5.5.1 ?? F1004 Internal compiler error at 0x12548c1 with base 0x1200000
  /** Set up expectations with constraints.
    * @param p1 mock method parameter 1
    * @param p2 mock method parameter 2
    * @param p3 mock method parameter 3
    * @param p4 mock method parameter 4
    */
    void forward (const ConstraintHolder<P1> &p1, const ConstraintHolder<P2> &p2, const ConstraintHolder<P3> &p3, const ConstraintHolder<P4> &p4) const
    {
       VisitableMockMethod4Common<void, P1, P2, P3, P4>::forward(p1, p2, p3, p4);
    }
#else
    using VisitableMockMethod4Common<void, P1, P2, P3, P4>::forward;
#endif
};


MOCKPP_NS_END


#endif // MOCKPP_VisitableMockMethod4_H

