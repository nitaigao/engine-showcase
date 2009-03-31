/** @file
    @brief    Time dependent objects

  $Id: TimeServer.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 **************************************************************************/

/**************************************************************************

   begin                : Sat Apr 30 2005
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

#ifndef MOCKPP_TimeServer_H
#define MOCKPP_TimeServer_H

#include <mockpp/mockpp.h> // always first

#include <ctime>


MOCKPP_NS_START


/** Base class for time serving purposes
  * \ingroup grp_production
  * @see MockTimeServer
  */
class MOCKPP_API_DECL0 TimeServer
{
  public:

  /** Destroys the time server.
    */
    virtual ~TimeServer()
    {
    }

  /** Returns the current unix time
    * @return unix time
    */
    virtual MOCKPP_STD_NS::time_t getUnixTime() = 0;
};


/** Real time serving.
  * \ingroup grp_production
  */
class MOCKPP_API_DECL0 RealTimeServer : public TimeServer
{
  public:

  /** Returns the current unix time
    * @return unix time
    */
    virtual MOCKPP_STD_NS::time_t getUnixTime();
};


MOCKPP_NS_END


#endif // MOCKPP_TimeServer_H


