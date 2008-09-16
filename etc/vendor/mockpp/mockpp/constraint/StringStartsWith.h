/** @file
    @brief  Checks the occurrence of a substring at the beginning

  $Id: StringStartsWith.h 1254 2007-01-01 16:46:44Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sat Aug 21 2004
   begin                : Wed Jan 05 2005
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

#ifndef MOCKPP_STRINGSTARTSWITH_H
#define MOCKPP_STRINGSTARTSWITH_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/constraint/Constraint.h>
#include <mockpp/compat/Formatter.h>


MOCKPP_NS_START


/** Tests if the argument is a string that contains a substring.
  * @ingroup grp_constraint
  * @see mockpp::startsWith
  */
template <typename StringType = MOCKPP_NS::String>
class StringStartsWith : public Constraint<StringType>
{
  public:

  /** Constructs the object
    * @param in_substring  the substring to search
    */
    StringStartsWith( const StringType &in_substring )
      : substring(in_substring)
    {
    }

  /** Destroys the object
    */
    virtual ~StringStartsWith()
    {}

  /** Evaluates the constraint
    * @param o the object against which the constraint is evaluated.
    * @return true:  o meets the constraint,
    * @return false if it does not.
    */
    virtual bool eval( const StringType &o ) const
    {
        return o.substr(0, substring.length()) == substring;
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
        buffer += MOCKPP_PCHAR("a string starting with \"")
               + MOCKPP_GET_STRING(substring)
               + MOCKPP_PCHAR("\"");
        return buffer;
    }

  private:

    StringType substring;
 };


MOCKPP_NS_END


#endif // MOCKPP_STRINGSTARTSWITH_H
