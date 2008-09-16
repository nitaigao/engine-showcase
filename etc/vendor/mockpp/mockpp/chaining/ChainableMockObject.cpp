/** @file
    @brief    MockObjects with chained setup parameters


  $Id: ChainableMockObject.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Sep 28 2004
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

#define MOCKPP_NEED_EXPORTS
#include <mockpp/mockpp.h> // always first

#include MOCKPP_ALGORITHM_H

#include <mockpp/chaining/ChainableMockObject.h>


MOCKPP_NS_START


MOCKPP_API_IMPL0 ChainableMockObjectBase::ChainableMockObjectBase(const String &name)
 : BuilderNamespace(name)
{
}


MOCKPP_API_IMPL0 ChainableMockObjectBase::~ChainableMockObjectBase()
{
  chainableReset();
}


MOCKPP_API_IMPL(void) ChainableMockObjectBase::chainableVerify()
{
  checkPendingRecorders();
}


MOCKPP_API_IMPL(void) ChainableMockObjectBase::checkPendingRecorders() const
{
  String penders;
  while(pendingRecorders.size() != 0)
  {
    if (penders.length() != 0)
      penders += MOCKPP_PCHAR(", ");

    penders += (*pendingRecorders.begin()).first;
    delete (*pendingRecorders.begin()).second;
    pendingRecorders.erase(pendingRecorders.begin());
  }

  if (penders.length() != 0)
    assertionFailed( __LINE__, __FILE__,
                                MOCKPP_PCHAR("Chaining mock object \"") + getIdent()
                                + MOCKPP_PCHAR( "\": there are unassigned idenfiers:\n  " )
                                + penders);
}


MOCKPP_API_IMPL(MatchBuilderAdapterBase *) ChainableMockObjectBase::lookupID( const String &id ) const
{
  TableType::const_iterator it = idTable.find( id );
  if ( it == idTable.end() )
    return 0;

  return ( (*it).second );
}


MOCKPP_API_IMPL(void) ChainableMockObjectBase::registerUniqueID( const String &id, MatchBuilderAdapterBase *builder )
{
  MatchBuilderAdapterBase *tmb = lookupID(id);
  if ( tmb != 0 )
    assertionFailed( __LINE__, __FILE__,
                                MOCKPP_PCHAR( "duplicate invocation named \"" )
                                + id
                                + MOCKPP_PCHAR( "\"" ) );
  storeID(id, builder);
}


MOCKPP_API_IMPL(void) ChainableMockObjectBase::storeBuilder( AutoPointer<MatchBuilderAdapterBase> builder )
{
  allBuilders.push_back(builder.release());
}


MOCKPP_API_IMPL(void) ChainableMockObjectBase::addPendingRecorder(const String &id, InvokedRecorder::AP recorder)
{
  pendingRecorders.insert( PendingType::value_type(id, recorder.release()) );
}


MOCKPP_API_IMPL(void) ChainableMockObjectBase::storeID( const String &id, MatchBuilderAdapterBase *builder )
{
  idTable.insert( MOCKPP_STL::make_pair(id, builder) );

  PendingType::iterator it;
  while ( (it = pendingRecorders.find( id )) != pendingRecorders.end() )
  {
    builder->match( (*it).second );
    pendingRecorders.erase(it);
  }
}


MOCKPP_API_IMPL(void) ChainableMockObjectBase::chainableReset()
{
  while(idTable.size() != 0)
  {
    delete (*idTable.begin()).second;
    idTable.erase(idTable.begin());
  }

  for (unsigned i = 0; i < allBuilders.size(); ++i)
    delete allBuilders[i];
  allBuilders.clear();

  while(pendingRecorders.size() != 0)
  {
    delete (*pendingRecorders.begin()).second;
    pendingRecorders.erase(pendingRecorders.begin());
  }
}


/////////////////////////////////////////////////////////////////


MOCKPP_API_IMPL0 ChainableMockObject::ChainableMockObject(const String &name, VerifiableList *parent)
 : MockObject(name, parent)
 , ChainableMockObjectBase(name)
{
}


MOCKPP_API_IMPL0 ChainableMockObject::~ChainableMockObject()
{
  reset();
}


MOCKPP_API_IMPL(void) ChainableMockObject::verify()
{
  MockObject::verify();
  chainableVerify();
}


MOCKPP_API_IMPL(void) ChainableMockObject::reset()
{
  MockObject::reset();
  chainableReset();
}


MOCKPP_NS_END
