#include "AnimationSystem.h"

#include "AnimationSystemScene.h"

#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Memory/hkThreadMemory.h>
#include <Common/Base/Memory/Memory/Pool/hkPoolMemory.h>	

#define INCLUDE_HAVOK_ANIMATION_CLASSES
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkClasses.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.cxx>
#define HK_COMPAT_FILE <Common/Compat/hkCompatVersions.h>
#include <Common/Compat/hkCompat_All.cxx> 

#include "Logging/Logger.h"
using namespace Logging;

namespace Animation
{
	void AnimationSystem::Initialize( Configuration::IConfiguration* configuration )
	{
		hkPoolMemory* memoryManager = new hkPoolMemory( );
		m_threadMemory = new hkThreadMemory( memoryManager );
		hkBaseSystem::init( memoryManager, m_threadMemory, errorReportFunction );
		memoryManager->removeReference( );

		int stackSize = 0x100000;
		m_stackBuffer = hkAllocate<char>( stackSize, HK_MEMORY_CLASS_BASE );
		hkThreadMemory::getInstance( ).setStackArea( m_stackBuffer, stackSize );
	}

	void AnimationSystem::errorReportFunction( const char* errorMessage, void* errorOutputObject )
	{
		Logger::Get( )->Warn( errorMessage );
	}

	void AnimationSystem::Update( const float& deltaMilliseconds )
	{
		m_scene->Update( deltaMilliseconds );
	}

	void AnimationSystem::Release( )
	{
		m_threadMemory->setStackArea(0, 0);
		hkDeallocate( m_stackBuffer );
		m_threadMemory->removeReference();

		hkBaseSystem::quit( );
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