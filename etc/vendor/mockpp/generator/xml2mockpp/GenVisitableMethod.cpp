/***************************************************************************
        GenVisitableMethod.cpp  -  generate class with visitable methods
                             -------------------
    begin                : Sun May 28 2007
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp@ewald-arnold.de

    $Id: GenVisitableMethod.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#include "GenVisitableMethod.h"

#include <fstream>
#include <iostream>
#include <iostream>

#include <sys/stat.h>


GenerateByVisitableMethods::GenerateByVisitableMethods(const MockppIdlClass &in_class,
                                                       bool in_ctor)
  : theClass(in_class)
  , user_ctor(in_ctor)
{
  theClass.resolveOverloaded();
}


void GenerateByVisitableMethods::generate_H(const std::string &destdir, const std::string &name)
{
  const std::string h_name = destdir + name + "_mock.h";
  std::ofstream h_file(h_name.c_str());
  std::cout << "Header file will be created: " << h_name << std::endl;

  generateHeaderHead(h_file, name, "Visit", theClass);

  for (unsigned i = 0; i < theClass.numMethods(); ++i)
  {
    const Method method = theClass.getMethod(i);

    if (method.numArgs() != 0)
    {
      if (method.getName() != method.getOverloadName())
        h_file << "    // mapped to " << method.getOverloadName() << "_mocker (there are overloaded methods)\n";
      else
        h_file << "    // mapped to " << method.getName() << "_mocker\n";

      h_file << "    void "
             << method.getName()
             << "(\n";
      for (unsigned iarg = 0; iarg < method.numArgs(); ++iarg)
      {
        if (iarg != 0)
          h_file << ",\n";

        Type type = method.getArg(iarg).getType();
        h_file << "      const MOCKPP_NS::ConstraintHolder<"
               << type.getContraintType()
               << "> &p" << iarg;
      }
      h_file << ");\n\n";
    }
  }

  generateHeaderMethods(h_file, theClass);
  generateHeaderMocker(h_file, "Visit", theClass);
  generateHeaderReferenceVariables(h_file, theClass);
  generateHeaderTail(h_file, name);
}


void GenerateByVisitableMethods::generate_CPP(const std::string &destdir, const std::string &name)
{
  const std::string h_name = name + "_mock.h";
  const std::string cpp_name = destdir + name + "_mock.cpp";
  std::ofstream cpp_file(cpp_name.c_str());
  std::cout << "Source file will be created: " << cpp_name << std::endl;

  generateSourceHead(cpp_file, h_name, theClass);

  if (!user_ctor)
    generateSourceCtors(cpp_file, name, "Visit", theClass);

  for (unsigned i = 0; i < theClass.numMethods(); ++i)
  {
    const Method method = theClass.getMethod(i);
    if (method.numArgs() != 0)
    {
      cpp_file << "void "
               << theClass.getName() << "Mock" << "::" << method.getName()
               << "(\n";
      for (unsigned iarg = 0; iarg < method.numArgs(); ++iarg)
      {
        if (iarg != 0)
          cpp_file << ",\n";

        Type type = method.getArg(iarg).getType();
        cpp_file << "  const MOCKPP_NS::ConstraintHolder<"
                 << type.getContraintType()
                 << "> &p" << iarg;
      }
      cpp_file << ")\n"
                  "{\n";

      cpp_file << "  " << method.getOverloadName() << "_mocker.forward(";

      for (unsigned iarg = 0; iarg < method.numArgs(); ++iarg)
      {
        if (iarg != 0)
          cpp_file << ", ";

        char buffer[40];
        std::sprintf(buffer, "%d", iarg);
        std::string s = std::string("p") + buffer;
        cpp_file << Argument::getConstCastedName(s,
                                                 method.getArg(iarg).getType(),
                                                 "const MOCKPP_NS::ConstraintHolder< ",
                                                 ">& ");
      }

      cpp_file << ");\n"
               << "}\n\n\n";
    }
  }

  generateSourceMethods(cpp_file, theClass);
}


void GenerateByVisitableMethods::generate_CPP_USER(const std::string &destdir, const std::string &name)
{
  const std::string h_name = name + "_mock.h";
  std::string cpp_name = destdir + name + "_mock_user.cpp";

  struct stat statbuf;
  if (stat(cpp_name.c_str(), &statbuf) >= 0)
  {
    std::cout << "User file already exists: " << cpp_name << std::endl;
    cpp_name += ".new";
    std::cout << "New template will be created: " << cpp_name << std::endl;
  }

  std::ofstream cpp_file(cpp_name.c_str());
  std::cout << "User file will be created: " << cpp_name << std::endl;

  generateUserSourceHead(cpp_file, h_name);

  if (user_ctor)
    generateSourceCtors(cpp_file, name, "Visit", theClass);
}


void GenerateByVisitableMethods::generate(const std::string &destdir, const std::string &name)
{
  generate_H(destdir, name);
  generate_CPP(destdir, name);
  generate_CPP_USER(destdir, name);
}

/*

class MyVisitableMockObject_2 : public MOCKPP_NS::VisitableMockObject
{
  public:

    MyVisitableMockObject_2(const MOCKPP_NS::String &name)
      : MOCKPP_NS::VisitableMockObject(name, 0)
      , visitable_mocker(MOCKPP_PCHAR("visitablei2"), this)
      , visitablev_mocker(MOCKPP_PCHAR("visitablev2"), this)
    {}

    void visitable(const MOCKPP_NS::ConstraintHolder<unsigned> &p1,
                   const MOCKPP_NS::ConstraintHolder<unsigned> &p2)
    {
      visitable_mocker.forward(p1, p2);
    }

    int visitable(unsigned i, unsigned j)
    {
      return visitable_mocker.forward(i, j);
    }

    void visitablev(const MOCKPP_NS::ConstraintHolder<unsigned> &p1,
                    const MOCKPP_NS::ConstraintHolder<unsigned> &p2)
    {
      visitablev_mocker.forward(p1, p2);
    }

    void visitablev(unsigned i, unsigned j)
    {
      visitablev_mocker.forward(i, j);
    }

    MOCKPP_NS::VisitableMockMethod2<int, unsigned, unsigned>  visitable_mocker;
    MOCKPP_NS::VisitableMockMethod2<void, unsigned, unsigned> visitablev_mocker;
};


void VisitableMockMethod_2_test::test_parameter_2()
{
  MyVisitableMockObject_2 vmo(MOCKPP_PCHAR("Visitable-object"));
  MOCKPP_NS::VisitableMockMethod2<int, unsigned, unsigned> &vmb (vmo.visitable_mocker);

  vmb.setDefaultReturnValue(123);
  vmb.addReturnValue(321, 1);
  vmb.addThrowable(MOCKPP_NS::make_throwable(123u));
  vmb.addReturnValue(432, 1);
  vmb.addThrowable(MOCKPP_NS::make_throwable(321u));

  vmo.visitable(1, 2);
  vmo.visitable(2, 3);
  vmo.visitable(3, 4);
  vmo.visitable(4, 5);
  vmo.visitable(5, 6);
  vmo.visitable(2, 2);
  vmo.visitable(MOCKPP_NS::eq<unsigned>(2, 2),
                MOCKPP_NS::eq<unsigned>(20, 2));
}

*/


