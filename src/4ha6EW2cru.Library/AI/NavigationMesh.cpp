#include "NavigationMesh.h"

using namespace Maths;

#include <Ogre.h>

#include "../Service/IService.hpp"
#include "../Management/Management.h"

namespace AI
{
	NavigationMesh::~NavigationMesh()
	{
		for( NavigationPolygon::NavPolyList::iterator i = m_polygons.begin( ); i != m_polygons.end( ); ++i )
		{
			delete ( *i );
		}
	}

	void NavigationMesh::AddPolygon( NavigationPolygon* polygon )
	{
		for ( NavigationPolygon::NavPolyList::iterator i = m_polygons.begin( ); i != m_polygons.end( ); ++i )
		{
			if ( ( *i ) != polygon )
			{
				if ( ( *i )->IsNeighbour( polygon ) )
				{
					( *i )->AddNeighbour( polygon );
					polygon->AddNeighbour( *i );
				}
			}
		}

		m_polygons.push_back( polygon );
	}

	void NavigationMesh::Render( )
	{
		Ogre::ManualObject* navMeshRenderable = Ogre::Root::getSingletonPtr( )->getSceneManager( "default" )->createManualObject( "navMesh" );

		navMeshRenderable->begin( "Yellow", Ogre::RenderOperation::OT_LINE_STRIP );

		for( NavigationPolygon::NavPolyList::iterator poly = m_polygons.begin( ); poly != m_polygons.end( ); ++poly )
		{
			for( MathVector3::MathVector3List::const_iterator vert = ( *poly )->GetVertices( ).begin( ); vert != ( *poly )->GetVertices( ).end( ); ++vert )
			{
				navMeshRenderable->position( ( *vert ).X, ( *vert ).Y + 0.1f, ( *vert ).Z );
			}
		}

		navMeshRenderable->end( );

		Ogre::SceneNode* sceneNode = Ogre::Root::getSingletonPtr( )->getSceneManager( "default" )->createSceneNode( "navMesh" );
		sceneNode->attachObject( navMeshRenderable );
		Ogre::Root::getSingletonPtr( )->getSceneManager( "default" )->getRootSceneNode( )->addChild( sceneNode );
	}

	Maths::MathVector3::MathVector3List NavigationMesh::FindPath( const Maths::MathVector3& start, const Maths::MathVector3& finish )
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

		Maths::MathVector3::MathVector3List results = this->FindPath( startPolygon, endPolygon );
		
		//results.push_front( start );
		//results.push_back( finish );

		return results;
	}

	Maths::MathVector3::MathVector3List NavigationMesh::FindPath( NavigationPolygon* start, NavigationPolygon* finish )
	{
		for( NavigationPolygon::NavPolyList::iterator i = m_polygons.begin( ); i != m_polygons.end( ); ++i )
		{
			( *i )->Reset( );
		}

		MathVector3::MathVector3List waypoints;
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
}