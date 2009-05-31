#include "AINavigationMeshComponent.h"

using namespace Maths;

#include "NavigationMesh.h"

#include "../Service/IService.hpp"
#include "../Management/Management.h"

namespace AI
{
	void AINavigationMeshComponent::Destroy( )
	{
		delete m_attributes[ System::Attributes::NavigationMesh ].As< INavigationMesh* >( );
	}

	void AINavigationMeshComponent::Initialize( )
	{
		// -- This will be substituted by automatic generation -- //

		IService* renderService = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::RENDER );

		AnyType::AnyTypeMap results = renderService->Execute( System::Messages::LoadMesh, m_attributes );

		MathVector3::MathVector3List vertices = results[ "vertices" ].As< MathVector3::MathVector3List >( );

		INavigationMesh* navMesh = new NavigationMesh( );
		NavigationPolygon* poly = new NavigationPolygon( );

		for( MathVector3::MathVector3List::iterator i = vertices.begin( ); i != vertices.end( ); ++i )
		{
			( *i ).X = ( *i ).X + 21.434f;
			( *i ).Z = ( *i ).Z + 28.174f;

			poly->AddVertex( *i );

			if ( poly->CountVertices( ) == 3 )
			{
				navMesh->AddPolygon( poly );
				poly = new NavigationPolygon( );
			}
		}

		delete poly;

		m_attributes[ System::Attributes::NavigationMesh ] = navMesh;

		navMesh->Render( );
	}

	AnyType AINavigationMeshComponent::Message( const System::Message& message, AnyType::AnyTypeMap parameters )
	{
		return AISystemComponent::Message( message, parameters );

		
	}
}