#include "AnimationSystem.h"

#include "AnimationSystemScene.h"

namespace Animation
{
	void AnimationSystem::Initialize( Configuration::IConfiguration* configuration )
	{

	}

	void AnimationSystem::Update( const float& deltaMilliseconds )
	{
		m_scene->Update( deltaMilliseconds );
	}

	void AnimationSystem::Release( )
	{

	}

	ISystemScene* AnimationSystem::CreateScene( )
	{
		m_scene = new AnimationSystemScene( );
		return m_scene;
	}

	AnyType::AnyTypeMap AnimationSystem::Execute( const std::string& actionName, AnyType::AnyTypeMap& parameters )
	{
		return AnyType::AnyTypeMap( );
	}
}