/***************************************************************************
        GenChainableMethod.h  -  generate class with chainable methods
                             -------------------
    begin                : Sun May 28 2007
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp@ewald-arnold.de

    $Id: GenChainableMethod.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef GENCHAINABLEMETHOD_H
#define GENCHAINABLEMETHOD_H

#include <string>

#include "GenerateMethod.h"


class GenerateByChainableMethods : public GenerateMethod
{
  public:

    GenerateByChainableMethods(const MockppIdlClass &theClass, bool user_ctor);

    void generate(const std::string &destdir, const std::string &name);

  private:

    void generate_H(const std::string &destdir, const std::string &name);

    void generate_CPP(const std::string &destdir, const std::string &name);

    void generate_CPP_USER(const std::string &destdir, const std::string &name);

  private:

    MockppIdlClass theClass;
    bool           user_ctor;
};


#endif // GENCHAINABLEMETHOD_H

