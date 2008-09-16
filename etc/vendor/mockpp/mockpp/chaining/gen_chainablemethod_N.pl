#!/usr/bin/perl -w
#
#   $Id: gen_chainablemethod_N.pl 1437 2008-03-08 20:12:53Z ewald-arnold $

use English;
use Cwd;

if ($#ARGV < 0) {
  print "Usage: perl gen_Chainablemethod_N <number of arguments>\n";
  exit;
}


$totalNumArgs = $ARGV[0];
if ($totalNumArgs < 5) {
  $totalNumArgs = 5; }

for ($numArgs = 0; $numArgs <= $totalNumArgs; ++$numArgs) {
  open OUT, ">ChainableMockMethod" . $numArgs . ".h";

$dir = getcwd;
print "============================================================\n";
print "==> Creating file: $dir/ChainableMockMethod" . $numArgs . ".h\n";

print OUT
"/** \@file
    \@brief  Chainable Mock Method with $numArgs parameters.
            Generated with gen_chainablemethod_N.pl.

  \$I" . "d: ChainableMockMethod" . $numArgs . ".h,v 1.7 2005/10/19 20:53:09 ewald-arnold Exp \$

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Oct 2 2005
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

#ifndef MOCKPP_ChainableMockMethod" . $numArgs . "_H
#define MOCKPP_ChainableMockMethod" . $numArgs . "_H

#include <mockpp/mockpp.h>

#include <mockpp/chaining/ChainableMockObject.h>
#include <mockpp/chaining/ChainableMockMethod.h>


MOCKPP_NS_START


";

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

# print OUT "// templateParms: $templateParms\n";

  $templateArgs_colon = "";
  $_ = $templateParms;
  s/typename //g;
  $templateArgs = $_;
  if ($numArgs > 0) {
    $templateArgs_colon = ", ". $templateArgs; }

# print OUT "// templateArgs: $templateArgs\n";

  $parms = "";
  for ($i = 1; $i <= $numArgs; ++$i) {
    if ($i > 1) {
      $parms .= ", ";
    }
    $parms .= "const P$i &p$i";
  }

# print OUT "// parms: $parms\n";

  $holderParms = "";
  for ($i = 1; $i <= $numArgs; ++$i) {
    if ($i > 1) {
      $holderParms .= ", ";
    }
    $holderParms .= "const ConstraintHolder<P$i> &p$i";
  }

# print OUT "// holderParms: $holderParms\n";

  $args_colon = "";
  $args = "";
  for ($i = 1; $i <= $numArgs; ++$i) {
    if ($i > 1) {
      $args .= ", ";
    }
    $args .= "p$i";
  }
  if ($numArgs > 0) {
    $args_colon = ", ". $args; }

# print OUT "// args: $args\n";

  $boundArgs = "";
  for ($i = 1; $i <= $numArgs; ++$i) {
    if ($i > 1) {
      $boundArgs .= ", ";
    }
    $boundArgs .= "args->a$i";
  }

# print OUT "// boundArgs: $boundArgs\n";

  $argsAsMembers = "";
  for ($i = 1; $i <= $numArgs; ++$i) {
    $argsAsMembers .= "P$i p$i;";
  }

# print OUT "// argsAsMembers: $argsAsMembers\n";

  $copyParms = "";
  for ($i = 1; $i <= $numArgs; ++$i) {
    if ($i > 1) {
      $copyParms .= ", ";
    }
    $copyParms .= "P$i ia$i";
  }

# print OUT "// copyParms: $copyParms\n";

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

# print OUT "// initArgs: $initArgs\n";

  $argTypes = "";
  for ($i = 1; $i <= $numArgs; ++$i) {
    $argTypes .= "typedef P$i p". ($i+1) . "_type; //!< internal shorthand"; }

# print OUT "// argTypes: $argTypes\n\n\n";


print OUT
"/** Common stuff to set up chainable mock method expectations with " . $numArgs . " parameters.
  * \@ingroup grp_chainer
  * \@internal
  */
template <typename RT" . $templateParms_colon . ">
class ChainableMockMethod" . $numArgs . "Common
  : public ChainableMockMethodCommon";

if ($numArgs > 0)
{
print OUT "
  , public ChainingMockBuilder <ArgumentsMatchBuilder" . $numArgs . "<RT, Invocation" . $numArgs . "<" . $templateArgs . "> > >";
}
else
{
print OUT "
  , public ChainingMockBuilder <ArgumentsMatchBuilder0<RT, Invocation0> >";
}

print OUT "
{
  public:
";

if ($numArgs > 0)
{
print OUT "
    typedef Invocation" . $numArgs . "<" . $templateArgs . ">                InvocationType;  //!< internal shorthand";
}
else
{
print OUT "
    typedef Invocation0                                       InvocationType; //!< internal shorthand";
}

print OUT "

    typedef CoreMock<RT, InvocationType>                      CoreMockType;   //!< internal shorthand
    typedef RT                                                ReturnType;     //!< internal shorthand

    typedef ChainingMockBuilder <ArgumentsMatchBuilder" . $numArgs . "<ReturnType, InvocationType> > Builder; //!< internal shorthand

  /** Constructs the mock object.
    * \@param name    human readable description about the expectation
    * \@param parent  parent chainable mock object
    */
    ChainableMockMethod" . $numArgs . "Common(const String &name, ChainableMockObject *parent )
      : ChainableMockMethodCommon(name, parent)
      , Builder(&coremock, this->getChainableMockObject(), this->getMethodName())
      , coremock(name + MOCKPP_PCHAR(\"/coreMock\"), this)
    {
    }

  protected:

  /** Returns the underlying coremock.
    * \@return reference to the mock object
    */
    CoreMockType & getCoreMock() const
    {
      return coremock;
    }

  private:

    mutable CoreMockType    coremock;
};


/** Set up a chainable mock method expectations with " . $numArgs . " parameters.
  * \@ingroup grp_chainer
  */
template <typename RT" . $templateParms_colon . ">
class ChainableMockMethod" . $numArgs . "
  : public ChainableMockMethod" . $numArgs . "Common<RT" . $templateArgs_colon . ">
{
  public:

    typedef typename ChainableMockMethod" . $numArgs . "Common<RT" . $templateArgs_colon . ">::InvocationType   InvocationType;  //!< internal shorthand

  /** Constructs the mock object.
    * \@param name    human readable description about the expectation
    * \@param parent  parent chainable mock object
    */
    ChainableMockMethod" . $numArgs . "(const String &name, ChainableMockObject *parent )
      : ChainableMockMethod" . $numArgs . "Common<RT" . $templateArgs_colon . ">(name, parent)
    {
    }

  /** Actually verifies the mocked method.
    * Must be called by the client code.";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param p" . $p . " mock method parameter " . $p; }

print OUT "
    * \@return the calculated value
    */
    RT forward(" . $parms . ") const
    {
      InvocationType inv(this->getMethodName()" . $args_colon . ");
      return this->getCoreMock().invoke(inv);
    }
};


/** Set up a chainable mock method expectations with " . $numArgs . " parameters.
  * Partial specialisation for a void return value.
  * \@ingroup grp_chainer
  */
template <" . $templateParms . ">
class ChainableMockMethod" . $numArgs . "<void" . $templateArgs_colon . ">
  : public ChainableMockMethod" . $numArgs . "Common<void" . $templateArgs_colon . ">
{
  public:
 ";

if ($numArgs > 0)
{
print OUT "
    typedef typename ChainableMockMethod" . $numArgs . "Common<void" . $templateArgs_colon . ">::InvocationType   InvocationType;  //!< internal shorthand";
}
else
{
print OUT "
    typedef ChainableMockMethod" . $numArgs . "Common<void" . $templateArgs_colon . ">::InvocationType   InvocationType;  //!< internal shorthand";
}

print OUT "

  /** Constructs the mock object.
    * \@param name    human readable description about the expectation
    * \@param parent  parent chainable mock object
    */
    ChainableMockMethod" . $numArgs . "(const String &name, ChainableMockObject *parent )
      : ChainableMockMethod" . $numArgs . "Common<void" . $templateArgs_colon . ">(name, parent)
    {
    }

  /** Actually verifies the mocked method.
    * Must be called by the client code.";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param p" . $p . " mock method parameter " . $p; }

print OUT "
    */
    void forward(" . $parms . ") const
    {
      InvocationType inv(this->getMethodName()" . $args_colon . ");
      this->getCoreMock().invoke(inv);
    }
};


MOCKPP_NS_END


#endif // MOCKPP_ChainableMockMethod" . $numArgs . "_H

";


  close OUT;
}

