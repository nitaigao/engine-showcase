/** @file
    @brief    Basic support for "Design By Contract"

      If you want to use the DBC statements within you sources in your doxygen documentation
      you have to add something similar to this into your doxygen configuration file:

  @verbatim

  PREDEFINED =  \
                DOXYGEN_SHOULD_SKIP_THIS \
                "MOCKPP_PRE(a)              = / *! \pre a                        * /" \
                "MOCKPP_PRE_MSG(a,b)       = / *! \pre a \code b \endcode       * /" \
                "MOCKPP_POST(a)             = / *! \post a                       * /" \
                "MOCKPP_POST_MSG(a,b)      = / *! \post a \code b \endcode      * /" \
                "MOCKPP_INVARIANT(a)        = / *! \invariant a                  * /" \
                "MOCKPP_INVARIANT_MSG(a,b) = / *! \invariant a \code b \endcode * /"
  @endverbatim

  $Id: mockpp_dbc.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sun Apr 23 2005
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

#ifndef MOCKPP_DBC_H
#define MOCKPP_DBC_H

#include <mockpp/mockpp.h> // always first

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#ifdef MOCK_ENABLE_DBC

# define  MOCKPP_PRE(a)               MOCKPP_NS::assertTrue(__LINE__, __FILE__, #a, a)

# define  MOCKPP_PRE_MSG(a, b)        MOCKPP_NS::assertTrue(__LINE__, __FILE__, b, a)

# define  MOCKPP_POST(a)              MOCKPP_NS::assertTrue(__LINE__, __FILE__, #a, a)

# define  MOCKPP_POST_MSG(a, b)       MOCKPP_NS::assertTrue(__LINE__, __FILE__, b, a)

# define  MOCKPP_INVARIANT(a)         MOCKPP_NS::assertTrue(__LINE__, __FILE__, #a, a)

# define  MOCKPP_INVARIANT_MSG(a, b)  MOCKPP_NS::assertTrue(__LINE__, __FILE__, b, a)

#else // MOCK_ENABLE_DBC

# define  MOCKPP_PRE(a)
# define  MOCKPP_PRE_MSG(a, b)
# define  MOCKPP_POST(a)
# define  MOCKPP_POST_MSG(a, b)
# define  MOCKPP_INVARIANT(a)
# define  MOCKPP_INVARIANT_MSG(a, b)

#endif // MOCK_ENABLE_DBC

/** @def MOCKPP_PRE(a)
  * @brief Checks a precondition of the method. If the condition evaluates to \c false
  *        an AssertionFailedError is thrown.
  * @param a   the condition to check
  */

/** @def MOCKPP_PRE(a,b)
  * @brief Checks a precondition of the method. If the condition evaluates to \c false
  *        an AssertionFailedError is thrown.
  * @param a   the condition to check
  * @param b   An explanation for the condition
  */

/** @def MOCKPP_POST(a)
  * @brief Checks a postcondition of the method. If the condition evaluates to \c false
  *        an AssertionFailedError is thrown.
  * @param a   the condition to check
  */

/** @def MOCKPP_POST(a,b)
  * @brief Checks a postcondition of the method. If the condition evaluates to \c false
  *        an AssertionFailedError is thrown.
  * @param a   the condition to check
  * @param b   An explanation for the condition
  */

/** @def MOCKPP_INVARIANT(a)
  * @brief Checks an invariant of the method. If the condition evaluates to \c false
  *        an AssertionFailedError is thrown.
  * @param a   the condition to check
  */

/** @def MOCKPP_INVARIANT(a,b)
  * @brief Checks an invariant of the method. If the condition evaluates to \c false
  *        an AssertionFailedError is thrown.
  * @param a   the condition to check
  * @param b   An explanation for the condition
  */

#endif

#endif // MOCKPP_DBC_H
