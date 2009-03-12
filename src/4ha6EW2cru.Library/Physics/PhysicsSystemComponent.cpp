#include "PhysicsSystemComponent.h"

#include "../io/FileBuffer.hpp"
#include "../io/FileManager.h"

#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Memory/hkThreadMemory.h>
#include <Common/Base/Memory/Memory/Pool/hkPoolMemory.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileReader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Serialize/Util/hkLoader.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>	
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>

void PhysicsSystemComponent::Initialize( SystemPropertyList properties )
{
	for ( SystemPropertyList::iterator i = properties.begin( ); i != properties.end( ); ++i )
	{
		if ( ( *i ).GetName( ) == "body" )
		{
			std::string bodyPath = ( *i ).GetValue< std::string >( );
			FileBuffer* bodyBuffer = FileManager::GetInstance( )->GetFile( bodyPath, true );

			//hkBinaryPackfileReader reader;
			//reader.loadEntireFileInplace( bodyBuffer->fileBytes, bodyBuffer->fileLength );
			//hkPackfileData* loadedData = reader.getPackfileData( );
			//loadedData->addReference();

			//hkRootLevelContainer* container = static_cast< hkRootLevelContainer* >( reader.getContents( "hkRootLevelContainer" ) );

			hkLoader* loader = new hkLoader( );
			hkRootLevelContainer* container = loader->load("C:\\box.hkx");

			hkpPhysicsData* physicsData = static_cast< hkpPhysicsData* >( container->findObjectByType("hkpPhysicsData") );

			hkpRigidBody* body = physicsData->findRigidBodyByName( "Box01" );

			//loadedData->removeReference( );

			_scene->GetWorld( )->addEntity( body );
			body->removeReference( );
		}
	}
}