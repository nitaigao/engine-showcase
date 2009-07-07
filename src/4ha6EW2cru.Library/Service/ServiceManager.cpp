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

	void ServiceManager::MessageAll( const std::string& message, AnyType::AnyTypeMap parameters )
	{
		for( IService::ServiceList::const_iterator i = m_services.begin( ); i != m_services.end( ); ++i )
		{
			( *i )->Execute( message, parameters );
		}
	}
}