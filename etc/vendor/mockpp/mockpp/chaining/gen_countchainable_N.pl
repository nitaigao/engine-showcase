#!/usr/bin/perl -w
#
#   $Id: gen_countchainable_N.pl 1437 2008-03-08 20:12:53Z ewald-arnold $

use English;
use Cwd;

if ($#ARGV < 0) {
  print "Usage: perl gen_countchainable_N <number of arguments>\n";
  exit;
}


$totalNumArgs = $ARGV[0];
if ($totalNumArgs < 5) {
  $totalNumArgs = 5; }


$dir = getcwd;
print "============================================================\n";
print "==> Creating file: $dir/CountedChainableMethod.h\n";

open OUT, ">CountedChainableMethod.h";

print OUT
"/** \@file
    \@internal NOT INTENDED FOR PUBLIC INCLUSION
    \@brief    Generated with gen_countchainable_N.pl.

  \$I" . "d: CountedChainableMethod.h,v 1.7 2005/10/19 20:53:09 ewald-arnold Exp \$

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

#ifndef MOCKPP_COUNTEDCHAINABLEMETHOD_H
#define MOCKPP_COUNTEDCHAINABLEMETHOD_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/CountParameters.h>
";

for ($p = 0; $p <= $totalNumArgs; ++$p) { print OUT "
#include <mockpp/chaining/ChainableMockMethod" . $p . ".h>";}

print OUT "


MOCKPP_NS_START


#ifndef MOCKPP_COUNTED_WEAKNESS


/** Helper class to select the method mock with the correct number of parameters.
  * Works with meta programming.
  * \@internal
  */
template<unsigned params>
class SwitchChainable
{
};

";



for ($numArgs = 0; $numArgs <= $totalNumArgs; ++$numArgs) {

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

print "Creating CountedChainableMethod" . $numArgs ."\n";

print OUT "
/** Helper class to select the method mock with " . $numArgs ." parameters.
  * Works with meta programming.
  * \@internal
  */
template<>
class SwitchChainable<" . $numArgs .">
{
  public:
";

print OUT "
  /** Helper class with an embeded type to the the method mock with " . $numArgs ." parameters.
    */
    template<  typename RT";

for($p = 1; $p <= $totalNumArgs; ++$p) { print OUT "
             , typename P" . $p; }

print OUT ">
    class Method
    {
      public:

        typedef ChainableMockMethod" . $numArgs ."<RT" . $templateArgs_colon . "> ChainableMockMethodType; //!< internal shorthand
    };
};

";

}


print OUT "
/** Method mock class to select the actual class with the desired number of parameters.
  * Works with meta programming.
  */
template<   typename RT
          , typename P1 = NoParameter";

for($p = 2; $p <= $totalNumArgs; ++$p) { print OUT "
          , typename P" . $p . " = NoParameter"; };

print OUT ">
class ChainableMockMethod
  : public SwitchChainable<CountParameters<RT, " . $templateArgs . ">::value >
#ifdef MOCKPP_NO_TEMPLATE_HINT
             ::Method<RT, " . $templateArgs . ">
#else
             ::template Method<RT, " . $templateArgs . ">
#endif
               ::ChainableMockMethodType
{
  public:

  /** Constructs the mock object.
    * \@param name    human readable description about the expectation
    * \@param parent  parent chainable mock object
    */
    ChainableMockMethod(const String &name, ChainableMockObject *parent )
      : SwitchChainable<CountParameters<RT, " . $templateArgs . ">::value >
#ifdef MOCKPP_NO_TEMPLATE_HINT
          ::Method<RT, " . $templateArgs . ">
#else
          ::template Method<RT, " . $templateArgs . ">
#endif
            ::ChainableMockMethodType(name, parent)
    {
    }
};


#else // MOCKPP_COUNTED_WEAKNESS


/** Helper class to select the method mock with the correct number of parameters.
  * Works with partial specialisation.
  * \@internal
  */
template<   typename RT
          , typename P1 = NoParameter";

for($p = 2; $p <= $totalNumArgs+1; ++$p) { print OUT "
          , typename P" . $p . " = NoParameter"; };

print OUT "> // one more parameter to enabled full specialisation
class ChainableMockMethod
{
};

";

for ($numArgs = 0; $numArgs <= $totalNumArgs; ++$numArgs) {

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

  $noParamArgs = $templateArgs;
  for ($i = $numArgs+1; $i <= $totalNumArgs; ++$i) {
    if ($i > 1) {
      $noParamArgs .= ", ";
    }
    $noParamArgs .= "NoParameter";
  }


print OUT "
/** Method mock class to select the actual class with " . $numArgs . " parameters.
  * Works with partial specialisation.
  */
template<typename RT" . $templateParms_colon . ">
class ChainableMockMethod<RT, " . $noParamArgs . ">
  : public ChainableMockMethod" . $numArgs . "<RT" . $templateArgs_colon . ">
{
  public:

  /** Constructs the mock object.
    * \@param name    human readable description about the expectation
    * \@param parent  parent chainable mock object
    */
    ChainableMockMethod<RT, " . $noParamArgs . ">
          (const String &name, ChainableMockObject *parent )
      : ChainableMockMethod" . $numArgs . "<RT" . $templateArgs_colon . ">(name, parent)
    {
    }
};

";

}

print OUT "
#endif // MOCKPP_COUNTED_WEAKNESS


MOCKPP_NS_END


#endif // MOCKPP_COUNTEDCHAINABLEMETHOD_H

";

close OUT;

