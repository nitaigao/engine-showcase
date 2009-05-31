#pragma once
#ifndef AINAVIGATIONMESHCOMPONENT_H
#define AINAVIGATIONMESHCOMPONENT_H

#include "AISystemComponent.h"

namespace AI
{
	class AINavigationMeshComponent : public AISystemComponent
	{

	public:

		AINavigationMeshComponent( const std::string& name )
			: AISystemComponent( name )
		{

		}

		void Initialize( );

		void Destroy( );

		/*! Messages the Component to influence its internal state
		*
		*  @param[in] const std::string & message
		*  @return (AnyType)
		*/
		AnyType Message( const System::Message& message, AnyType::AnyTypeMap parameters );

	};
};

#endif