/** @file
    @brief Support in production code

  $Id: mockpp_production.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 **************************************************************************/

/**************************************************************************

   begin                : Dat Apr 30 2005
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

#ifndef MOCKPP_PRODUCTION_H
#define MOCKPP_PRODUCTION_H

#include <mockpp/mockpp.h> // always first

/** @defgroup grp_production Support in production code
  * This module contains classes that help to prepare production code
  * for testing with mock objects. This code is placed in a library
  * of its own. So it is still possible to run production code
  * without the bloat of the testing environment.
  */

MOCKPP_NS_START

MOCKPP_NS_END


#endif // MOCKPP_PRODUCTION_H


