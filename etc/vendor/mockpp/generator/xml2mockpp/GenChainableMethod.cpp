/***************************************************************************
        GenChainableMethod.cpp  -  generate class with chainable methods
                             -------------------
    begin                : Sun May 28 2007
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp@ewald-arnold.de

    $Id: GenChainableMethod.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#include "GenChainableMethod.h"

#include <fstream>
#include <iostream>

#include <sys/stat.h>


GenerateByChainableMethods::GenerateByChainableMethods(const MockppIdlClass &in_class,
                                                       bool in_ctor)
  : theClass(in_class)
  , user_ctor(in_ctor)
{
  theClass.resolveOverloaded();
}


void GenerateByChainableMethods::generate_H(const std::string &destdir, const std::string &name)
{
  const std::string h_name = destdir + name + "_mock.h";
  std::ofstream h_file(h_name.c_str());
  std::cout << "Header file will be created: " << h_name << std::endl;

  generateHeaderHead(h_file, name, "Chain", theClass);
  generateHeaderMethods(h_file, theClass);
  generateHeaderMocker(h_file, "Chain", theClass);
  generateHeaderReferenceVariables(h_file, theClass);
  generateHeaderTail(h_file, name);
}


void GenerateByChainableMethods::generate_CPP(const std::string &destdir, const std::string &name)
{
  const std::string h_name = name + "_mock.h";
  const std::string cpp_name = destdir + name + "_mock.cpp";
  std::ofstream cpp_file(cpp_name.c_str());
  std::cout << "Source file will be created: " << cpp_name << std::endl;

  generateSourceHead(cpp_file, h_name, theClass);

  if (!user_ctor)
    generateSourceCtors(cpp_file, name, "Chain", theClass);

  generateSourceMethods(cpp_file, theClass);
}


void GenerateByChainableMethods::generate_CPP_USER(const std::string &destdir, const std::string &name)
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
    generateSourceCtors(cpp_file, name, "Chain", theClass);
}


void GenerateByChainableMethods::generate(const std::string &destdir, const std::string &name)
{
  generate_H(destdir, name);
  generate_CPP(destdir, name);
  generate_CPP_USER(destdir, name);
}

/*

void ChainableMockMethod_2_test::test_parameter_2()
{
  MyChainableMockObject_2 mcm(MOCKPP_PCHAR("chainable-object"));
  MOCKPP_NS::ChainableMockMethod2<int, unsigned, long> &chain(mcm.chainable_mocker);

  chain.stubs(MOCKPP_NS::once())
       .with(MOCKPP_NS::eq<unsigned>(12u),
             MOCKPP_NS::eq<long>(23))
       .will(MOCKPP_NS::returnValue<int>(21));

  mcm.verify();
}


*/

