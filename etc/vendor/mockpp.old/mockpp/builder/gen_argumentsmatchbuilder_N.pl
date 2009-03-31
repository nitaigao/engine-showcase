#!/usr/bin/perl -w
#
#   $Id: gen_argumentsmatchbuilder_N.pl 1437 2008-03-08 20:12:53Z ewald-arnold $

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
print "==> Creating file: $dir/ArgumentsMatchBuilderN.h\n";

open OUT, ">ArgumentsMatchBuilderN.h";

print OUT
"/** \@file
    \@internal NOT INTENDED FOR PUBLIC INCLUSION
    \@brief    Generated with gen_argumentsmatchbuilder_N.pl.

  \$I" . "d: InvocationN.h,v 1.7 2005/10/19 20:53:09 ewald-arnold Exp \$

 ***************************************************************************/

/**************************************************************************

   begin                : Sun Aug 22 2004
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

#ifndef MOCKPP_ARGUMENTSMATCHBUILDER_N_H
#define MOCKPP_ARGUMENTSMATCHBUILDER_N_H

//#include <mockpp/builder/ArgumentsMatchBuilder.h>


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


print "Creating ArgumentsMatchBuilder" . $numArgs ."\n";

print OUT "
/** Build a matcher for an invocation with " . $numArgs ." argument.
  * \@ingroup grp_chainer
  * \@internal
  */
template <typename R,    // Returntype
          typename I>    // Invocation
class ArgumentsMatchBuilder" . $numArgs ." : public MatchBuilder<R,I>
{
  public:

    typedef R ReturnType;           //!< internal shared member
    typedef I InvocationType;       //!< internal shared member";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    typedef typename I::T" .$p . "Type P" .$p . ";  //!< internal shared member"; }

print OUT "

    typedef ConstraintSet" . $numArgs ."<" . $templateArgs . "> ConstraintSetType;  //!< internal shorthand

  /** Adds another matcher.
    * \@param matcher the matcher object
    * \@return the builder object
    */
    virtual MatchBuilder<R, I>& addMatcher( const MatcherHolder<I> &matcher ) = 0;

  /** Indicate a matcher for " . $numArgs ." arguments.
    * \@ingroup grp_chainer";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param p" .$p . " pointer to the constraint for method parameter " .$p; }

print OUT "
    * \@return the builder object
    */
    MatchBuilder<R, I>& with(  const ConstraintHolder<P1> &p1";

for($p = 2; $p <= $numArgs; ++$p) { print OUT "
                             , const ConstraintHolder<P" .$p . "> &p" .$p; }

print OUT " )
    {
      return with_cs( ConstraintSetType( " . $args . " ) );
    }

  protected:

  /** Indicate a matcher for a set of arguments.
    * \@param constraintset   constraint set
    * \@return the builder object
    */
    virtual MatchBuilder<R, I>& with_cs( const ConstraintSetType &constraintset ) = 0;
};

";

}

print OUT "

MOCKPP_NS_END


#endif // MOCKPP_ARGUMENTSMATCHBUILDER_N_H

";

close OUT;

