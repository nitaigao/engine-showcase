/***************************************************************************
      MyBuilderNamespace.h  -  helper class for common use in tests
                             -------------------
    begin                : Wed Sep 4 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

   $Id: MyBuilderNamespace.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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


#include <mockpp/matcher/InvokedRecorder.h>

#include MOCKPP_ALGORITHM_H


class MyBuilderNamespace : public MOCKPP_NS::BuilderNamespace
{
  public:

    typedef MOCKPP_STL::pair<MOCKPP_NS::String, MOCKPP_NS::MatchBuilderAdapterBase*> PairType;
    typedef MOCKPP_STL::vector<PairType> TableType;

    TableType                                      idTable;
    MOCKPP_STL::vector<MOCKPP_NS::MatchBuilderAdapterBase*>  allBuilders;

    struct MyFinder
    {
      MOCKPP_NS::String ref;

      MyFinder ( const MOCKPP_NS::String &s )
          : ref ( s )
      {}

      bool operator() ( const PairType &p )
      {
        return p.first == ref;
      }
    };

    MyBuilderNamespace()
      : MOCKPP_NS::BuilderNamespace(MOCKPP_PCHAR("ns"))
    {}

    virtual ~MyBuilderNamespace()
    {
      reset();
    }

    virtual void reset()
    {
      while(idTable.size() != 0)
      {
        delete (*idTable.begin()).second;
        idTable.erase(idTable.begin());
      }

      for (unsigned i = 0; i < allBuilders.size(); ++i)
        delete allBuilders[i];
      allBuilders.clear();
    }

    virtual void addPendingRecorder(const MOCKPP_NS::String & /*id*/, MOCKPP_NS::InvokedRecorder::AP)
    {
    }

    virtual void storeBuilder(MOCKPP_NS::AutoPointer<MOCKPP_NS::MatchBuilderAdapterBase> builder )
    {
      allBuilders.push_back(builder.release());
    }

    virtual MOCKPP_NS::MatchBuilderAdapterBase *lookupID( const MOCKPP_NS::String &id ) const
    {
      TableType::const_iterator it = MOCKPP_STL::find_if( idTable.begin(), idTable.end(), MyFinder( id ) );
      if (it != idTable.end())
        return ( ( *it ).second );
      else
        return 0;
    }

    virtual void registerUniqueID( const MOCKPP_NS::String &id, MOCKPP_NS::MatchBuilderAdapterBase *invocation )
    {
      TableType::const_iterator it = MOCKPP_STL::find_if( idTable.begin(), idTable.end(), MyFinder( id ) );
      MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("duplicate invocation named"), ( it == idTable.end() ) );
      idTable.push_back( MOCKPP_STL::make_pair( id, invocation ) );
    }
};
