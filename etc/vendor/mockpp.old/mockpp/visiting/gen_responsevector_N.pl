#!/usr/bin/perl -w
#
#   $Id: gen_responsevector_N.pl 1437 2008-03-08 20:12:53Z ewald-arnold $

use English;
use Cwd;

if ($#ARGV < 0) {
  print "Usage: perl gen_responsevector_N <number of arguments>\n";
  exit;
}


$totalNumArgs = $ARGV[0];
if ($totalNumArgs < 5) {
  $totalNumArgs = 5; }

$dir = getcwd;

for ($numArgs = 1; $numArgs <= $totalNumArgs; ++$numArgs) {

print "============================================================\n";
print "==> Creating file: $dir/ResponseVector" . $numArgs . ".h\n";

  open OUT, ">ResponseVector" . $numArgs . ".h";

print OUT
"/** \@file
    \@internal NOT INTENDED FOR PUBLIC INCLUSION
    \@brief    Generated with gen_responsevector_N.pl.

  \$I" . "d: ResponseVector" . $numArgs . ".h,v 1.7 2005/10/19 20:53:09 ewald-arnold Exp \$

 ***************************************************************************/

/**************************************************************************

   begin                : Sat Feb 15 2003
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

#ifndef MOCKPP_ResponseVector" . $numArgs . "_H
#define MOCKPP_ResponseVector" . $numArgs . "_H

#include <mockpp/constraint/ConstraintHolder.h>
#include <mockpp/constraint/IsEqual.h>
#include <mockpp/visiting/VisitableMockObject_template.h>


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
    $argTypes .= "typedef P$i p". ($i+1) . "_type; "; }



print OUT "
/** Class returning a throwable depending on the parameters
  * passed to a method.
  */
template <" . $templateParms . ">
class ResponseThrowableVector" . $numArgs . " : public ResponseVectorBase
{
  public:

  /**
    * Construct a new empty vector
    * \@param aName Label used to identify vector
    * \@param parent  parent verifiable
    */
    ResponseThrowableVector" . $numArgs . "(const String &aName, VerifiableList *parent)
      : ResponseVectorBase(aName, parent)
    {}

  /** Destroys the vector
    */
    virtual ~ResponseThrowableVector" . $numArgs . "()
    {
      reset();
    }

  /** Adds a throwable.
    * \@param t      the throwable";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param p" . $p . "     the parameter " . $p . " upon which to return"; }

print OUT "
    * \@param count  the number of times the object may be used
    */
    void add(Throwable *throwable, " . $parms . ", unsigned count)
    {";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
      const ConstraintHolder<P" . $p . "> h" . $p . " = new IsEqual<P" . $p . ">(p" . $p . ");" }

print OUT "
      add(throwable, " . $hArgs . ", count);
    }

  /** Adds a throwable.
    * \@param throwable      the throwable";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param p" . $p . "     the parameter " . $p . " upon which to return"; }

print OUT "
    * \@param count  the number of times the object may be used
    */
    void add(Throwable *throwable,";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
             const ConstraintHolder<P" . $p . "> &p" . $p . ","; }

print OUT "
             unsigned count)
    {
      counter.push_back(count);
      tvec.push_back(throwable);";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
      typename MOCKPP_NS::Constraint<P" . $p . ">::AP cons" . $p . " (p" . $p . ");"; }

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
      t" . $p . "vec.push_back(cons" . $p . ".release());"; }

print OUT "
    }

  /** Removes all the objects from the list.
    */
    void reset()
    {
      counter.clear();
      tvec.reset();";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "

      for (unsigned i" . $p . " = 0; i" . $p . " < t" . $p . "vec.size(); ++i" . $p . ")
        delete t" . $p . "vec[i" . $p . "];
      t" . $p . "vec.clear();"; }

print OUT "
    }

  /** Finds a throwable for a set of parameters.
    * \@param t      the throwable";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param p" . $p . "     the parameter " . $p . " upon which to return"; }

print OUT "
    * \@return true: throwable found for parameters
    */
    bool find(Throwable * &throwable, " . $parms . ")
    {
       for (unsigned i = 0; i < t1vec.size(); ++i)
         if (   counter[i] > 0";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
             && t" . $p . "vec[i]->eval(p" . $p . ")"; }

print OUT "
            )
         {
           if (tvec.at(i) == 0)
             return false;

           if (counter[i] != MOCKPP_UNLIMITED)
             --counter[i];

           throwable = tvec.at(i);
           return true;
         }
       return false;
    }

  protected:
";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    MOCKPP_STL::vector<Constraint<P" . $p . ">*>       t" . $p . "vec; //!< shared internal member"; }

print OUT "
};


/** Class returning a throwable depending on the parameters
  * passed to a method.
  */
template <typename R, " . $templateParms . ">
class ResponseVector" . $numArgs . "  : public ResponseThrowableVector" . $numArgs . "<" . $templateArgs . ">
{
  public:

  /** Construct a new empty vector
    * \@param aName Label used to identify vector
    * \@param parent  parent verifiable
    */
    ResponseVector" . $numArgs . "(const String &aName, VerifiableList *parent)
      : ResponseThrowableVector" . $numArgs . "<" . $templateArgs . ">(aName, parent)
    {}

  /** Adds a throwable.
    * \@param t      the throwable";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param p" . $p . "     the parameter " . $p . " upon which to return"; }

print OUT "
    * \@param count  the number of times the object may be used
    */
    void add(Throwable *throwable, " . $parms . ", unsigned count)
    {
      MOCKPP_ASSERT_TRUE(throwable != 0);
      ResponseThrowableVector" . $numArgs . "<" . $templateArgs . ">::add(throwable, " . $args . ", count);
      R r;
      rvec.push_back(r);
    }

  /** Adds a throwable.
    * \@param t      the throwable";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param p" . $p . "     the parameter " . $p . " upon which to return"; }

print OUT "
    * \@param count  the number of times the object may be used
    */
    void add(Throwable *throwable,";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
             const ConstraintHolder<P" . $p . "> &p" . $p . ","; }

print OUT "
             unsigned count)
    {
      MOCKPP_ASSERT_TRUE(throwable != 0);
      ResponseThrowableVector" . $numArgs . "<" . $templateArgs . ">::add (throwable, " . $args . ", count);
      R r;
      rvec.push_back(r);
    }

  /** Adds a return value.
    * \@param r      the return value";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param p" . $p . "     the parameter " . $p . " upon which to return"; }

print OUT "
    * \@param count  the number of times the object may be used
    */
    void add(const R &r, " . $parms . ", unsigned count)
    {
      ResponseThrowableVector" . $numArgs . "<" . $templateArgs . ">::add((Throwable*)0, " . $args . ", count);
      rvec.push_back(r);
    }

  /** Adds a return value.
    * \@param r      the return value";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param p" . $p . "     the parameter " . $p . " upon which to return"; }

print OUT "
    * \@param count  the number of times the object may be used
    */
    void add(const R &r,";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
             const ConstraintHolder<P" . $p . "> &p" . $p . ","; }

print OUT "
             unsigned count)
    {
      ResponseThrowableVector" . $numArgs . "<" . $templateArgs . ">::add((Throwable*)0, " . $args . ", count);
      rvec.push_back(r);
    }

  /** Removes all the objects from the list.
    */
    void reset()
    {
      ResponseThrowableVector" . $numArgs . "<" . $templateArgs . ">::reset();
      rvec.clear();
    }

#if defined(__BORLANDC__) || (__GNUC__ < 3)   // ==> BCB5.5.1 ?? F1004 Internal compiler error at 0x12548c1 with base 0x1200000
    bool find(Throwable * &throwable, " . $parms . ")
    {
      return ResponseThrowableVector" . $numArgs . "<" . $templateArgs . ">::find(throwable, " . $args . ");
    }
#else
    using ResponseThrowableVector" . $numArgs . "<" . $templateArgs . ">::find;
#endif

  /** Finds a return value for a set of parameters.
    * \@param r      the value";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
    * \@param p" . $p . "     the parameter " . $p . " upon which to return"; }

print OUT "
    * \@return true: return value found for parameters
    */
    bool find(R &r, " . $parms . ")
    {
       for (unsigned i = 0; i < this->t1vec.size(); ++i)
         if (   this->counter[i] > 0";

for($p = 1; $p <= $numArgs; ++$p) { print OUT "
             && this->t" . $p . "vec[i]->eval(p" . $p . ")"; }

print OUT "
            )
         {
           if (this->tvec.at(i) != 0)
             return false;

           if (this->counter[i] != MOCKPP_UNLIMITED)
             --this->counter[i];

           r = rvec[i];
           return true;
         }
       return false;
    }

  private:

    MOCKPP_STL::vector<R>        rvec;
};

MOCKPP_NS_END


#endif // MOCKPP_ResponseVector" . $numArgs . "_H

";


  close OUT;
}

