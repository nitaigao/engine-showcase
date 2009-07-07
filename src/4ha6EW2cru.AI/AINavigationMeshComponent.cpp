#include "AINavigationMeshComponent.h"

using namespace Maths;

#include "Service/IService.hpp"
using namespace Services;

#include "Management/Management.h"

namespace AI
{
	AINavigationMeshComponent::~AINavigationMeshComponent( )
	{
		for( NavigationPolygon::NavPolyList::iterator i = m_polygons.begin( ); i != m_polygons.end( ); ++i )
		{
			delete ( *i );
		}
	}

	void AINavigationMeshComponent::Destroy( )
	{
		/*AnyType::AnyTypeMap parameters;
		parameters[ System::Attributes::Name ] = m_attributes[ System::Attributes::Name ].As< std::string >( );

		IService* renderService = Management::Get( )->GetServiceManager( )->FindService( System::Types::RENDER );
		renderService->Execute( System::Messages::DestroyMesh, parameters );*/
	}

	void AINavigationMeshComponent::Initialize( )
	{
		IService* renderService = Management::Get( )->GetServiceManager( )->FindService( System::Types::RENDER );
		AnyType::AnyTypeMap results = renderService->Execute( System::Messages::LoadMesh, m_attributes );
		MathVector3::MathVector3List vertices = results[ "vertices" ].As< MathVector3::MathVector3List >( );

		NavigationPolygon* polygon = new NavigationPolygon( );

		for( MathVector3::MathVector3List::iterator i = vertices.begin( ); i != vertices.end( ); ++i )
		{	
			polygon->AddVertex( *i );

			if ( polygon->CountVertices( ) == 3 )
			{
				m_polygons.push_back( polygon );
				polygon = new NavigationPolygon( );
			}
		}

		delete polygon;
	}

	MathVector3::MathVector3List AINavigationMeshComponent::FindPath( const Maths::MathVector3& start, const Maths::MathVector3& finish )
	{
		NavigationPolygon* startPolygon = 0;
		NavigationPolygon* endPolygon = 0;

		for ( NavigationPolygon::NavPolyList::iterator i = m_polygons.begin( ); i != m_polygons.end( ); ++i )
		{
			if ( ( *i )->IsPointInside( start ) )
			{
				startPolygon = ( *i );
			}

			if ( ( *i )->IsPointInside( finish ) )
			{
				endPolygon = ( *i );
			}
		}

		assert( startPolygon != 0 && "Start Polygon not found"  );
		assert( endPolygon  != 0 && "End Polygon not found"  );

		return this->FindPath( startPolygon, endPolygon );
	}

	MathVector3::MathVector3List AINavigationMeshComponent::FindPath( NavigationPolygon* start, NavigationPolygon* finish )
	{
		for( NavigationPolygon::NavPolyList::iterator i = m_polygons.begin( ); i != m_polygons.end( ); ++i )
		{
			( *i )->Reset( );
		}

		NavigationPolygon::NavPolyList openList;
		openList.push_back( start );

		NavigationPolygon* currentNode = 0;

		while( !openList.empty( ) )
		{
			currentNode = openList.front( );
			openList.pop_front( );
			currentNode->Mark( );

			if ( currentNode == finish ) { break; }

			for( NavigationPolygon::NavPolyList::const_iterator neighbour = currentNode->GetNeighbours( ).begin( ); neighbour != currentNode->GetNeighbours( ).end( ); ++neighbour )
			{
				if ( ( *neighbour )->IsMarked( ) ) { continue; }

				bool onOpenList = false;

				for( NavigationPolygon::NavPolyList::iterator open = openList.begin( ); open != openList.end( ); ++open )
				{
					if ( ( *open ) == ( *neighbour ) )
					{
						onOpenList = true;

						if( currentNode->GetCost( ) < ( *open )->GetCost( ) )
						{
							( *open )->SetParent( currentNode, finish );

							// recalculate queue here
						}

						break;
					}
				}

				if ( !onOpenList )
				{
					( *neighbour )->SetParent( currentNode, finish );
					openList.push_back( *neighbour );
				}
			}
		}

		MathVector3::MathVector3List waypoints;

		while( currentNode )
		{
			if ( currentNode->GetParent( ) != 0 )
			{
				NavigationPolygon::PolyEdge edge = currentNode->GetEdgeLinks( )[ currentNode->GetEntryEdge( ) ];
				MathVector3 halfDifference = ( edge.second - edge.first ) * 0.5f;
				MathVector3 midPoint = edge.first + halfDifference;

				waypoints.push_front( midPoint ); 
			}

			currentNode = currentNode->GetParent( );
		}

		return waypoints;
	}

	AnyType AINavigationMeshComponent::Message( const System::Message& message, AnyType::AnyTypeMap parameters )
	{
		AISystemComponent::Message( message, parameters );

		AnyType result;

		if ( message == System::Messages::SetPosition )
		{
			IService* renderService = Management::Get( )->GetServiceManager( )->FindService( System::Types::RENDER );

			AnyType::AnyTypeMap renderParameters;
			MathVector3::MathVector3List vertices;
			
			for ( NavigationPolygon::NavPolyList::iterator i = m_polygons.begin( ); i != m_polygons.end( ); ++i )
			{
				( *i )->SetPosition( parameters[ System::Attributes::Position ].As< MathVector3 >( ) );

				MathVector3::MathVector3List polyVerts = ( *i )->GetVertices( );
				for( Maths::MathVector3::MathVector3List::const_iterator i = polyVerts.begin( ); i != polyVerts.end( ); ++i )
				{
					vertices.push_back( ( *i ) );
				}
			}

			this->RecalculateNeighbours( );

			renderParameters[ System::Parameters::Vertices ] = vertices;
			renderParameters[ System::Attributes::Name ] = m_attributes[ System::Attributes::Name ].As< std::string >( );
			renderService->Execute( System::Messages::RenderMesh, renderParameters );
		}

		if ( message == System::Messages::FindPath )
		{
			result = this->FindPath(  
				parameters[ System::Parameters::Origin ].As< MathVector3 >( ),
				parameters[ System::Parameters::Destination ].As< MathVector3 >( )
				);
		}

		return result;
	}

	void AINavigationMeshComponent::RecalculateNeighbours( )
	{
		for ( NavigationPolygon::NavPolyList::iterator polygona = m_polygons.begin( ); polygona != m_polygons.end( ); ++polygona )
		{
			( *polygona )->ResetNeighbours( );

			for ( NavigationPolygon::NavPolyList::iterator polygonb = m_polygons.begin( ); polygonb != m_polygons.end( ); ++polygonb )
			{
				if ( ( *polygonb ) != ( *polygona ) )
				{
					if ( ( *polygonb )->IsNeighbour( *polygona ) )
					{
						( *polygona )->AddNeighbour( *polygonb );
					}
				}
			}
		}
	}
}