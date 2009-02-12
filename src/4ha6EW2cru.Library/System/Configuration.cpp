#include "Configuration.h"

#include "../Events/EventManager.h"

Configuration::~Configuration( )
{
	EventManager::GetInstance( )->RemoveEventListener( GRAPHICS_SETTINGS_CHANGED, this, &Configuration::OnConfigurationUpdated );
	delete _configFile;
}

Configuration* Configuration::Load( const std::string& filePath )
{
	ConfigurationFile* configFile = ConfigurationFile::Load( filePath );
	Configuration* configuration = new Configuration( configFile );
	configuration->Initialize( );
	return configuration;
};

void Configuration::Initialize( )
{
	EventManager::GetInstance( )->AddEventListener( GRAPHICS_SETTINGS_CHANGED, this, &Configuration::OnConfigurationUpdated );
}

void Configuration::OnConfigurationUpdated( const IEvent* event )
{
	_configFile->Save( "config/game.cfg" );
}