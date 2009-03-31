/** @file
    @brief    Stub triggering contraints.
              Generated with gen_triggerstub_N.pl.

  $Id: TriggerStub.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Aug 03 2006
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

#ifndef MOCKPP_TriggerStub_H
#define MOCKPP_TriggerStub_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/compat/Formatter.h>

#include <mockpp/stub/TypelessStub.h>

#include <mockpp/constraint/TriggeredConstraint.h>


MOCKPP_NS_START


/** Trigger some contraint.
  * @ingroup grp_stub
  * @see mockpp::trigger
  */
template <  typename T1    // Parameter type 1
         >
class TriggerStub1 : public TypelessStub<void>
{
  public:

  /** Creates the object.
    * @param  in_tc1  pointer to trigger constraint 1
    */
    TriggerStub1(  TriggeredConstraint<T1> *in_tc1
                )
      : tc1(in_tc1)
    {}

  /** Mock the invocation without passing parameters.
    * Triggers the according constraints.
    */
    virtual void typelessInvoke()
    {
      tc1->trigger();
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

      fmt += MOCKPP_PCHAR("is void");
      buffer += fmt;

      return buffer;
    }

  private:

    TriggeredConstraint<T1>   *tc1;
};


/** Trigger some contraint.
  * @ingroup grp_stub
  * @see mockpp::trigger
  */
template <  typename T1    // Parameter type 1
          , typename T2    // Parameter type 2
         >
class TriggerStub2 : public TypelessStub<void>
{
  public:

  /** Creates the object.
    * @param  in_tc1  pointer to trigger constraint 1
    * @param  in_tc2  pointer to trigger constraint 2
    */
    TriggerStub2(  TriggeredConstraint<T1> *in_tc1
                 , TriggeredConstraint<T2> *in_tc2
                )
      : tc1(in_tc1)
      , tc2(in_tc2)
    {}

  /** Mock the invocation without passing parameters.
    * Triggers the according constraints.
    */
    virtual void typelessInvoke()
    {
      tc1->trigger();
      tc2->trigger();
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

      fmt += MOCKPP_PCHAR("is void");
      buffer += fmt;

      return buffer;
    }

  private:

    TriggeredConstraint<T1>   *tc1;
    TriggeredConstraint<T2>   *tc2;
};


/** Trigger some contraint.
  * @ingroup grp_stub
  * @see mockpp::trigger
  */
template <  typename T1    // Parameter type 1
          , typename T2    // Parameter type 2
          , typename T3    // Parameter type 3
         >
class TriggerStub3 : public TypelessStub<void>
{
  public:

  /** Creates the object.
    * @param  in_tc1  pointer to trigger constraint 1
    * @param  in_tc2  pointer to trigger constraint 2
    * @param  in_tc3  pointer to trigger constraint 3
    */
    TriggerStub3(  TriggeredConstraint<T1> *in_tc1
                 , TriggeredConstraint<T2> *in_tc2
                 , TriggeredConstraint<T3> *in_tc3
                )
      : tc1(in_tc1)
      , tc2(in_tc2)
      , tc3(in_tc3)
    {}

  /** Mock the invocation without passing parameters.
    * Triggers the according constraints.
    */
    virtual void typelessInvoke()
    {
      tc1->trigger();
      tc2->trigger();
      tc3->trigger();
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

      fmt += MOCKPP_PCHAR("is void");
      buffer += fmt;

      return buffer;
    }

  private:

    TriggeredConstraint<T1>   *tc1;
    TriggeredConstraint<T2>   *tc2;
    TriggeredConstraint<T3>   *tc3;
};


/** Trigger some contraint.
  * @ingroup grp_stub
  * @see mockpp::trigger
  */
template <  typename T1    // Parameter type 1
          , typename T2    // Parameter type 2
          , typename T3    // Parameter type 3
          , typename T4    // Parameter type 4
         >
class TriggerStub4 : public TypelessStub<void>
{
  public:

  /** Creates the object.
    * @param  in_tc1  pointer to trigger constraint 1
    * @param  in_tc2  pointer to trigger constraint 2
    * @param  in_tc3  pointer to trigger constraint 3
    * @param  in_tc4  pointer to trigger constraint 4
    */
    TriggerStub4(  TriggeredConstraint<T1> *in_tc1
                 , TriggeredConstraint<T2> *in_tc2
                 , TriggeredConstraint<T3> *in_tc3
                 , TriggeredConstraint<T4> *in_tc4
                )
      : tc1(in_tc1)
      , tc2(in_tc2)
      , tc3(in_tc3)
      , tc4(in_tc4)
    {}

  /** Mock the invocation without passing parameters.
    * Triggers the according constraints.
    */
    virtual void typelessInvoke()
    {
      tc1->trigger();
      tc2->trigger();
      tc3->trigger();
      tc4->trigger();
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

      fmt += MOCKPP_PCHAR("is void");
      buffer += fmt;

      return buffer;
    }

  private:

    TriggeredConstraint<T1>   *tc1;
    TriggeredConstraint<T2>   *tc2;
    TriggeredConstraint<T3>   *tc3;
    TriggeredConstraint<T4>   *tc4;
};


/** Trigger some contraint.
  * @ingroup grp_stub
  * @see mockpp::trigger
  */
template <  typename T1    // Parameter type 1
          , typename T2    // Parameter type 2
          , typename T3    // Parameter type 3
          , typename T4    // Parameter type 4
          , typename T5    // Parameter type 5
         >
class TriggerStub5 : public TypelessStub<void>
{
  public:

  /** Creates the object.
    * @param  in_tc1  pointer to trigger constraint 1
    * @param  in_tc2  pointer to trigger constraint 2
    * @param  in_tc3  pointer to trigger constraint 3
    * @param  in_tc4  pointer to trigger constraint 4
    * @param  in_tc5  pointer to trigger constraint 5
    */
    TriggerStub5(  TriggeredConstraint<T1> *in_tc1
                 , TriggeredConstraint<T2> *in_tc2
                 , TriggeredConstraint<T3> *in_tc3
                 , TriggeredConstraint<T4> *in_tc4
                 , TriggeredConstraint<T5> *in_tc5
                )
      : tc1(in_tc1)
      , tc2(in_tc2)
      , tc3(in_tc3)
      , tc4(in_tc4)
      , tc5(in_tc5)
    {}

  /** Mock the invocation without passing parameters.
    * Triggers the according constraints.
    */
    virtual void typelessInvoke()
    {
      tc1->trigger();
      tc2->trigger();
      tc3->trigger();
      tc4->trigger();
      tc5->trigger();
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

      fmt += MOCKPP_PCHAR("is void");
      buffer += fmt;

      return buffer;
    }

  private:

    TriggeredConstraint<T1>   *tc1;
    TriggeredConstraint<T2>   *tc2;
    TriggeredConstraint<T3>   *tc3;
    TriggeredConstraint<T4>   *tc4;
    TriggeredConstraint<T5>   *tc5;
};


/** Trigger some contraint.
  * @ingroup grp_stub
  * @see mockpp::trigger
  */
template <  typename T1    // Parameter type 1
          , typename T2    // Parameter type 2
          , typename T3    // Parameter type 3
          , typename T4    // Parameter type 4
          , typename T5    // Parameter type 5
          , typename T6    // Parameter type 6
         >
class TriggerStub6 : public TypelessStub<void>
{
  public:

  /** Creates the object.
    * @param  in_tc1  pointer to trigger constraint 1
    * @param  in_tc2  pointer to trigger constraint 2
    * @param  in_tc3  pointer to trigger constraint 3
    * @param  in_tc4  pointer to trigger constraint 4
    * @param  in_tc5  pointer to trigger constraint 5
    * @param  in_tc6  pointer to trigger constraint 6
    */
    TriggerStub6(  TriggeredConstraint<T1> *in_tc1
                 , TriggeredConstraint<T2> *in_tc2
                 , TriggeredConstraint<T3> *in_tc3
                 , TriggeredConstraint<T4> *in_tc4
                 , TriggeredConstraint<T5> *in_tc5
                 , TriggeredConstraint<T6> *in_tc6
                )
      : tc1(in_tc1)
      , tc2(in_tc2)
      , tc3(in_tc3)
      , tc4(in_tc4)
      , tc5(in_tc5)
      , tc6(in_tc6)
    {}

  /** Mock the invocation without passing parameters.
    * Triggers the according constraints.
    */
    virtual void typelessInvoke()
    {
      tc1->trigger();
      tc2->trigger();
      tc3->trigger();
      tc4->trigger();
      tc5->trigger();
      tc6->trigger();
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

      fmt += MOCKPP_PCHAR("is void");
      buffer += fmt;

      return buffer;
    }

  private:

    TriggeredConstraint<T1>   *tc1;
    TriggeredConstraint<T2>   *tc2;
    TriggeredConstraint<T3>   *tc3;
    TriggeredConstraint<T4>   *tc4;
    TriggeredConstraint<T5>   *tc5;
    TriggeredConstraint<T6>   *tc6;
};


MOCKPP_NS_END


#endif // MOCKPP_TriggerStub_H

