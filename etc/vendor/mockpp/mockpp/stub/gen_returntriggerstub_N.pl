#!/usr/bin/perl -w
#
#   $Id: gen_returntriggerstub_N.pl 1437 2008-03-08 20:12:53Z ewald-arnold $

use English;
use Cwd;

if ($#ARGV < 0) {
  print "Usage: perl gen_returntriggerstub_N <number of arguments>\n";
  exit;
}


$totalNumArgs = $ARGV[0];
if ($totalNumArgs < 5) {
  $totalNumArgs = 5; }

$dir = getcwd;
print "============================================================\n";
print "==> Creating file: $dir/ReturnAndTriggerStub.h\n";

open OUT, ">ReturnAndTriggerStub.h";

print OUT
"/** \@file
    \@brief    Stub returning a defined value and triggering contraints.
              Generated with gen_returntriggerstub_N.pl.

  \$I" . "d: ReturnAndTriggerStub.h,v 1.7 2005/10/19 20:53:09 ewald-arnold Exp \$

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

#ifndef MOCKPP_ReturnAndTriggerStub_H
#define MOCKPP_ReturnAndTriggerStub_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/compat/Formatter.h>

#include <mockpp/stub/TypelessStub.h>

#include <mockpp/constraint/TriggeredConstraint.h>


MOCKPP_NS_START
";

for ($numArgs = 1; $numArgs <= $totalNumArgs; ++$numArgs) {

  $templateParms = "";
  $templateParms_colon = "";
  for ($i = 1; $i <= $numArgs; ++$i) {
    if ($i > 1) {
      $templateParms .= ", ";
    }
    $templateParms .= "typename P$i";
  }
  if ($numArgs > 0) {
  $templateParms_colon = ", " . $templateParms; }


  $templateArgs_colon = "";
  $_ = $templateParms;
  s/typename //g;
  $templateArgs = $_;
  if ($numArgs > 0) {
  $templateArgs_colon = ", ". $templateArgs; }


  $args = "";
  for ($i = 1; $i <= $numArgs; ++$i) {
    if ($i > 1) {
      $args .= ", ";
    }
    $args .= "p$i";
  }




print "Creating ReturnAndTriggerStub" . $numArgs ."\n";

print OUT "

/** Always return the same specified value and trigger some contraint.
  * \@ingroup grp_stub
  * \@see mockpp::returnValueAndTrigger
  */
template <  typename R     // Returntype";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
          , typename T" . $p . "    // Parameter type " . $p }

print OUT "
         >
class ReturnAndTriggerStub" . $numArgs . " : public TypelessStub<R>
{
  public:

  /** Creates the object.
    * \@param  in_result   the return value";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param  in_tc" . $p . "  pointer to trigger constraint " . $p . ""; }

print OUT "
    */
    ReturnAndTriggerStub" . $numArgs . "(  const R &in_result";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
                          , TriggeredConstraint<T" . $p . "> *in_tc" . $p  }

print OUT "
                         )
      : result(in_result)";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
      , tc" . $p . "(in_tc" . $p . ")" }

print OUT "
    {}

  /** Mock the invocation without passing parameters.
    * Triggers the according constraints and returns the value.
    * \@return return value from mocked invocation
    */
    virtual R typelessInvoke()
    {";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
      tc" . $p . "->trigger();"; }

print OUT "
      return result;
    }

  /** Appends the description of this object to the buffer.
    * \@param buffer The buffer that the description is appended to.
    * \@return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      String fmt;";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
      {
        String tcfmt;
        tc" . $p . "->describeTo(tcfmt);
        fmt += MOCKPP_PCHAR(\"triggers [%1]\\n\");
        fmt << tcfmt;
      }
"}

print OUT "
      fmt += MOCKPP_PCHAR(\"returns <%1>\");
      fmt << result;
      buffer += fmt;

      return buffer;
    }

  private:

    R                          result;";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    TriggeredConstraint<T" . $p . ">   *tc" . $p . ";" }

print OUT "
};
"

}

print OUT "

MOCKPP_NS_END


#endif // MOCKPP_ReturnAndTriggerStub_H

";

close OUT;

