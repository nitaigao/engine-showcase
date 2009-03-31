/** @file
    @brief    Generated with gen_returntrigger_N.pl.

  $Id: ReturnValueAndTrigger.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_ReturnAndTrigger_H
#define MOCKPP_ReturnAndTrigger_H

#include <mockpp/constraint/TriggeredConstraint.h>

#include <mockpp/stub/ReturnAndTriggerStub.h>
#include <mockpp/stub/TriggerStub.h>


MOCKPP_NS_START


/** Creates a stub returning a value and triggering a constraint
  * @ingroup grp_stub_abbrev
  * @see mockpp::ReturnStub
  * @param  o    the value to return
  * @param  tc1  pointer to trigger constraint 1
  * @return the new stub
  */
template <  typename R      // Return Value
          , typename TC1    // Contraint Parameter Type 1
         >
typename TypelessStub<R>::AP returnValueAndTrigger(  const R &o
                                                   , TriggeredConstraint<TC1> *tc1
                                                  )
{
  return new ReturnAndTriggerStub1<R, TC1>(
                                       o
                                     , tc1
                                     );
}


/** Creates a stub triggering a constraint
  * @ingroup grp_stub_abbrev
  * @see mockpp::ReturnStub
  * @param  tc1  pointer to trigger constraint 1
  * @return the new stub
  */
template <  typename TC1    // Contraint Parameter Type 1
         >
typename TypelessStub<void>::AP trigger(  TriggeredConstraint<TC1> *tc1
                                                     )
{
  return new TriggerStub1<TC1>
                     (  tc1);
}


/** Creates a stub returning a value and triggering a constraint
  * @ingroup grp_stub_abbrev
  * @see mockpp::ReturnStub
  * @param  o    the value to return
  * @param  tc1  pointer to trigger constraint 1
  * @param  tc2  pointer to trigger constraint 2
  * @return the new stub
  */
template <  typename R      // Return Value
          , typename TC1    // Contraint Parameter Type 1
          , typename TC2    // Contraint Parameter Type 2
         >
typename TypelessStub<R>::AP returnValueAndTrigger(  const R &o
                                                   , TriggeredConstraint<TC1> *tc1
                                                   , TriggeredConstraint<TC2> *tc2
                                                  )
{
  return new ReturnAndTriggerStub2<R, TC1, TC2>(
                                       o
                                     , tc1
                                     , tc2
                                     );
}


/** Creates a stub triggering a constraint
  * @ingroup grp_stub_abbrev
  * @see mockpp::ReturnStub
  * @param  tc1  pointer to trigger constraint 1
  * @param  tc2  pointer to trigger constraint 2
  * @return the new stub
  */
template <  typename TC1    // Contraint Parameter Type 1
          , typename TC2    // Contraint Parameter Type 2
         >
typename TypelessStub<void>::AP trigger(  TriggeredConstraint<TC1> *tc1
                                                      , TriggeredConstraint<TC2> *tc2
                                                     )
{
  return new TriggerStub2<TC1, TC2>
                     (  tc1
                      , tc2);
}


/** Creates a stub returning a value and triggering a constraint
  * @ingroup grp_stub_abbrev
  * @see mockpp::ReturnStub
  * @param  o    the value to return
  * @param  tc1  pointer to trigger constraint 1
  * @param  tc2  pointer to trigger constraint 2
  * @param  tc3  pointer to trigger constraint 3
  * @return the new stub
  */
template <  typename R      // Return Value
          , typename TC1    // Contraint Parameter Type 1
          , typename TC2    // Contraint Parameter Type 2
          , typename TC3    // Contraint Parameter Type 3
         >
typename TypelessStub<R>::AP returnValueAndTrigger(  const R &o
                                                   , TriggeredConstraint<TC1> *tc1
                                                   , TriggeredConstraint<TC2> *tc2
                                                   , TriggeredConstraint<TC3> *tc3
                                                  )
{
  return new ReturnAndTriggerStub3<R, TC1, TC2, TC3>(
                                       o
                                     , tc1
                                     , tc2
                                     , tc3
                                     );
}


/** Creates a stub triggering a constraint
  * @ingroup grp_stub_abbrev
  * @see mockpp::ReturnStub
  * @param  tc1  pointer to trigger constraint 1
  * @param  tc2  pointer to trigger constraint 2
  * @param  tc3  pointer to trigger constraint 3
  * @return the new stub
  */
template <  typename TC1    // Contraint Parameter Type 1
          , typename TC2    // Contraint Parameter Type 2
          , typename TC3    // Contraint Parameter Type 3
         >
typename TypelessStub<void>::AP trigger(  TriggeredConstraint<TC1> *tc1
                                                      , TriggeredConstraint<TC2> *tc2
                                                      , TriggeredConstraint<TC3> *tc3
                                                     )
{
  return new TriggerStub3<TC1, TC2, TC3>
                     (  tc1
                      , tc2
                      , tc3);
}


/** Creates a stub returning a value and triggering a constraint
  * @ingroup grp_stub_abbrev
  * @see mockpp::ReturnStub
  * @param  o    the value to return
  * @param  tc1  pointer to trigger constraint 1
  * @param  tc2  pointer to trigger constraint 2
  * @param  tc3  pointer to trigger constraint 3
  * @param  tc4  pointer to trigger constraint 4
  * @return the new stub
  */
template <  typename R      // Return Value
          , typename TC1    // Contraint Parameter Type 1
          , typename TC2    // Contraint Parameter Type 2
          , typename TC3    // Contraint Parameter Type 3
          , typename TC4    // Contraint Parameter Type 4
         >
typename TypelessStub<R>::AP returnValueAndTrigger(  const R &o
                                                   , TriggeredConstraint<TC1> *tc1
                                                   , TriggeredConstraint<TC2> *tc2
                                                   , TriggeredConstraint<TC3> *tc3
                                                   , TriggeredConstraint<TC4> *tc4
                                                  )
{
  return new ReturnAndTriggerStub4<R, TC1, TC2, TC3, TC4>(
                                       o
                                     , tc1
                                     , tc2
                                     , tc3
                                     , tc4
                                     );
}


/** Creates a stub triggering a constraint
  * @ingroup grp_stub_abbrev
  * @see mockpp::ReturnStub
  * @param  tc1  pointer to trigger constraint 1
  * @param  tc2  pointer to trigger constraint 2
  * @param  tc3  pointer to trigger constraint 3
  * @param  tc4  pointer to trigger constraint 4
  * @return the new stub
  */
template <  typename TC1    // Contraint Parameter Type 1
          , typename TC2    // Contraint Parameter Type 2
          , typename TC3    // Contraint Parameter Type 3
          , typename TC4    // Contraint Parameter Type 4
         >
typename TypelessStub<void>::AP trigger(  TriggeredConstraint<TC1> *tc1
                                                      , TriggeredConstraint<TC2> *tc2
                                                      , TriggeredConstraint<TC3> *tc3
                                                      , TriggeredConstraint<TC4> *tc4
                                                     )
{
  return new TriggerStub4<TC1, TC2, TC3, TC4>
                     (  tc1
                      , tc2
                      , tc3
                      , tc4);
}


/** Creates a stub returning a value and triggering a constraint
  * @ingroup grp_stub_abbrev
  * @see mockpp::ReturnStub
  * @param  o    the value to return
  * @param  tc1  pointer to trigger constraint 1
  * @param  tc2  pointer to trigger constraint 2
  * @param  tc3  pointer to trigger constraint 3
  * @param  tc4  pointer to trigger constraint 4
  * @param  tc5  pointer to trigger constraint 5
  * @return the new stub
  */
template <  typename R      // Return Value
          , typename TC1    // Contraint Parameter Type 1
          , typename TC2    // Contraint Parameter Type 2
          , typename TC3    // Contraint Parameter Type 3
          , typename TC4    // Contraint Parameter Type 4
          , typename TC5    // Contraint Parameter Type 5
         >
typename TypelessStub<R>::AP returnValueAndTrigger(  const R &o
                                                   , TriggeredConstraint<TC1> *tc1
                                                   , TriggeredConstraint<TC2> *tc2
                                                   , TriggeredConstraint<TC3> *tc3
                                                   , TriggeredConstraint<TC4> *tc4
                                                   , TriggeredConstraint<TC5> *tc5
                                                  )
{
  return new ReturnAndTriggerStub5<R, TC1, TC2, TC3, TC4, TC5>(
                                       o
                                     , tc1
                                     , tc2
                                     , tc3
                                     , tc4
                                     , tc5
                                     );
}


/** Creates a stub triggering a constraint
  * @ingroup grp_stub_abbrev
  * @see mockpp::ReturnStub
  * @param  tc1  pointer to trigger constraint 1
  * @param  tc2  pointer to trigger constraint 2
  * @param  tc3  pointer to trigger constraint 3
  * @param  tc4  pointer to trigger constraint 4
  * @param  tc5  pointer to trigger constraint 5
  * @return the new stub
  */
template <  typename TC1    // Contraint Parameter Type 1
          , typename TC2    // Contraint Parameter Type 2
          , typename TC3    // Contraint Parameter Type 3
          , typename TC4    // Contraint Parameter Type 4
          , typename TC5    // Contraint Parameter Type 5
         >
typename TypelessStub<void>::AP trigger(  TriggeredConstraint<TC1> *tc1
                                                      , TriggeredConstraint<TC2> *tc2
                                                      , TriggeredConstraint<TC3> *tc3
                                                      , TriggeredConstraint<TC4> *tc4
                                                      , TriggeredConstraint<TC5> *tc5
                                                     )
{
  return new TriggerStub5<TC1, TC2, TC3, TC4, TC5>
                     (  tc1
                      , tc2
                      , tc3
                      , tc4
                      , tc5);
}


/** Creates a stub returning a value and triggering a constraint
  * @ingroup grp_stub_abbrev
  * @see mockpp::ReturnStub
  * @param  o    the value to return
  * @param  tc1  pointer to trigger constraint 1
  * @param  tc2  pointer to trigger constraint 2
  * @param  tc3  pointer to trigger constraint 3
  * @param  tc4  pointer to trigger constraint 4
  * @param  tc5  pointer to trigger constraint 5
  * @param  tc6  pointer to trigger constraint 6
  * @return the new stub
  */
template <  typename R      // Return Value
          , typename TC1    // Contraint Parameter Type 1
          , typename TC2    // Contraint Parameter Type 2
          , typename TC3    // Contraint Parameter Type 3
          , typename TC4    // Contraint Parameter Type 4
          , typename TC5    // Contraint Parameter Type 5
          , typename TC6    // Contraint Parameter Type 6
         >
typename TypelessStub<R>::AP returnValueAndTrigger(  const R &o
                                                   , TriggeredConstraint<TC1> *tc1
                                                   , TriggeredConstraint<TC2> *tc2
                                                   , TriggeredConstraint<TC3> *tc3
                                                   , TriggeredConstraint<TC4> *tc4
                                                   , TriggeredConstraint<TC5> *tc5
                                                   , TriggeredConstraint<TC6> *tc6
                                                  )
{
  return new ReturnAndTriggerStub6<R, TC1, TC2, TC3, TC4, TC5, TC6>(
                                       o
                                     , tc1
                                     , tc2
                                     , tc3
                                     , tc4
                                     , tc5
                                     , tc6
                                     );
}


/** Creates a stub triggering a constraint
  * @ingroup grp_stub_abbrev
  * @see mockpp::ReturnStub
  * @param  tc1  pointer to trigger constraint 1
  * @param  tc2  pointer to trigger constraint 2
  * @param  tc3  pointer to trigger constraint 3
  * @param  tc4  pointer to trigger constraint 4
  * @param  tc5  pointer to trigger constraint 5
  * @param  tc6  pointer to trigger constraint 6
  * @return the new stub
  */
template <  typename TC1    // Contraint Parameter Type 1
          , typename TC2    // Contraint Parameter Type 2
          , typename TC3    // Contraint Parameter Type 3
          , typename TC4    // Contraint Parameter Type 4
          , typename TC5    // Contraint Parameter Type 5
          , typename TC6    // Contraint Parameter Type 6
         >
typename TypelessStub<void>::AP trigger(  TriggeredConstraint<TC1> *tc1
                                                      , TriggeredConstraint<TC2> *tc2
                                                      , TriggeredConstraint<TC3> *tc3
                                                      , TriggeredConstraint<TC4> *tc4
                                                      , TriggeredConstraint<TC5> *tc5
                                                      , TriggeredConstraint<TC6> *tc6
                                                     )
{
  return new TriggerStub6<TC1, TC2, TC3, TC4, TC5, TC6>
                     (  tc1
                      , tc2
                      , tc3
                      , tc4
                      , tc5
                      , tc6);
}




MOCKPP_NS_END


#endif // MOCKPP_ReturnAndTrigger_H

