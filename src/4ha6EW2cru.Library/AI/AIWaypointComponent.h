#pragma once
#ifndef AIWAYPOINTCOMPONENT_H
#define AIWAYPOINTCOMPONENT_H

#include "AISystemComponent.h"

namespace AI
{
	class AIWaypointComponent : public AISystemComponent
	{

	public:

		~AIWaypointComponent( ) { };

		AIWaypointComponent( const std::string& name )
			: AISystemComponent( name )
		{

		}

	};
};

#endif