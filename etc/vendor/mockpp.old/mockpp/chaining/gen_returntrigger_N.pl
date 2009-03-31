#!/usr/bin/perl -w
#
#   $Id: gen_returntrigger_N.pl 1437 2008-03-08 20:12:53Z ewald-arnold $

use English;
use Cwd;

if ($#ARGV < 0) {
  print "Usage: perl gen_returntrigger_N <number of arguments>\n";
  exit;
}


$totalNumArgs = $ARGV[0];
if ($totalNumArgs < 5) {
  $totalNumArgs = 5; }

$dir = getcwd;
print "============================================================\n";
print "==> Creating file: $dir/ReturnValueAndTrigger.h\n";

open OUT, ">ReturnValueAndTrigger.h";

print OUT
"/** \@file
    \@brief    Generated with gen_returntrigger_N.pl.

  \$I" . "d: InvocationN.h,v 1.7 2005/10/19 20:53:09 ewald-arnold Exp \$

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

";

for ($numArgs = 1; $numArgs <= $totalNumArgs; ++$numArgs) {

  $templateParms = "";
  $templateParms_colon = "";
  for ($i = 1; $i <= $numArgs; ++$i) {
    if ($i > 1) {
      $templateParms .= ", ";
    }
    $templateParms .= "typename TC$i";
  }
  if ($numArgs > 0) {
  $templateParms_colon = ", " . $templateParms; }


  $templateArgs_colon = "";
  $_ = $templateParms;
  s/typename //g;
  $templateArgs = $_;
  if ($numArgs > 0) {
  $templateArgs_colon = ", ". $templateArgs; }


print "Creating returnValueAndTrigger" . $numArgs ."\n";

print OUT "
/** Creates a stub returning a value and triggering a constraint
  * \@ingroup grp_stub_abbrev
  * \@see mockpp::ReturnStub
  * \@param  o    the value to return";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
  * \@param  tc" . $p . "  pointer to trigger constraint " . $p . ""; }

print OUT "
  * \@return the new stub
  */
template <  typename R      // Return Value";
for($p = 1; $p <= $numArgs; ++$p) { print OUT "
          , typename TC" . $p . "    // Contraint Parameter Type " . $p }

print OUT "
         >
typename TypelessStub<R>::AP returnValueAndTrigger(  const R &o";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
                                                   , TriggeredConstraint<TC" . $p . "> *tc" . $p }
print OUT "
                                                  )
{
  return new ReturnAndTriggerStub" . $numArgs . "<R" . $templateArgs_colon . ">(
                                       o";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
                                     , tc" . $p }

print OUT "
                                     );
}

";


print "Creating trigger" . $numArgs ."\n";

print OUT "
/** Creates a stub triggering a constraint
  * \@ingroup grp_stub_abbrev
  * \@see mockpp::ReturnStub";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
  * \@param  tc" . $p . "  pointer to trigger constraint " . $p . ""; }

print OUT "
  * \@return the new stub
  */
template <  typename TC1    // Contraint Parameter Type 1";

for($p = 2; $p <= $numArgs; ++$p) { print OUT "
          , typename TC" . $p . "    // Contraint Parameter Type " . $p }

print OUT "
         >
typename TypelessStub<void>::AP trigger(  TriggeredConstraint<TC1> *tc1";

for($p = 2; $p <= $numArgs; ++$p) { print OUT "
                                                      , TriggeredConstraint<TC" . $p . "> *tc" . $p }
print OUT "
                                                     )
{
  return new TriggerStub" . $numArgs . "<" . $templateArgs . ">
                     (  tc1";

for($p = 2; $p <= $numArgs; ++$p) { print OUT "
                      , tc" . $p }

print OUT ");
}

";
}

print OUT "


MOCKPP_NS_END


#endif // MOCKPP_ReturnAndTrigger_H

";

close OUT;

