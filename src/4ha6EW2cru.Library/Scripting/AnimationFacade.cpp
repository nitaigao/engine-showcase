#include "AnimationFacade.h"

#include <luabind/luabind.hpp>
using namespace luabind;

namespace Script
{
	void AnimationFacade::RegisterFunctions( lua_State* state )
	{
		module( state )
		[
			class_< AnimationFacade >( "AnimationFacade" )
				.def( "startAnimation", &AnimationFacade::StartAnimation )
				.def( "stopAnimation", &AnimationFacade::StopAnimation )
		];
	}

	void AnimationFacade::StartAnimation( const std::string& animationName, const bool& loop )
	{
		AnyType::AnyTypeMap parameters;
		parameters[ System::Parameters::AnimationName ] = animationName;
		parameters[ System::Parameters::LoopAnimation ] = loop;

		m_component->PushMessage( System::Messages::StartAnimation, parameters );
	}

	void AnimationFacade::StopAnimation( const std::string& animationName )
	{
		AnyType::AnyTypeMap parameters;
		parameters[ System::Parameters::AnimationName ] = animationName;

		m_component->PushMessage( System::Messages::StopAnimation, parameters );
	}
}