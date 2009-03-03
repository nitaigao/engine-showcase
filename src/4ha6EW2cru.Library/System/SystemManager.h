#ifndef __SYSTEMMANAGER_H
#define __SYSTEMMANAGER_H

class SystemManager : public ISystemManager
{

public:

	virtual ~SystemManager( )
	{
		for( SystemList::reverse_iterator i = _systems.rbegin( ); i != _systems.rend( ); ++i )
		{
			delete ( *i );
		}
	}

	void AddSystem( ISystem* system )
	{
		_systems.push_back( system );
	}

	ISystem* GetSystem( SystemType systemType )
	{
		for( SystemList::iterator i = _systems.begin( ); i != _systems.end( ); ++i )
		{
			if ( ( *i )->GetSystemType( ) == systemType )
			{
				return ( *i );
			}
		}

		return 0;
	}

	void InitializeAllSystems( )
	{
		for( SystemList::iterator i = _systems.begin( ); i != _systems.end( ); ++i )
		{
			( *i )->Initialize( );
		}
	}

	void Update( float deltaMilliseconds )
	{
		for( SystemList::iterator i = _systems.begin( ); i != _systems.end( ); ++i )
		{
			 ( *i )->Update( deltaMilliseconds );
		}
	}

	const SystemList& GetAllSystems( )
	{
		return _systems;
	}

private:

	SystemList _systems;

};

#endif