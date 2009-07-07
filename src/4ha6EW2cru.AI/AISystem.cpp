#include "AISystem.h"

#include "AISystemScene.h"

namespace AI
{
	ISystemScene* AISystem::CreateScene( )
	{
		m_scene = new AISystemScene( );
		m_scene->Initialize( );
		return m_scene;
	}

	void AISystem::Update( const float& deltaMilliseconds )
	{
		m_scene->Update( deltaMilliseconds );
	}
}