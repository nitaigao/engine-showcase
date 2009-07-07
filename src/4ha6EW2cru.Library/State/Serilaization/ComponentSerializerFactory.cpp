#include "ComponentSerializerFactory.h"

#include "../../System/SystemType.hpp"

#include "AIComponentSerializer.h"
#include "GraphicsComponentSerializer.h"
#include "GeometryComponentSerializer.h"
#include "PhysicsComponentSerializer.h"
#include "InputComponentSerializer.h"
#include "AIComponentSerializer.h"
#include "ScriptComponentSerializer.h"
#include "SoundComponentSerializer.h"
#include "AnimationComponentSerializer.h"
#include "NetworkComponentSerializer.h"

namespace Serialization
{

	IComponentSerializer* ComponentSerializerFactory::Create( const System::Types::Type& systemType )
	{
		IComponentSerializer* strategy = 0;

		if( systemType == System::Types::RENDER )
		{
			strategy = new GraphicsComponentSerializer( );
		} 
		else if ( systemType == System::Types::GEOMETRY )
		{
			strategy = new GeometryComponentSerializer( );
		}
		else if ( systemType == System::Types::PHYSICS )
		{
			strategy = new PhysicsComponentSerializer( );
		}
		else if ( systemType == System::Types::INPUT )
		{
			strategy = new InputComponentSerializer( );
		}
		else if ( systemType == System::Types::AI )
		{
			strategy = new AIComponentSerializer( );
		}
		else if ( systemType == System::Types::SCRIPT )
		{
			strategy = new ScriptComponentSerializer( );
		}
		else if ( systemType == System::Types::SOUND )
		{
			strategy = new SoundComponentSerializer( );
		}
		else if ( systemType == System::Types::ANIMATION )
		{
			strategy = new AnimationComponentSerializer( );
		}
		else if ( systemType == System::Types::NETWORK )
		{
			strategy = new NetworkComponentSerializer( );
		}

		return strategy;
	}
}