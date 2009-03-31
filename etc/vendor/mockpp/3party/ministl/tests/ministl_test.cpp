/***************************************************************************
                     ministl_test.cpp
                                 -
                             -------------------
    begin                : Tue Nov 28 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: ministl_test.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#include <ministl/vector>
#include <ministl/vector.h>
#include <ministl/map>
#include <ministl/map.h>
#include <ministl/multimap>
#include <ministl/pair>
#include <ministl/pair.h>
#include <ministl/set>
#include <ministl/set.h>
#include <ministl/string>
#include <ministl/basic_string.h>
#include <ministl/algorithm>
#include <ministl/algo.h>
#include <ministl/functional>
#include <ministl/function.h>
#include <ministl/defalloc.h>
//#include <ministl/bool.h>
#include <ministl/ministl.h>
#include <ministl/simplevec.h>
#include <ministl/raw_iterator.h>
#include <ministl/reverse_iterator.h>

#include <iostream>

bool map_test()
{
  MINISTL_STD_NS::cout << "Start map iter-test: \n";

  MINISTL_NS::map<int, unsigned> mm;
  MINISTL_NS::pair<int, unsigned> p(11, 22u);
  mm.insert(MINISTL_NS::make_pair(11, 22u));
  mm.insert(p);
  mm.insert(p);

  if (mm.size() != 1)
    return 1;

  mm.insert(MINISTL_NS::make_pair(111, 222u));
  mm.insert(MINISTL_NS::make_pair(1111, 2222u));

  MINISTL_NS::map<int, unsigned>::iterator fit = mm.begin();
  while (fit != mm.end())
  {
    MINISTL_STD_NS::cout << "fit++ " << (*fit).first
              << " " << (*fit).second << MINISTL_STD_NS::endl;
    ++fit;
  }

  MINISTL_STD_NS::cout << "End map iter-test: \n";

  return 0;
}


bool multimap_test()
{
  MINISTL_STD_NS::cout << "Start multimap iter-test: \n";

  MINISTL_NS::multimap<unsigned, signed> mm;
  mm.insert(MINISTL_NS::make_pair(3u, 2));
  mm.insert(MINISTL_NS::make_pair(10u, 2));
  mm.insert(MINISTL_NS::make_pair(1u, 22));
  mm.insert(MINISTL_NS::make_pair(10u, 22));
  mm.insert(MINISTL_NS::make_pair<unsigned, signed>(1u, 222));
  mm.insert(MINISTL_NS::make_pair(2u, 22));

  // lower_bound
  // upper_bound

  if (mm.size() != 6)
    return 1;

  MINISTL_NS::multimap<unsigned, signed>::iterator fit = mm.begin();
  if ((*fit++).first != 1) return 1;
  if ((*fit++).first != 1) return 1;
  if ((*fit++).first != 2) return 1;
  if ((*fit++).first != 3) return 1;
  if ((*fit++).first != 10) return 1;
  if ((*fit++).first != 10) return 1;

  fit = mm.begin();
  while (fit != mm.end())
  {
    MINISTL_STD_NS::cout << "fit++ " << (*fit).first
              << " " << (*fit).second << MINISTL_STD_NS::endl;
    ++fit;
  }

  MINISTL_STD_NS::cout << "End multimap iter-test: \n";

  return 0;
}


bool vector_test()
{
  MINISTL_NS::vector<unsigned> vv;
  vv.push_back(1u);
  vv.push_back(2u);
  vv.push_back(3u);

  MINISTL_STD_NS::cout << "Start vector iter-test:\n";

  MINISTL_NS::vector<unsigned>::iterator fit = vv.begin();
  if (*fit != 1)
    return 1;

  while (fit != vv.end())
    MINISTL_STD_NS::cout << "fit++ " << *fit++ << MINISTL_STD_NS::endl;

  fit = vv.end();
  if (*(fit-1) != 3)
    return 1;

  while (fit != vv.begin())
    MINISTL_STD_NS::cout << "fit-- " << *--fit << MINISTL_STD_NS::endl;

  MINISTL_STD_NS::cout << "reverse iter-test:\n";

  MINISTL_NS::vector<unsigned>::reverse_iterator rit = vv.rbegin();
  if (*rit != 3)
    return 1;

  while (rit != vv.rend())
    MINISTL_STD_NS::cout << "rit++ " << *rit++ << MINISTL_STD_NS::endl;

  rit = vv.rend();
  if (*(rit-1) != 1)
    return 1;

  while (rit != vv.rbegin())
    MINISTL_STD_NS::cout << "rit-- " << *--rit << MINISTL_STD_NS::endl;

  MINISTL_STD_NS::cout << "End vector iter-test: \n";

  return 0;
}


bool set_test()
{
  MINISTL_NS::set<int> ss;
  ss.insert(2);
  if (ss.size() != 1)
    return 1;

  return 0;
}


int main(int /*argc*/, char** /*argv*/)
{
  unsigned ret = 0;

  ret |= vector_test();
  ret |= multimap_test();
  ret |= map_test();
  ret |= set_test();

  return ret;
}

