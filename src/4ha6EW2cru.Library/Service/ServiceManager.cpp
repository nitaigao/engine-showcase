#include "ServiceManager.h"

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