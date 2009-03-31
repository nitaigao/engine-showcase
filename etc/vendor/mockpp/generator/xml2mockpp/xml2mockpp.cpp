/***************************************************************************
             xml2mockpp.cpp -- generate mock objects from idl files
                             -------------------
    begin                : Sun May 20 2007
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp@ewald-arnold.de

    $Id: xml2mockpp.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

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


#include "MockppIdlParser.h"
#include "GenChainableMethod.h"
#include "GenVisitableMethod.h"
#include "xmlexcept.h"

#include <iostream>


bool user_ctor = false;
bool gen_chain = false;
bool gen_visit = false;
std::string sourcename;
std::string destdir;


void gen_file(const MockppIdlClass &theClass)
{
    if (gen_chain)
    {
      GenerateByChainableMethods converter(theClass, user_ctor);
      converter.generate(destdir, theClass.getName());
    }

    else if (gen_visit)
    {
      GenerateByVisitableMethods converter(theClass, user_ctor);
      converter.generate(destdir, theClass.getName());
    }

    else
    {
      std::cerr << "No conversion method given\n";
      exit(1);
    }
}


int main(int argc, char**argv)
{
  int ret = 1;


  if (argc < 2)
  {
    std::cerr
      << "No parameters given\n"
      << "Usage:\n"
      << "xml2mockpp [--gen-visitable-method]\n"
      << "           [--gen-chainable-method]\n"
      << "           [--gen-user-ctor]\n"
      << "           [--gen-class=class-name]\n"
      << "           [--file-name=filename.ext]\n"
      << "           [--dest-dir=dest-path]\n"
      << "\n";
    return 1;
  }

  std::vector<std::string> classes;

  for (int i = 1; i < argc; ++i)
  {
    std::string arg = argv[i];
    if (arg == "--gen-visitable-method")
      gen_visit = true;

    else if (arg == "--gen-chainable-method")
      gen_chain = true;

    else if (arg == "--gen-user-ctor")
      user_ctor = true;

    else if (arg.substr(0, 12) == "--gen-class=")
      classes.push_back(arg.substr(12));

    else if (arg.substr(0, 12) == "--file-name=")
      sourcename = arg.substr(12);

    else if (arg.substr(0, 11) == "--dest-dir=")
      destdir = arg.substr(11);

    else
    {
      std::cerr << "Unknown option: " << arg << std::endl;
      return 1;
    }
  }

  if (sourcename.length() == 0)
  {
    std::cerr << "No source file given\n";
    return 1;
  }

  std::FILE *fs;
  fs = std::fopen (sourcename.c_str(), "r");

  if (fs == 0)
  {
    std::cerr << "File not readable: " << sourcename << std::endl;
    return 2;
  }

  char buffer[1000];

  MockppIdlParser parser;
  try
  {
    while (!feof(fs))
    {
      unsigned readed = std::fread(buffer, 1, sizeof(buffer), fs);
      if (!parser.parse(buffer, readed, false))
      {
        throw XmlException(parser.mapToFaultCode(parser.getErrorCode()),
                          "Problem while parsing idl file",
                          parser.getCurrentLineNumber(),
                          parser.getErrorString(parser.getErrorCode()));
      }
    }
    ret = 0;
  }

  catch(std::exception &ex)
  {
    std::cerr << ex.what() << std::endl;
    ret = 1;
  }

  catch(...)
  {
    std::cerr << "Unexpected problem while parsing idl file\n";
    ret = 1;
  }

  fclose(fs);

  if (ret != 0)
    return ret;

  try
  {
    for (unsigned ii = 0; ii < parser.numClasses(); ++ii)
    {
      if (classes.size() == 0)
        gen_file(parser.getClass(ii));

      else
      {
        for (unsigned ic = 0; ic < classes.size(); ++ic)
        {
          if (classes[ic] == parser.getClass(ii).getName())
            gen_file(parser.getClass(ii));
        }
      }
    }
    ret = 0;
  }

  catch(std::exception &ex)
  {
    std::cerr << ex.what() << std::endl;
    ret = 2;
  }

  catch(...)
  {
    std::cerr << "Unexpected problem while generating mock files\n";
    ret = 2;
  }



  return ret;
}

