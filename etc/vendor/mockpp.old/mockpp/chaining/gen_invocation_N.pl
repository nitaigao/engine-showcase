#!/usr/bin/perl -w
#
#   $Id: gen_invocation_N.pl 1437 2008-03-08 20:12:53Z ewald-arnold $

use English;
use Cwd;

if ($#ARGV < 0) {
  print "Usage: perl gen_invocation_N <number of arguments>\n";
  exit;
}


$totalNumArgs = $ARGV[0];
if ($totalNumArgs < 5) {
  $totalNumArgs = 5; }

$dir = getcwd;
print "============================================================\n";
print "==> Creating file: $dir/InvocationN.h\n";

open OUT, ">InvocationN.h";

print OUT
"/** \@file
    \@internal NOT INTENDED FOR PUBLIC INCLUSION
    \@brief    Generated with gen_invocation_N.pl.

  \$I" . "d: InvocationN.h,v 1.7 2005/10/19 20:53:09 ewald-arnold Exp \$

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Aug 24 2004
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

#ifndef MOCKPP_INVOCATION_N_H
#define MOCKPP_INVOCATION_N_H

//#include <mockpp/chaining/Invocation.h>


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
    $argTypes .= "typedef P$i p". ($i+1) . "_type;  //!< internal shorthand"; }


print "Creating Invocation" . $numArgs ."\n";

print OUT "

/** Hold invocations with " .  $numArgs . " parameters
  * \@internal
  */
template <" . $templateParms . ">
class Invocation" .  $numArgs . " : public Invocation
{
  public:

    enum { numParams = " .  $numArgs . " };

    typedef ConstraintSet" .  $numArgs . "<" . $templateArgs . "> ConstraintSetType;   //!< internal shorthand
";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    typedef P" .  $p . " T" .  $p . "Type;   //!< internal shorthand"; }

print OUT "

  /** Constructs the object
    * \@param  objname   name of the object";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param in_param" .  $p . "  parameter " .  $p ; }

print OUT "
    */
    Invocation" .  $numArgs . "(  const String &objname";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
                , const P" .  $p . " &in_param" .  $p; }

print OUT ")
      : Invocation( objname )";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
      , param" .  $p . "( in_param" .  $p . " )"; }

print OUT "
    {
    }

#ifdef MOCKPP_USE_INVOCATION_EQUALS

  /** Check if the object equals another invocation
    * \@param other  the other invocation
    * \@return  true: objects are equal
    */
    virtual bool equals( const Invocation" .  $numArgs . "<" . $templateArgs . "> &other ) const
    {
      return invocationComparison(param1, other.param1)";

for($p = 2; $p <= $numArgs; ++$p) { print OUT "
             && invocationComparison(param" . $p . ", other.param" . $p . ")";}

print OUT ";
    }

  /** Check if the object equals another invocation
    * \@param other  the other invocation
    * \@return  true: objects are equal
    */
    bool equals( const InvocationBase &other ) const
    {
      MOCKPP_UNUSED(other);
      return false;
    }

#endif // MOCKPP_USE_INVOCATION_EQUALS
";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
  /** Returns an invocation parameter
    * \@return  the invocation parameter " . $p . "
    */
    const P" . $p . " & getParameter" . $p . "() const
    {
      return param" . $p . ";
    }
"; }

print OUT "
  /** Returns a description of the parameters
    * \@return the description
    */
    virtual String describeParameters() const
    {
      String fmt = MOCKPP_PCHAR(\"" . $percentParms . "\");
      fmt << param1";

for($p = 2; $p <= $numArgs; ++$p) { print OUT "
          << param" .$p; }

print OUT ";
      return fmt;
    }

  private:
";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    const P" . $p . " & param" .$p . ";"; }

print OUT "
};
"

}

print OUT "

MOCKPP_NS_END


#endif // MOCKPP_INVOCATION_N_H

";

close OUT;

