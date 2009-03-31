/** @file
    @brief  Checks the occurrence of a substring at the end

  $Id: StringEndsWith.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Wed Jan 05 2005
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

#ifndef MOCKPP_STRINGENDSWITH_H
#define MOCKPP_STRINGENDSWITH_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/constraint/Constraint.h>
#include <mockpp/compat/Formatter.h>


MOCKPP_NS_START


/** Tests if the argument is a string that contains a substring.
  * @ingroup grp_constraint
  * @see mockpp::endsWith
  */
template <typename StringType = MOCKPP_NS::String>
class StringEndsWith : public Constraint<StringType>
{
  public:

  /** Constructs the object
    * @param in_substring  the trailing string
    */
    StringEndsWith( const StringType &in_substring )
      : substring(in_substring)
    {
    }

  /** Destroys the object
    */
    virtual ~StringEndsWith()
    {}

  /** Evaluates the constraint
    * @param o the object against which the constraint is evaluated.
    * @return true:  o meets the constraint,
    * @return false if it does not.
    */
    virtual bool eval( const StringType &o ) const
    {
        unsigned l = substring.length();
        unsigned ol = o.length();
        if (ol < l)
          return false;

        return o.substr(ol - l, l) == substring;
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
        buffer += MOCKPP_PCHAR("a string ending with \"")
               + MOCKPP_GET_STRING(substring)
               + MOCKPP_PCHAR("\"");
        return buffer;
    }

  private:

    StringType substring;
 };


MOCKPP_NS_END


#endif // MOCKPP_STRINGENDSWITH_H
