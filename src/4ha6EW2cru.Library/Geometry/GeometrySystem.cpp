#include "GeometrySystem.h"

#include "GeometrySystemScene.h"

#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
using namespace luabind;

#include "../Maths/MathQuaternion.hpp"
#include "../Maths/MathVector3.hpp"
using namespace Maths;

#include "../Management/Management.h"

namespace Geometry
{
	ISystemScene* GeometrySystem::CreateScene( )
	{
		return new GeometrySystemScene( this );
	}

	void GeometrySystem::Message( const std::string& message, AnyType::AnyTypeMap parameters )
	{
		if ( message == System::Messages::RegisterService )
		{
			Management::GetInstance( )->GetServiceManager( )->RegisterService( this );
		}
	}

	AnyType::AnyTypeMap GeometrySystem::Execute( const std::string& message, AnyType::AnyTypeMap& parameters )
	{
		if ( message == System::Messages::RegisterScriptFunctions )
		{
			module( parameters[ System::Attributes::ScriptState ].As< lua_State* >( ) )
			[
				class_< MathVector3 >( "Vector" )
					.def( constructor< const float&, const float&, const float& >( ) )
					.def( self + MathVector3( ) ),

				class_< MathQuaternion >( "Quaternion" )
					.def( constructor< const float&, const float&, const float&, const float& >( ) )
			];
		}

		return AnyType::AnyTypeMap( );
	}
};