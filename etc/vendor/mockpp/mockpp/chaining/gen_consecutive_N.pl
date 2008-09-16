#!/usr/bin/perl -w
#
#   $Id: gen_consecutive_N.pl 1437 2008-03-08 20:12:53Z ewald-arnold $

use English;
use Cwd;

if ($#ARGV < 0) {
  print "Usage: perl gen_consecutive_N <number of arguments>\n";
  exit;
}


$totalNumArgs = $ARGV[0];
if ($totalNumArgs < 10) {
  $totalNumArgs = 10; }

$dir = getcwd;
print "============================================================\n";
print "==> Creating file: $dir/OnConsecutiveCalls.h\n";

open OUT, ">OnConsecutiveCalls.h";

print OUT
"/** \@file
    \@internal NOT INTENDED FOR PUBLIC INCLUSION
    \@brief    Generated with gen_consecutive_N.pl.

  \$I" . "d: OnConsecutiveCalls.h,v 1.7 2006/04/07 20:53:09 ewald-arnold Exp \$

 ***************************************************************************/

/**************************************************************************

   begin                : Fri Apr 7 2006
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

#ifndef MOCKPP_ONCONSECUTIVECALLS_H
#define MOCKPP_ONCONSECUTIVECALLS_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/stub/StubSequence.h>
#include <mockpp/stub/TypelessStubSequence.h>


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

print "Creating onConsectiveCalls " . $numArgs ."\n";

###################################################################

print OUT "
/** Creates a sequence of "  . $numArgs . " stubs.
  * \@ingroup grp_stub_abbrev
  * \@see mockpp::StubSequence
  * \@param  stub1  first stub";

for($p = 2; $p <= $numArgs; ++$p) { print OUT "
  * \@param  stub"  . $p . "  the next stub"; }


print OUT "
  * \@return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  AutoPointer<Stub<R, I> > stub1";

for($p = 2; $p <= $numArgs; ++$p) { print OUT "
                                           , AutoPointer<Stub<R, I> > stub" . $p; }

print OUT "
                                          )
{
  MOCKPP_STL::vector<Stub<R, I>*> vec;";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
  vec.push_back( stub" . $p . ".release());" };

print OUT "
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}

";

###################################################################

print OUT "
/** Creates a sequence of "  . $numArgs . " stubs.
  * \@ingroup grp_stub_abbrev
  * \@see mockpp::StubSequence
  * \@param  stub1  first stub";

for($p = 2; $p <= $numArgs; ++$p) { print OUT "
  * \@param  stub"  . $p . "  the next stub"; }


print OUT "
  * \@return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  Stub<R, I> *stub1";

for($p = 2; $p <= $numArgs; ++$p) { print OUT "
                                           , Stub<R, I> *stub" . $p; }

print OUT "
                                          )
{
  MOCKPP_STL::vector<Stub<I, R>*> vec;";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
  vec.push_back( stub" . $p . ");" };

print OUT "
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}

";

###################################################################

print OUT "
/** Creates a sequence of "  . $numArgs . " typeless stubs.
  * \@ingroup grp_stub_abbrev
  * \@see mockpp::TypelessStubSequence
  * \@param  stub1  first stub";

for($p = 2; $p <= $numArgs; ++$p) { print OUT "
  * \@param  stub"  . $p . "  the next stub"; }


print OUT "
  * \@return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  AutoPointer<TypelessStub<R> > stub1";

for($p = 2; $p <= $numArgs; ++$p) { print OUT "
                                                , AutoPointer<TypelessStub<R> > stub" . $p; }

print OUT "
                                               )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
  vec.push_back( stub" . $p . ".release());" };

print OUT "
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}

";

###################################################################

print OUT "
/** Creates a sequence of "  . $numArgs . " typeless stubs.
  * \@ingroup grp_stub_abbrev
  * \@see mockpp::TypelessStubSequence
  * \@param  stub1  first stub";

for($p = 2; $p <= $numArgs; ++$p) { print OUT "
  * \@param  stub"  . $p . "  the next stub"; }


print OUT "
  * \@return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  TypelessStub<R> *stub1";

for($p = 2; $p <= $numArgs; ++$p) { print OUT "
                                                , TypelessStub<R> *stub" . $p; }

print OUT "
                                          )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
  vec.push_back( stub" . $p . ");" };

print OUT "
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}

";

###################################################################

}

print OUT "
MOCKPP_NS_END


#endif // MOCKPP_ONCONSECUTIVECALLS_H

";

close OUT;

