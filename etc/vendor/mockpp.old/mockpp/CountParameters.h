/** @file
    @internal NOT INTENDED FOR PUBLIC INCLUSION
    @brief    Generated with gen_countparameters_N.pl.

  $Id: CountParameters.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sun Oct 30 2005
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

#ifndef MOCKPP_COUNTPARAMETERS_N_H
#define MOCKPP_COUNTPARAMETERS_N_H


MOCKPP_NS_START


/** Helper class to distinguish present from missing parameters.
  * @internal
  */
class NoParameter
{
  NoParameter() {}
};


/** Helper class to count actual parameters.
  * @internal
  */
template<typename T>
class CountIfUsed
{
  public:

    enum { value = 1 };
};


/** Helper class to disable counting of non-parameters.
  * @internal
  */
template<>
class CountIfUsed<NoParameter>
{
  public:

    enum { value = 0 };
};


/** Helper class to sum actual parameters.
  * @internal
  */
template<  typename T
         , typename P1 = NoParameter
         , typename P2 = NoParameter
         , typename P3 = NoParameter
         , typename P4 = NoParameter
         , typename P5 = NoParameter
         , typename P6 = NoParameter>
class CountParameters
{
  public:

    enum
    { value =   CountIfUsed<P1>::value
              + CountIfUsed<P2>::value
              + CountIfUsed<P3>::value
              + CountIfUsed<P4>::value
              + CountIfUsed<P5>::value
              + CountIfUsed<P6>::value
    };
};


MOCKPP_NS_END


#endif // MOCKPP_COUNTPARAMETERS_N_H

