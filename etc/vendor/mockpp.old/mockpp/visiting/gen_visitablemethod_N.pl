#!/usr/bin/perl -w
#
#   $Id: gen_visitablemethod_N.pl 1437 2008-03-08 20:12:53Z ewald-arnold $

use English;
use Cwd;

if ($#ARGV < 0) {
  print "Usage: perl gen_visitablemethod_N <number of arguments>\n";
  exit;
}


$totalNumArgs = $ARGV[0];
if ($totalNumArgs < 5) {
  $totalNumArgs = 5; }

$dir = getcwd;

for ($numArgs = 0; $numArgs <= $totalNumArgs; ++$numArgs) {

print "============================================================\n";
print "==> Creating file: $dir/VisitableMockMethod" . $numArgs . ".h\n";

  open OUT, ">VisitableMockMethod" . $numArgs . ".h";

print OUT
"/** \@file
    \@brief  Visitable Mock Method with $numArgs parameters.
            Generated with gen_visitablemethod_N.pl.

  \$I" . "d: VisitableMockMethod" . $numArgs . ".h,v 1.7 2005/10/19 20:53:09 ewald-arnold Exp \$

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Oct 22 2005
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

#ifndef MOCKPP_VisitableMockMethod" . $numArgs . "_H
#define MOCKPP_VisitableMockMethod" . $numArgs . "_H

#include <mockpp/mockpp.h>

#include <mockpp/visiting/VisitableMockObject.h>
#include <mockpp/visiting/VisitableMockMethod.h>";

if ($numArgs > 0){ print OUT "
#include <mockpp/visiting/ResponseVector" . $numArgs . ".h>"; }

print OUT "


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

  $args = "";
  for ($i = 1; $i <= $numArgs; ++$i) {
    if ($i > 1) {
      $args .= ", ";
    }
    $args .= "p$i";
  }

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
    $argTypes .= "typedef P$i p". ($i+1) . "_type; "; }

# print OUT "// argTypes: $argTypes\n\n\n";


print OUT
"/** Common stuff to set up visitable mock method expectations with " . $numArgs . " parameters.
  * \@ingroup grp_controller
  */
template <typename R" . $templateParms_colon . ">
class VisitableMockMethod" . $numArgs . "Common : public VisitableMockReturningMethodBase<R>
{
  public:

  /** Constructs the mock object.
    * \@param name    human readable description about the expectation
    * \@param parent  parent Visitable mock object
    */
    VisitableMockMethod" . $numArgs . "Common(const String &name, VisitableMockObject *parent)
      : VisitableMockReturningMethodBase<R>(name, parent)";

      if ($numArgs > 0)
{
print OUT "
      , responseThrowables(this->getMethodName() + MOCKPP_PCHAR(\"/responseThrowables\") , this)";
}

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
      , parameter" . $p . "(this->getMethodName() + MOCKPP_PCHAR(\"/parameter" . $p . "\"), this)"; }

print OUT "
    {
    }
";

if ($numArgs != 0) {
print OUT "

  /** Set up expectations with constraints.";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param p" . $p . " mock method parameter " . $p; }

print OUT "
    */
    void forward (" . $holderParms . ") const
    {
      MOCKPP_ASSERT_FALSE_MESSAGE(this->getVisitableMockObject()->getVerifiableName() + MOCKPP_PCHAR(\".isActivated() != true\"),
                                  this->getVisitableMockObject()->isActivated());
      this->getVisitableMockObject()->addExpectedMethod(this->getMethodIdentifier());";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
      parameter" . $p . ".addExpected(p" . $p . ");"; }

print OUT "
    }
";
}

print OUT "
  protected:
";

if ($numArgs > 0)
{
print OUT "
  /** Perform the internals to verify a mocked method or setup expectations.";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param p" . $p . " mock method parameter " . $p; }

print OUT "
    */
    void forward_param(" . $parms . ") const
    {
      if (!this->getVisitableMockObject()->isActivated() )
      {
        this->getVisitableMockObject()->addExpectedMethod(this->getMethodIdentifier());";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
        parameter" . $p . ".addExpected(p" . $p . ");"; }

print OUT "
      }

      else
      {
        try
        {
          this->getVisitableMockObject()->addActualMethod(this->getMethodIdentifier());

          Throwable *t;
          if (this->responseThrowables.find(t, " . $args . "))
             t->throw_me();

          this->throwAvailableException();
        }

        catch(...)
        {";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
          parameter" . $p . ".balanceActual();"; }

print OUT "
          MOCKPP_RETHROW;
        }
";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
        parameter" . $p . ".addActual(p" . $p . ");"; }

print OUT "
      }
    }
" ;
}
else
{
print OUT "
  /** Perform the internals to verify a mocked method.";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param p" . $p . " mock method parameter " . $p; }

print OUT "
    */
    void forward_param(" . $parms . ") const
    {
      if (!this->getVisitableMockObject()->isActivated() )
      {
        this->getVisitableMockObject()->addExpectedMethod(this->getMethodIdentifier());
      }
      else
      {
        this->getVisitableMockObject()->addActualMethod(this->getMethodIdentifier());
        this->throwAvailableException();
      }
    }";
}

print OUT "
  public:
";

if ($numArgs > 0)
{
print OUT "
  /** Adds another response throwable.
    * Response values are determined on the parameters you pass. This way the
    * object returns a value that is totally based on the input.
    * \@param t       the throwable object";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param p" . $p . " mock method parameter " . $p; }

print OUT "
    * \@param count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseThrowable(Throwable *t, " . $parms . ", unsigned count = MOCKPP_UNLIMITED)
    {
      MOCKPP_ASSERT_FALSE(this->getVisitableMockObject()->isActivated());
      this->responseThrowables.add(t, " . $args . ", count);
    }

  /** Adds another response throwable.
    * Response values are determined on the parameters you pass. This way the
    * object returns a value that is totally based on the input.
    * \@param t       the throwable object";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param p" . $p . " mock method parameter " . $p; }

print OUT "
    * \@param count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseThrowable(Throwable *t, " . $holderParms . ", unsigned count = MOCKPP_UNLIMITED)
    {
      MOCKPP_ASSERT_FALSE(this->getVisitableMockObject()->isActivated());
      this->responseThrowables.add(t, " . $args . ", count);
    }";
}


if ($numArgs > 0)
{
print OUT "

  private:

    mutable ResponseThrowableVector" . $numArgs . "<" . $templateArgs . ">   responseThrowables;
";
}

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    mutable ConstraintList<P" . $p . ">             parameter" . $p . ";"; }

print OUT "
};


/** Set up visitable mock method expectations with " . $numArgs . " parameters.
  * \@ingroup grp_controller
  */
template <typename R" . $templateParms_colon . ">
class VisitableMockMethod" . $numArgs . "
  : public VisitableMockMethod" . $numArgs . "Common<R" . $templateArgs_colon . ">
{
  public:

  /** Constructs the mock object.
    * \@param name    human readable description about the expectation
    * \@param parent  parent Visitable mock object
    */
    VisitableMockMethod" . $numArgs . "(const String &name, VisitableMockObject *parent)
      : VisitableMockMethod" . $numArgs . "Common<R" . $templateArgs_colon . ">(name, parent)";

if ($numArgs > 0)
{
print OUT "
      , responseValues(this->getMethodName() + MOCKPP_PCHAR(\"/responseValues\") , this)";
}

print OUT "
    {
    }

  /** Actually verifies the mocked method.
    * Must be called by the client code.";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param p" . $p . " mock method parameter " . $p; }

print OUT "
    */
    R forward(" . $parms . ") const
    {
      this->forward_param(" . $args . ");

      if (this->getVisitableMockObject()->isActivated() )
      {";

if ($numArgs > 0)
{
print OUT "
        R ret_val;
        if (this->responseValues.find(ret_val, " . $args . "))
          return ret_val;
";
}

print OUT "
        return this->determineReturnValue();
      }
      else
        return R(); // only dummy value to make the compiler happy
    }";

if ($numArgs > 0)
{
print OUT "

#if defined(__BORLANDC__) // ==> BCB5.5.1 ?? F1004 Internal compiler error at 0x12548c1 with base 0x1200000
  /** Set up expectations with constraints.";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param p" . $p . " mock method parameter " . $p; }

print OUT "
    */
    void forward (" . $holderParms . ") const
    {
       VisitableMockMethod" . $numArgs . "Common<R" . $templateArgs_colon . ">::forward(" . $args . ");
    }
#else
    using VisitableMockMethod" . $numArgs . "Common<R" . $templateArgs_colon . ">::forward;
#endif
";
}

if ($numArgs > 0)
{
print OUT "
  /** Adds another response value.
    * Response values are determined on the parameters you pass. This way the
    * object returns a value that is totally based on the input.
    * \@param rv      the return value";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param p" . $p . " mock method parameter " . $p; }

print OUT "
    * \@param count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseValue(const R &rv, " . $parms . ", unsigned count = MOCKPP_UNLIMITED)
    {
      MOCKPP_ASSERT_FALSE(this->getVisitableMockObject()->isActivated());
      this->responseValues.add(rv, " . $args . ", count);
    }

  /** Adds another response value.
    * Response values are determined on the parameters you pass. This way the
    * object returns a value that is totally based on the input.
    * \@param rv      the return value";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param p" . $p . " mock method parameter " . $p; }

print OUT "
    * \@param count   the number of times this value shall be returned. Default is unlimited.
    */
    void addResponseValue(const R &rv, " . $holderParms . ", unsigned count = MOCKPP_UNLIMITED)
    {
      MOCKPP_ASSERT_FALSE(this->getVisitableMockObject()->isActivated());
      this->responseValues.add(rv, " . $args . ", count);
    }";
}

if ($numArgs > 0)
{
print OUT "

  private:

    mutable ResponseVector" . $numArgs . "<R, " . $templateArgs . ">   responseValues;";
}

print OUT "
};


/** Set up visitable mock method expectations with " . $numArgs . " parameters.
  * Partial specialisation for a void return value.
  * \@ingroup grp_controller
  */
template <" . $templateParms . ">
class VisitableMockMethod" . $numArgs . "<void" . $templateArgs_colon . ">
   : public VisitableMockMethod" . $numArgs . "Common<void" . $templateArgs_colon . ">
{
  public:

  /** Constructs the mock object.
    * \@param name    human readable description about the expectation
    * \@param parent  parent Visitable mock object
    */
    VisitableMockMethod" . $numArgs . "(const String &name, VisitableMockObject *parent)
      : VisitableMockMethod" . $numArgs . "Common<void" . $templateArgs_colon . ">(name, parent)
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
       this->forward_param(" . $args . ");
    }";

if ($numArgs > 0)
{
print OUT "

#if defined(__BORLANDC__) // ==> BCB5.5.1 ?? F1004 Internal compiler error at 0x12548c1 with base 0x1200000
  /** Set up expectations with constraints.";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param p" . $p . " mock method parameter " . $p; }

print OUT "
    */
    void forward (" . $holderParms . ") const
    {
       VisitableMockMethod" . $numArgs . "Common<void" . $templateArgs_colon . ">::forward(" . $args . ");
    }
#else
    using VisitableMockMethod" . $numArgs . "Common<void" . $templateArgs_colon . ">::forward;
#endif";
}

print OUT "
};


MOCKPP_NS_END


#endif // MOCKPP_VisitableMockMethod" . $numArgs . "_H

";


  close OUT;
}

