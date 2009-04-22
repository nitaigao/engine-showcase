#include "ServiceManager.h"

IService* ServiceManager::FindService( SystemType systemType ) const
{
	for( IService::ServiceList::const_iterator i = _services.begin( ); i != _services.end( ); ++i )
	{
		if ( ( *i )->GetType( ) == systemType )
		{
			return ( *i );
		}
	}
}