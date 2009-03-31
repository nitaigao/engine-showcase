/***************************************************************************
             MockppIdlClass.h  -  parse mockpp idl files
                             -------------------
    begin                : Sun May 20 2007
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp@ewald-arnold.de

    $Id: MockppIdlClass.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_IDL_CLASS_H
#define MOCKPP_IDL_CLASS_H

#include <string>
#include <vector>


//  <TYPE  qleft="const" qright="*">QString</TYPE>
struct Type
{
    Type();

    void setProperty(const std::string &in_left, const std::string &in_right);

    void setName(const std::string &in_name);

    void clear();

    std::string getCppString(unsigned indent) const;

    std::string getContraintType() const;

    std::string getLeft() const;

    std::string getRight() const;

    bool isReference() const;

    std::string getName() const;

  private:

    std::string   name;
    std::string   left;
    std::string   right;
};


//   <ARG><TYPE>int</TYPE><NAME>num</NAME></ARG>
struct Argument
{
    Argument();

    void setName(const std::string &in_name);

    std::string getName() const;

    static std::string getConstCastedName(const std::string &name,
                                          const Type &type,
                                          const std::string &prefix = "",
                                          const std::string &suffix = "");

    std::string getConstCastedName() const;

    void setType(const Type &in_type);

    Type getType() const;

    void clear();

    std::string getCppString(unsigned indent) const;

  private:

    std::string   name;
    Type          type;
};


/*
  <METHOD virtual='1'>
    <TYPE>DCOPRef</TYPE>
    <NAME>getObject</NAME>
    <ARG><TYPE>int</TYPE><NAME>num</NAME></ARG>
  </METHOD>
*/
struct Method
{
    Method();

    void setProperty(bool constructor,
                     bool in_virtual,
                     bool in_const,
                     const std::string & in_throw_decl);

    void setName(const std::string &in_classname, const std::string &in_name);

    void setName(const std::string &in_name);

    std::string getName() const;

    void setOverloadName(const std::string &in_name);

    std::string getOverloadName() const;

    void setType(const Type &in_type);

    Type getType() const;

    void addArgument(const Argument &arg);

    std::string getCppString(unsigned indent, bool impl, const std::string &suffix) const;

    std::string getSignature() const;

    std::string getTemplateSignature() const;

    void clear();

    bool isVirtual() const;

    bool isConstructor() const;

    unsigned numArgs() const;

    Argument getArg(unsigned i) const;

    bool operator<(const Method &rhs) const;

  private:

    std::vector<Argument>  args;

    Type         type;
    bool         isvirtual;
    bool         isconst;
    bool         isconstructor;

    std::string  throw_decl;
    std::string  name;
    std::string  overloadname;
    std::string  classname;
};


/** An c++ class for mockpp idl files
  */
class MockppIdlClass
{
 public:

  /** Constructs a parser.
    */
    MockppIdlClass();

    void setName(const std::string &in_name);

    std::string getName() const;

    void addMethod(const Method &method);

    void clear();

    unsigned numMethods() const;

    Method getMethod(unsigned i) const;

    unsigned numCtors() const;

    Method getCtor(unsigned i) const;

    std::vector<unsigned> getArgsCount() const;

    void resolveOverloaded();

   std::string getSource() const;

   void setSource(const std::string &name);

  private:

    std::string          name;
    std::vector<Method>  methods;
    std::vector<Method>  ctors;
    std::vector<unsigned> argsCnt;
    std::string           sourcefile;
};


#endif // MOCKPP_IDL_CLASS_H
