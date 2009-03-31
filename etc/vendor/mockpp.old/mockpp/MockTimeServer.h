/** @file
    @brief    Time dependent objects

  $Id: MockTimeServer.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_TimeServerMock_H
#define MOCKPP_TimeServerMock_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/production/TimeServer.h>

#include <mockpp/ReturnObjectList.h>


MOCKPP_NS_START

/** Mock object to return defined time points.
  * \ingroup grp_basic_mo
  * Some unix times:
  *   @li 2005-02-02T10:52:19 ==> 1107337939L (0x4200A2D3)
  */
class MOCKPP_API_DECL0 MockTimeServer : public TimeServer
{
  public:

  /** Construct a mock time server
    * @param name     object name
    * @param parent   parent verifiable
    */
    MockTimeServer(const String &name = MOCKPP_PCHAR("") , MOCKPP_NS::VerifiableList *parent = 0);

  /** Returns the "current" unix time
    * @return unix time
    */
    virtual MOCKPP_STD_NS::time_t getUnixTime();

  /** Adds another "current" unix time
    * @param time  unix time
    */
    void addUnixTime(MOCKPP_STD_NS::time_t time);

  private:

    MOCKPP_NS::ReturnObjectList<MOCKPP_STD_NS::time_t> mocktimes;
};


MOCKPP_NS_END


#endif // MOCKPP_TimeServerMock_H


