#include "ServiceManager.h"

namespace Services
{
	IService* ServiceManager::FindService( System::Types::Type systemType ) const
	{
		for( IService::ServiceList::const_iterator i = m_services.begin( ); i != m_services.end( ); ++i )
		{
			if ( ( *i )->GetType( ) == systemType )
			{
				return ( *i );
			}
		}

		return 0;
	}

	AnyType::AnyTypeMap ServiceManager::MessageAll( const std::string& message, AnyType::AnyTypeMap parameters )
	{
		AnyType::AnyTypeMap results;

		for( IService::ServiceList::const_iterator i = m_services.begin( ); i != m_services.end( ); ++i )
		{
			AnyType::AnyTypeMap serviceResults = ( *i )->Execute( message, parameters );

			for ( AnyType::AnyTypeMap::iterator i = serviceResults.begin( ); i != serviceResults.end( ); ++i )
			{
				results.insert( *i );
			}
		}

		return results;
	}
}