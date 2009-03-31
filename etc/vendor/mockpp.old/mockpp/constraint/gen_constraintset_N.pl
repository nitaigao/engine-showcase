#!/usr/bin/perl -w
#
#   $Id: gen_constraintset_N.pl 1437 2008-03-08 20:12:53Z ewald-arnold $

use English;
use Cwd;

if ($#ARGV < 0) {
  print "Usage: perl gen_constraintset_N <number of arguments>\n";
  exit;
}


$totalNumArgs = $ARGV[0];
if ($totalNumArgs < 5) {
  $totalNumArgs = 5; }

$dir = getcwd;
print "============================================================\n";
print "==> Creating file: $dir/ConstraintSetN.h\n";

open OUT, ">ConstraintSetN.h";

print OUT
"/** \@file
    \@internal NOT INTENDED FOR PUBLIC INCLUSION
    \@brief    Generated with gen_constraintset_N.pl.

  \$I" . "d: ConstraintSetN.h,v 1.7 2005/10/19 20:53:09 ewald-arnold Exp \$

 ***************************************************************************/

/**************************************************************************

   begin                : Sat Aug 21 2004
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

#ifndef MOCKPP_ConstraintSet_N_H
#define MOCKPP_ConstraintSet_N_H

//#include <mockpp/builder/ConstraintSet.h>


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


  $parms = "";
  for ($i = 1; $i <= $numArgs; ++$i) {
    if ($i > 1) {
      $parms .= ", ";
    }
    $parms .= "const P$i &p$i";
  }

  $percentParms = "";
  for ($i = 1; $i <= $numArgs; ++$i) {
    if ($i > 1) {
      $percentParms .= ", ";
    }
    $percentParms .= "%$i";
  }


  $holderParms = "";
  for ($i = 1; $i <= $numArgs; ++$i) {
    if ($i > 1) {
      $holderParms .= ", ";
    }
    $holderParms .= "const ConstraintHolder<P$i> &p$i";
  }


  $args = "";
  for ($i = 1; $i <= $numArgs; ++$i) {
    if ($i > 1) {
      $args .= ", ";
    }
    $args .= "p$i";
  }


  $boundArgs = "";
  for ($i = 1; $i <= $numArgs; ++$i) {
    if ($i > 1) {
      $boundArgs .= ", ";
    }
    $boundArgs .= "args->a$i";
  }


  $argsAsMembers = "";
  for ($i = 1; $i <= $numArgs; ++$i) {
    $argsAsMembers .= "P$i p$i;";
  }


  $copyParms = "";
  for ($i = 1; $i <= $numArgs; ++$i) {
    if ($i > 1) {
      $copyParms .= ", ";
    }
    $copyParms .= "P$i ia$i";
  }


  $hArgs = "";
  for ($i = 1; $i <= $numArgs; ++$i) {
    if ($i > 1) {
      $hArgs .= ", ";
    }
    $hArgs .= "h$i";
  }


  $initArgs = "";
  if ($numArgs > 0) {
      $initArgs = ":";
  }
  for ($i = 1; $i <= $numArgs; ++$i) {
    if ($i > 1) {
      $initArgs .= ", ";
    }
    $initArgs .= "p$i(ip$i)";
  }


  $argTypes = "";
  for ($i = 1; $i <= $numArgs; ++$i) {
    $argTypes .= "typedef P$i p". ($i+1) . "_type; //!< internal shorthand"; }


print "Creating ConstraintSet" . $numArgs ."\n";

print OUT "

/** A set of constraints for a method with " . $numArgs . " arguments
  * \@internal
  */
template <" . $templateParms . ">
class ConstraintSet" . $numArgs . "
{
  public:
  ";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    typedef P" . $p . " ValueType" . $p . "; //!< internal shorthand"; }

print OUT "

  /** Constructs the object";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param  in_constraint" . $p . "  constraint " . $p . ""; }

print OUT "
    */
    ConstraintSet" . $numArgs . "(  const ConstraintHolder<P1> &in_constraint1";

for($p = 2; $p <= $numArgs; ++$p) { print OUT "
                   , const ConstraintHolder<P" . $p . "> &in_constraint" . $p; }

print OUT ")
        : constraint1( in_constraint1 )";


for($p = 2; $p <= $numArgs; ++$p) { print OUT "
        , constraint" . $p . "( in_constraint" . $p . " )"; }

print OUT "
    {}

  /** Destroys the object
    */
    virtual ~ConstraintSet" . $numArgs . "()
    {}

  /** Tests if the constraint set matches the invocation
    * \@param   invocation   the invocation data
    * \@return true: the set matches
    */
    template <typename I> // Invocation
    bool matches( const I &invocation )
    {
      return    constraint1->eval( invocation.getParameter1() )";

for($p = 2; $p <= $numArgs; ++$p) { print OUT "
             && constraint" . $p . "->eval( invocation.getParameter" . $p . "() )"; }

print OUT ";
    }

  /** Appends the description of this object to the buffer.
    * \@param buffer The buffer that the description is appended to.
    * \@return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      String fmt = MOCKPP_PCHAR( \"" . $percentParms . "\" );
      fmt << constraint1->toString()";

for($p = 2; $p <= $numArgs; ++$p) { print OUT "
          << constraint" . $p . "->toString()"; }

print OUT ";
      buffer += fmt;
      return buffer;
    }

  private:
";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    const typename Constraint<P" . $p . ">::AP constraint" . $p . ";"; }

print OUT "
};
";

}

print OUT "

MOCKPP_NS_END


#endif // MOCKPP_ConstraintSet_N_H

";

close OUT;

