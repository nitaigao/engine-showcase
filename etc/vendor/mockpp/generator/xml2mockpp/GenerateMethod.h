/***************************************************************************
            GenerateMethod.h  -  generate class with mock methods
                             -------------------
    begin                : Sun June 03 2007
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp@ewald-arnold.de

    $Id: GenerateMethod.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef GENERATEMETHOD_H
#define GENERATEMETHOD_H

#include <string>
#include <vector>
#include <iosfwd>

#include "MockppIdlClass.h"


class GenerateMethod
{
  public:

    virtual ~GenerateMethod();

    virtual void generate(const std::string &destdir, const std::string &name) = 0;

    void generateHeaderHead(std::ostream &h_file,
                            const std::string &name,
                            const std::string &methname,
                            const MockppIdlClass &theClass);

    void generateHeaderMocker(std::ostream & h_file,
                              const std::string &methname,
                              const MockppIdlClass &theClass);

    void generateHeaderMethods(std::ostream & h_file,
                               const MockppIdlClass &theClass);

    void generateHeaderReferenceVariables(std::ostream & h_file,
                                          const MockppIdlClass &theClass);

    void generateHeaderTail(std::ostream & h_file,
                            const std::string &name);

    void generateSourceHead(std::ostream & cpp_file,
                            const std::string &h_name,
                            const MockppIdlClass &theClass);

    void generateUserSourceHead(std::ostream & cpp_file,
                                const std::string &h_name);

    void generateSourceCtors(std::ostream & cpp_file,
                             const std::string &name,
                             const std::string &methname,
                             const MockppIdlClass &theClass);

    void generateSourceMethods(std::ostream & cpp_file,
                               const MockppIdlClass &theClass);
};

#endif // GENERATEMETHOD_H
