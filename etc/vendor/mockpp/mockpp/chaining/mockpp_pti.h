/** @file
    @brief Support for compilers with weak template capabilities

  $Id: mockpp_pti.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Wed Jan 12 2005
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

#ifndef MOCKPP_PTI_H
#define MOCKPP_PTI_H

#ifdef MOCKPP_PTI_WEAKNESS // Partial Template Instantiation Weakness

#include <mockpp/mockpp.h>

#include <mockpp/chaining/LIFOInvocationDispatcher.h>
#include <mockpp/chaining/FIFOInvocationDispatcher.h>
#include <mockpp/chaining/CoreMock.h>
#include <mockpp/chaining/InvocationMocker.h>
#include <mockpp/chaining/AbstractDynamicChainingMock.h>

/////////////////////////////////////
// keep order
#define MOCKPP_PTI_IMPLEMENT(I) \
 MOCKPP_TYPELESSSTUBADAPTER_PTI_IMPL(I) \
 MOCKPP_FIFOINVOCATIONDISPATCHER_PTI_IMPL(I)  \
 MOCKPP_LIFOINVOCATIONDISPATCHER_PTI_IMPL(I) \
 MOCKPP_INVOCATIONMOCKER_PTI_IMPL(I) \
 MOCKPP_ABSTRACTDYNAMICCHAININGMOCK_PTI_IMPL(I) \
 MOCKPP_COREMOCK_PTI_IMPL(I)

/////////////////////////////////////
// keep order
#define MOCKPP_PTI_DECLARE(I) \
 MOCKPP_TYPELESSSTUBADAPTER_PTI_DECL(I) \
 MOCKPP_FIFOINVOCATIONDISPATCHER_PTI_DECL(I)  \
 MOCKPP_LIFOINVOCATIONDISPATCHER_PTI_DECL(I) \
 MOCKPP_INVOCATIONMOCKER_PTI_DECL(I) \
 MOCKPP_ABSTRACTDYNAMICCHAININGMOCK_PTI_DECL(I) \
 MOCKPP_COREMOCK_PTI_DECL(I)

#else

#define MOCKPP_PTI_IMPLEMENT(I)      /* empty by default */
#define MOCKPP_PTI_DECLARE(I)        /* empty by default */

#endif

/** @def MOCKPP_PTI_IMPLEMENT
  * Instantiate specialized methods for compilers with
  * weak template support.
  */

/** @def MOCKPP_PTI_DECLARE
  * Declare specialized methods for compilers with
  * weak template support.
  */

#endif // MOCKPP_PTI_H
