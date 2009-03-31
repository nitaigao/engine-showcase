/***************************************************************************
             MockppIdlParser.cpp  -  parse mockpp idl files
                             -------------------
    begin                : Sun May 27 2007
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp@ewald-arnold.de

    $Id: MockppIdlClass.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/


/**************************************************************************
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 ***************************************************************************/


#include "MockppIdlClass.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <iostream>


Type::Type()
{
}


void Type::setProperty(const std::string &in_left, const std::string &in_right)
{
  left = in_left;
  right = in_right;
}


void Type::setName(const std::string &in_name)
{
  name = in_name;
}


std::string Type::getLeft() const
{
  return left;
}


std::string Type::getRight() const
{
  return right;
}


bool Type::isReference() const
{
  return  getRight().find("&") != std::string::npos;
}


std::string Type::getName() const
{
  return name;
}


void Type::clear()
{
  name.clear();
  left.clear();
  right.clear();
}


std::string Type::getCppString(unsigned indent) const
{
  std::string ret;

  if (left.length() != 0)
    ret += left +  " ";

  ret += name;

  if (right.length() != 0)
    ret += " " + right;

  return ret;
}


std::string Type::getContraintType() const
{
  static std::string sconst = "const";
  static unsigned lconst = sconst.length();

  const bool reftype = isReference();
  std::string sleft = left;
  std::string ret;

  unsigned pos;
  while ((pos = sleft.find(sconst)) != std::string::npos)
    sleft.erase(pos, lconst);

  if (sleft.length() != 0)
    ret += sleft +  " ";

  ret += name;

  if (!reftype && right.length() != 0)
    ret += " " + right;

  return ret;
}


//////////////////////////////////////////////////////////


Argument::Argument()
{
}


void Argument::setName(const std::string &in_name)
{
  name = in_name;
}


std::string Argument::getConstCastedName() const
{
  return Argument::getConstCastedName(name, getType());
}


std::string Argument::getConstCastedName(const std::string &name,
                                         const Type &type,
                                         const std::string &prefix,
                                         const std::string &suffix)
{
  static std::string sconst = "const";
  static unsigned lconst = sconst.length();

  bool consttype = false;

  std::string sleft = type.getLeft();
  unsigned pos;
  while ((pos = sleft.find(sconst)) != std::string::npos)
  {
    consttype = true;
    sleft.erase(pos, lconst);
  }

  std::string sright = type.getRight();
  while ((pos = sright.find(sconst)) != std::string::npos)
  {
    consttype = true;
    sright.erase(pos, lconst);
  }

  if (!consttype)
    return name;

  while ((pos = sright.find("&")) != std::string::npos)
    sright.erase(pos, 1);

//  return std::string("const_cast<") + prefix + sleft + " " + type.getName() + " " + sright + suffix + ">(" + name + ")";
  return std::string("(") + prefix + sleft + " " + type.getName() + " " + sright + suffix + ")(" + name + ")";
}


std::string Argument::getName() const
{
  return name;
}


void Argument::setType(const Type &in_type)
{
  type = in_type;
}


Type Argument::getType() const
{
  return type;
}


std::string Argument::getCppString(unsigned indent) const
{
  return type.getCppString(indent) + " " + name;
}


void Argument::clear()
{
  type.clear();
  name.clear();
}


//////////////////////////////////////////////////////////


Method::Method()
{
}


void Method::setProperty(bool constructor,
                         bool in_virtual,
                         bool in_const,
                         const std::string & in_throw_decl)
{
  isvirtual = in_virtual;
  isconst = in_const;
  isconstructor = constructor;
  throw_decl = in_throw_decl;
}


void Method::setName(const std::string &in_classname, const std::string &in_name)
{
  name = in_name;
  overloadname = in_name;
  classname = in_classname;
}


void Method::setName(const std::string &in_name)
{
  name = in_name;
  overloadname = in_name;
}


std::string Method::getName() const
{
  return name;
}


void Method::setOverloadName(const std::string &in_name)
{
  overloadname = in_name;
}


std::string Method::getOverloadName() const
{
  return overloadname;
}


void Method::setType(const Type &in_type)
{
  type = in_type;
}


Type Method::getType() const
{
  return type;
}


void Method::addArgument(const Argument &arg)
{
  args.push_back(arg);
}


std::string Method::getTemplateSignature() const
{
  static std::string sconst = "const";
  static unsigned lconst = sconst.length();

  std::string s;
  for (unsigned i = 0; i < args.size(); ++i)
  {
    if (i != 0)
      s += ", ";

    bool reftype = args[i].getType().getRight().find("&") != std::string::npos;
    std::string left = args[i].getType().getLeft();

    unsigned pos;
    while ((pos = left.find(sconst)) != std::string::npos)
      left.erase(pos, lconst);

    if (left.length() != 0)
      s += left +  " ";

    s += args[i].getType().getName();

    std::string right = args[i].getType().getRight();

    while ((pos = right.find(sconst)) != std::string::npos)
      right.erase(pos, lconst);

    if (!reftype && right.length() != 0)
      s += " " + right;
  }

  //

  std::string left = getType().getLeft();
  std::string right = getType().getRight();
  bool reftype = right.find("&") != std::string::npos;

  std::string ret;

  unsigned pos;
  if ((pos = left.find(sconst)) != std::string::npos)
    left.erase(pos, lconst);

  if (left.length() != 0)
    ret += left +  " ";

  ret += getType().getName();

  while ((pos = right.find(sconst)) != std::string::npos)
    right.erase(pos, lconst);

  if (!reftype && right.length() != 0)
    ret += " " + right;

  if (s.length() != 0)
    ret += std::string(", ") + s;

  return ret;
}


std::string Method::getSignature() const
{
  std::string ret;

  for (unsigned i = 0; i < args.size(); ++i)
  {
    if (i != 0)
      ret += ", ";

    ret += args[i].getType().getCppString(0);
  }

  return ret;
}


std::string Method::getCppString(unsigned indent, bool impl, const std::string &suffix) const
{
  std::string ret;

  if (isvirtual && !impl)
    ret += "virtual ";

  if (!isconstructor)
    ret += type.getCppString(0) + " ";

  else
    ret += "/*ctor*/ ";

  if (impl)
    ret += classname + suffix + "::" + name;
  else
    ret += name;

  ret += "(";

  for (unsigned i = 0; i < args.size(); ++i)
  {
    if (i != 0)
      ret += ", ";

    ret += args[i].getCppString(0);
  }

  ret += ")";

  if (isconst)
    ret += " const";
//
  if (throw_decl.length() != 0)
    ret += std::string(" throw(") + throw_decl + ")";

  if (!impl)
    ret += ";";

  return ret;
}


bool Method::isConstructor() const
{
  return isconstructor;
}


bool Method::isVirtual() const
{
  return isvirtual;
}


unsigned Method::numArgs() const
{
  return args.size();
}


Argument Method::getArg(unsigned i) const
{
  return args[i];
}


bool Method::operator<(const Method &rhs) const
{
  return name < rhs.name;
}


void Method::clear()
{
  args.clear();
  name.clear();
  overloadname.clear();
  classname.clear();
}


//////////////////////////////////////////////////////////


MockppIdlClass::MockppIdlClass()
{
}


void MockppIdlClass::setName(const std::string &in_name)
{
  name = in_name;
}


std::string MockppIdlClass::getName() const
{
  return name;
}


void MockppIdlClass::addMethod(const Method &method)
{
  if (method.isConstructor())
    ctors.push_back(method);

  else
    methods.push_back(method);

  unsigned cnt = method.numArgs();
  if (std::find(argsCnt.begin(), argsCnt.end(), cnt) == argsCnt.end())
    argsCnt.push_back(cnt);
}


std::vector<unsigned> MockppIdlClass::getArgsCount() const
{
  return argsCnt;
}


void MockppIdlClass::clear()
{
  methods.clear();
  ctors.clear();
  argsCnt.clear();
}


unsigned MockppIdlClass::numMethods() const
{
  return methods.size();
}


Method MockppIdlClass::getMethod(unsigned i) const
{
  return methods[i];
}


unsigned MockppIdlClass::numCtors() const
{
  return ctors.size();
}


Method MockppIdlClass::getCtor(unsigned i) const
{
  return ctors[i];
}


struct MethodComp
{
  bool operator()(const Method *lhs, const Method *rhs)
  {
    return lhs->getName() < rhs->getName();
  }
};


void OverloadRename(Method *method, unsigned ctr)
{
   std::stringstream ss;
   ss << method->getName() << "_ovr" << ctr;
   std::cout << "Method overload remapped: "
             << method->getCppString(0, true, "") << " --> "
             << ss.str() << std::endl;
   method->setOverloadName(ss.str());
}


void MockppIdlClass::resolveOverloaded()
{
   // Workaround BCB5: intermediate vector with pointer

   std::vector<Method*> overp;
   for (unsigned i = 0; i < methods.size(); ++i)
     overp.push_back(&methods[i]);

   if (overp.size() != 0)
   {
     std::sort(overp.begin(), overp.end(), MethodComp());

     for (unsigned i = 0; i < overp.size(); ++i)
     {
       std::string s = overp[i]->getName();
       unsigned ctr = 0;
       if (i < overp.size()-1 && s == overp[i+1]->getName())
       {
         OverloadRename(overp[i], ctr);
         ++ctr;
         ++i;

         while (i < overp.size() && s == overp[i]->getName())
         {
           OverloadRename(overp[i], ctr);
           ++ctr;
           ++i;
         }
       }
     }
   }
}


std::string MockppIdlClass::getSource() const
{
  return sourcefile;
}


void MockppIdlClass::setSource(const std::string &in_name)
{
  sourcefile = in_name;
}

