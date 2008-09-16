/** @file
    @brief    Stub returning a defined value and triggering contraints.
              Generated with gen_returntriggerstub_N.pl.

  $Id: ReturnAndTriggerStub.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Aug 01 2006
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

#ifndef MOCKPP_ReturnAndTriggerStub_H
#define MOCKPP_ReturnAndTriggerStub_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/compat/Formatter.h>

#include <mockpp/stub/TypelessStub.h>

#include <mockpp/constraint/TriggeredConstraint.h>


MOCKPP_NS_START


/** Always return the same specified value and trigger some contraint.
  * @ingroup grp_stub
  * @see mockpp::returnValueAndTrigger
  */
template <  typename R     // Returntype
          , typename T1    // Parameter type 1
         >
class ReturnAndTriggerStub1 : public TypelessStub<R>
{
  public:

  /** Creates the object.
    * @param  in_result   the return value
    * @param  in_tc1  pointer to trigger constraint 1
    */
    ReturnAndTriggerStub1(  const R &in_result
                          , TriggeredConstraint<T1> *in_tc1
                         )
      : result(in_result)
      , tc1(in_tc1)
    {}

  /** Mock the invocation without passing parameters.
    * Triggers the according constraints and returns the value.
    * @return return value from mocked invocation
    */
    virtual R typelessInvoke()
    {
      tc1->trigger();
      return result;
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      String fmt;
      {
        String tcfmt;
        tc1->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR("triggers [%1]\n");
        fmt << tcfmt;
      }

      fmt += MOCKPP_PCHAR("returns <%1>");
      fmt << result;
      buffer += fmt;

      return buffer;
    }

  private:

    R                          result;
    TriggeredConstraint<T1>   *tc1;
};


/** Always return the same specified value and trigger some contraint.
  * @ingroup grp_stub
  * @see mockpp::returnValueAndTrigger
  */
template <  typename R     // Returntype
          , typename T1    // Parameter type 1
          , typename T2    // Parameter type 2
         >
class ReturnAndTriggerStub2 : public TypelessStub<R>
{
  public:

  /** Creates the object.
    * @param  in_result   the return value
    * @param  in_tc1  pointer to trigger constraint 1
    * @param  in_tc2  pointer to trigger constraint 2
    */
    ReturnAndTriggerStub2(  const R &in_result
                          , TriggeredConstraint<T1> *in_tc1
                          , TriggeredConstraint<T2> *in_tc2
                         )
      : result(in_result)
      , tc1(in_tc1)
      , tc2(in_tc2)
    {}

  /** Mock the invocation without passing parameters.
    * Triggers the according constraints and returns the value.
    * @return return value from mocked invocation
    */
    virtual R typelessInvoke()
    {
      tc1->trigger();
      tc2->trigger();
      return result;
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      String fmt;
      {
        String tcfmt;
        tc1->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR("triggers [%1]\n");
        fmt << tcfmt;
      }

      {
        String tcfmt;
        tc2->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR("triggers [%1]\n");
        fmt << tcfmt;
      }

      fmt += MOCKPP_PCHAR("returns <%1>");
      fmt << result;
      buffer += fmt;

      return buffer;
    }

  private:

    R                          result;
    TriggeredConstraint<T1>   *tc1;
    TriggeredConstraint<T2>   *tc2;
};


/** Always return the same specified value and trigger some contraint.
  * @ingroup grp_stub
  * @see mockpp::returnValueAndTrigger
  */
template <  typename R     // Returntype
          , typename T1    // Parameter type 1
          , typename T2    // Parameter type 2
          , typename T3    // Parameter type 3
         >
class ReturnAndTriggerStub3 : public TypelessStub<R>
{
  public:

  /** Creates the object.
    * @param  in_result   the return value
    * @param  in_tc1  pointer to trigger constraint 1
    * @param  in_tc2  pointer to trigger constraint 2
    * @param  in_tc3  pointer to trigger constraint 3
    */
    ReturnAndTriggerStub3(  const R &in_result
                          , TriggeredConstraint<T1> *in_tc1
                          , TriggeredConstraint<T2> *in_tc2
                          , TriggeredConstraint<T3> *in_tc3
                         )
      : result(in_result)
      , tc1(in_tc1)
      , tc2(in_tc2)
      , tc3(in_tc3)
    {}

  /** Mock the invocation without passing parameters.
    * Triggers the according constraints and returns the value.
    * @return return value from mocked invocation
    */
    virtual R typelessInvoke()
    {
      tc1->trigger();
      tc2->trigger();
      tc3->trigger();
      return result;
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      String fmt;
      {
        String tcfmt;
        tc1->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR("triggers [%1]\n");
        fmt << tcfmt;
      }

      {
        String tcfmt;
        tc2->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR("triggers [%1]\n");
        fmt << tcfmt;
      }

      {
        String tcfmt;
        tc3->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR("triggers [%1]\n");
        fmt << tcfmt;
      }

      fmt += MOCKPP_PCHAR("returns <%1>");
      fmt << result;
      buffer += fmt;

      return buffer;
    }

  private:

    R                          result;
    TriggeredConstraint<T1>   *tc1;
    TriggeredConstraint<T2>   *tc2;
    TriggeredConstraint<T3>   *tc3;
};


/** Always return the same specified value and trigger some contraint.
  * @ingroup grp_stub
  * @see mockpp::returnValueAndTrigger
  */
template <  typename R     // Returntype
          , typename T1    // Parameter type 1
          , typename T2    // Parameter type 2
          , typename T3    // Parameter type 3
          , typename T4    // Parameter type 4
         >
class ReturnAndTriggerStub4 : public TypelessStub<R>
{
  public:

  /** Creates the object.
    * @param  in_result   the return value
    * @param  in_tc1  pointer to trigger constraint 1
    * @param  in_tc2  pointer to trigger constraint 2
    * @param  in_tc3  pointer to trigger constraint 3
    * @param  in_tc4  pointer to trigger constraint 4
    */
    ReturnAndTriggerStub4(  const R &in_result
                          , TriggeredConstraint<T1> *in_tc1
                          , TriggeredConstraint<T2> *in_tc2
                          , TriggeredConstraint<T3> *in_tc3
                          , TriggeredConstraint<T4> *in_tc4
                         )
      : result(in_result)
      , tc1(in_tc1)
      , tc2(in_tc2)
      , tc3(in_tc3)
      , tc4(in_tc4)
    {}

  /** Mock the invocation without passing parameters.
    * Triggers the according constraints and returns the value.
    * @return return value from mocked invocation
    */
    virtual R typelessInvoke()
    {
      tc1->trigger();
      tc2->trigger();
      tc3->trigger();
      tc4->trigger();
      return result;
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      String fmt;
      {
        String tcfmt;
        tc1->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR("triggers [%1]\n");
        fmt << tcfmt;
      }

      {
        String tcfmt;
        tc2->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR("triggers [%1]\n");
        fmt << tcfmt;
      }

      {
        String tcfmt;
        tc3->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR("triggers [%1]\n");
        fmt << tcfmt;
      }

      {
        String tcfmt;
        tc4->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR("triggers [%1]\n");
        fmt << tcfmt;
      }

      fmt += MOCKPP_PCHAR("returns <%1>");
      fmt << result;
      buffer += fmt;

      return buffer;
    }

  private:

    R                          result;
    TriggeredConstraint<T1>   *tc1;
    TriggeredConstraint<T2>   *tc2;
    TriggeredConstraint<T3>   *tc3;
    TriggeredConstraint<T4>   *tc4;
};


/** Always return the same specified value and trigger some contraint.
  * @ingroup grp_stub
  * @see mockpp::returnValueAndTrigger
  */
template <  typename R     // Returntype
          , typename T1    // Parameter type 1
          , typename T2    // Parameter type 2
          , typename T3    // Parameter type 3
          , typename T4    // Parameter type 4
          , typename T5    // Parameter type 5
         >
class ReturnAndTriggerStub5 : public TypelessStub<R>
{
  public:

  /** Creates the object.
    * @param  in_result   the return value
    * @param  in_tc1  pointer to trigger constraint 1
    * @param  in_tc2  pointer to trigger constraint 2
    * @param  in_tc3  pointer to trigger constraint 3
    * @param  in_tc4  pointer to trigger constraint 4
    * @param  in_tc5  pointer to trigger constraint 5
    */
    ReturnAndTriggerStub5(  const R &in_result
                          , TriggeredConstraint<T1> *in_tc1
                          , TriggeredConstraint<T2> *in_tc2
                          , TriggeredConstraint<T3> *in_tc3
                          , TriggeredConstraint<T4> *in_tc4
                          , TriggeredConstraint<T5> *in_tc5
                         )
      : result(in_result)
      , tc1(in_tc1)
      , tc2(in_tc2)
      , tc3(in_tc3)
      , tc4(in_tc4)
      , tc5(in_tc5)
    {}

  /** Mock the invocation without passing parameters.
    * Triggers the according constraints and returns the value.
    * @return return value from mocked invocation
    */
    virtual R typelessInvoke()
    {
      tc1->trigger();
      tc2->trigger();
      tc3->trigger();
      tc4->trigger();
      tc5->trigger();
      return result;
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      String fmt;
      {
        String tcfmt;
        tc1->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR("triggers [%1]\n");
        fmt << tcfmt;
      }

      {
        String tcfmt;
        tc2->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR("triggers [%1]\n");
        fmt << tcfmt;
      }

      {
        String tcfmt;
        tc3->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR("triggers [%1]\n");
        fmt << tcfmt;
      }

      {
        String tcfmt;
        tc4->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR("triggers [%1]\n");
        fmt << tcfmt;
      }

      {
        String tcfmt;
        tc5->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR("triggers [%1]\n");
        fmt << tcfmt;
      }

      fmt += MOCKPP_PCHAR("returns <%1>");
      fmt << result;
      buffer += fmt;

      return buffer;
    }

  private:

    R                          result;
    TriggeredConstraint<T1>   *tc1;
    TriggeredConstraint<T2>   *tc2;
    TriggeredConstraint<T3>   *tc3;
    TriggeredConstraint<T4>   *tc4;
    TriggeredConstraint<T5>   *tc5;
};


/** Always return the same specified value and trigger some contraint.
  * @ingroup grp_stub
  * @see mockpp::returnValueAndTrigger
  */
template <  typename R     // Returntype
          , typename T1    // Parameter type 1
          , typename T2    // Parameter type 2
          , typename T3    // Parameter type 3
          , typename T4    // Parameter type 4
          , typename T5    // Parameter type 5
          , typename T6    // Parameter type 6
         >
class ReturnAndTriggerStub6 : public TypelessStub<R>
{
  public:

  /** Creates the object.
    * @param  in_result   the return value
    * @param  in_tc1  pointer to trigger constraint 1
    * @param  in_tc2  pointer to trigger constraint 2
    * @param  in_tc3  pointer to trigger constraint 3
    * @param  in_tc4  pointer to trigger constraint 4
    * @param  in_tc5  pointer to trigger constraint 5
    * @param  in_tc6  pointer to trigger constraint 6
    */
    ReturnAndTriggerStub6(  const R &in_result
                          , TriggeredConstraint<T1> *in_tc1
                          , TriggeredConstraint<T2> *in_tc2
                          , TriggeredConstraint<T3> *in_tc3
                          , TriggeredConstraint<T4> *in_tc4
                          , TriggeredConstraint<T5> *in_tc5
                          , TriggeredConstraint<T6> *in_tc6
                         )
      : result(in_result)
      , tc1(in_tc1)
      , tc2(in_tc2)
      , tc3(in_tc3)
      , tc4(in_tc4)
      , tc5(in_tc5)
      , tc6(in_tc6)
    {}

  /** Mock the invocation without passing parameters.
    * Triggers the according constraints and returns the value.
    * @return return value from mocked invocation
    */
    virtual R typelessInvoke()
    {
      tc1->trigger();
      tc2->trigger();
      tc3->trigger();
      tc4->trigger();
      tc5->trigger();
      tc6->trigger();
      return result;
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      String fmt;
      {
        String tcfmt;
        tc1->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR("triggers [%1]\n");
        fmt << tcfmt;
      }

      {
        String tcfmt;
        tc2->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR("triggers [%1]\n");
        fmt << tcfmt;
      }

      {
        String tcfmt;
        tc3->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR("triggers [%1]\n");
        fmt << tcfmt;
      }

      {
        String tcfmt;
        tc4->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR("triggers [%1]\n");
        fmt << tcfmt;
      }

      {
        String tcfmt;
        tc5->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR("triggers [%1]\n");
        fmt << tcfmt;
      }

      {
        String tcfmt;
        tc6->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR("triggers [%1]\n");
        fmt << tcfmt;
      }

      fmt += MOCKPP_PCHAR("returns <%1>");
      fmt << result;
      buffer += fmt;

      return buffer;
    }

  private:

    R                          result;
    TriggeredConstraint<T1>   *tc1;
    TriggeredConstraint<T2>   *tc2;
    TriggeredConstraint<T3>   *tc3;
    TriggeredConstraint<T4>   *tc4;
    TriggeredConstraint<T5>   *tc5;
    TriggeredConstraint<T6>   *tc6;
};


MOCKPP_NS_END


#endif // MOCKPP_ReturnAndTriggerStub_H

