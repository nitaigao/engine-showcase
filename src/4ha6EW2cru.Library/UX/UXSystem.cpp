#include "UXSystem.h"

#include "UXSystemScene.h"

namespace UX
{
	ISystemScene* UXSystem::CreateScene()
	{
		m_scene = new UXSystemScene( );
		m_scene->Initialize( ); 
		return m_scene;
	}
};