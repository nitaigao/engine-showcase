#include "AnimationSystem.h"

#include "AnimationSystemScene.h"

#define INCLUDE_HAVOK_ANIMATION_CLASSES
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkClasses.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.cxx>
#define HK_COMPAT_FILE <Common/Compat/hkCompatVersions.h>
#include <Common/Compat/hkCompat_All.cxx> 

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