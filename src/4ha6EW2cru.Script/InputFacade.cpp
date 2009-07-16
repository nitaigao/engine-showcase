#include "InputFacade.h"

#include "System/SystemType.hpp"

using namespace luabind;

#include "System/AnyType.hpp"
#include "Service/IService.hpp"
using namespace Services;

#include "Management/Management.h"

namespace Script
{
	luabind::scope InputFacade::RegisterFunctions()
	{
		return (
			class_< InputFacade >( "InputFacade" )
				.def( constructor< >( ) )
				.def( "getTextForMessage", &InputFacade::GetTextForMessage )
				.def( "setMessageBinding", &InputFacade::SetMessageBinding )
				.def( "setDefaultMessageBinding", &InputFacade::SetDefaultMessageBinding )
			);
	}

	std::string InputFacade::GetTextForMessage( const std::string& message )
	{
		IService* inputService = Management::Get( )->GetServiceManager( )->FindService( System::Types::INPUT );

		AnyType::AnyTypeMap parameters;
		parameters[ System::Attributes::Message ] = message;

		return inputService->Execute( System::Messages::Input::GetTextForMessage, parameters )[ "result" ].As< std::string >( );
	}


	void InputFacade::SetMessageBinding( const std::string& message, const std::string& binding )
	{
		IService* inputService = Management::Get( )->GetServiceManager( )->FindService( System::Types::INPUT );

		AnyType::AnyTypeMap parameters;
		parameters[ System::Attributes::Message ] = message;
		parameters[ System::Parameters::Binding ] = binding;

		inputService->Execute( System::Messages::SetBindingForMessage, parameters );
	}

	void InputFacade::SetDefaultMessageBinding( const std::string& message, const std::string& binding )
	{
		IService* inputService = Management::Get( )->GetServiceManager( )->FindService( System::Types::INPUT );

		AnyType::AnyTypeMap parameters;
		parameters[ System::Attributes::Message ] = message;
		parameters[ System::Parameters::Binding ] = binding;

		inputService->Execute( System::Messages::Input::SetDefaultBindingForMessage, parameters );
	}
}