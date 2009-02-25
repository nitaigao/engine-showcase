#include "CppSpec.h"

class LevelCoordinator
{

public:

	void LoadLevel( const std::string levelName )
	{
		//throw std::exception( );
	}

};

class LevelCoordinator_Specs : public CppSpec::Specification< LevelCoordinator, LevelCoordinator_Specs >
{

public:

	LevelCoordinator_Specs( )
	{
		REGISTER_BEHAVIOUR( LevelCoordinator_Specs, should_call_FileManager_on_LoadLevel );
	};

	LevelCoordinator* createContext( )
	{
		return new LevelCoordinator( );
	};

	void should_call_FileManager_on_LoadLevel( )
	{
		specify( invoking< void, const std::string >( &LevelCoordinator::LoadLevel, "TestLevelName").should.raise.exception< std::exception >( ) );
	};

};

REGISTER_SPECIFICATION( LevelCoordinator_Specs );