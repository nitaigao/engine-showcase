#include "NavigationPolygon.h"

using namespace Maths;

namespace AI
{
	void NavigationPolygon::SetPosition( const Maths::MathVector3& position )
	{
		m_offsetVertices.clear( );

		for( Maths::MathVector3::MathVector3List::const_iterator i = m_vertices.begin( ); i != m_vertices.end( ); ++i )
		{
			m_offsetVertices.push_back( ( *i ) + position );
		}
	}

	NavigationPolygon::PolyEdgeList NavigationPolygon::GetEdges( ) const
	{
		PolyEdgeList edges;

		for ( unsigned int i = 1; i < m_offsetVertices.size( ); i++ )
		{
			PolyEdge edge( m_offsetVertices[ i - 1 ], m_offsetVertices[ i ] );
			edges.push_back( edge );
		}

		PolyEdge lastEdge( m_offsetVertices[ m_offsetVertices.size( ) - 1 ], m_offsetVertices[ 0 ] );

		edges.push_back( lastEdge );

		return edges;
	}

	Maths::MathVector3 NavigationPolygon::GetCenter( ) const
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		for( Maths::MathVector3::MathVector3List::const_iterator i = m_offsetVertices.begin( ); i != m_offsetVertices.end( ); ++i )
		{
			x += ( *i ).X;
			y += ( *i ).Y;
			z += ( *i ).Z;
		}

		x /= m_offsetVertices.size( );
		y /= m_offsetVertices.size( );
		z /= m_offsetVertices.size( );

		return Maths::MathVector3( x, y, z );
	}

	bool NavigationPolygon::IsPointInside( const Maths::MathVector3& point ) const
	{
		return
			this->SameSide( point, m_offsetVertices[ 0 ], m_offsetVertices[ 1 ], m_offsetVertices[ 2 ] ) &&
			this->SameSide( point, m_offsetVertices[ 1 ], m_offsetVertices[ 0 ], m_offsetVertices[ 2 ] ) &&
			this->SameSide( point, m_offsetVertices[ 2 ], m_offsetVertices[ 0 ], m_offsetVertices[ 1 ] );
	}

	bool NavigationPolygon::SameSide( const Maths::MathVector3& point1, const Maths::MathVector3& point2, const Maths::MathVector3& a, const Maths::MathVector3& b ) const
	{
		Maths::MathVector3 crossPoint1 = ( b - a ).CrossProduct( point1 - a );
		Maths::MathVector3 crossPoint2 = ( b - a ).CrossProduct( point2 - a );

		return ( crossPoint1.DotProduct( crossPoint2 ) >= 0 );
	}

	bool NavigationPolygon::IsNeighbour( const NavigationPolygon* polygon ) const
	{
		const NavigationPolygon::PolyEdgeList existingEdges = this->GetEdges( );

		for ( NavigationPolygon::PolyEdgeList::const_iterator existingEdge = existingEdges.begin( ); existingEdge != existingEdges.end( ); ++existingEdge )
		{
			const NavigationPolygon::PolyEdgeList newEdges = polygon->GetEdges( );

			for ( NavigationPolygon::PolyEdgeList::const_iterator newEdge = newEdges.begin( ); newEdge != newEdges.end( ); ++newEdge )
			{
				if ( 
					( *existingEdge ).first == ( *newEdge ).first &&
					( *existingEdge ).second == ( *newEdge ).second ||
					( *existingEdge ).second == ( *newEdge ).first &&
					( *existingEdge ).first == ( *newEdge ).second
					)
				{
					return true;
				}
			}
		}

		return false;
	}

	void NavigationPolygon::AddNeighbour( NavigationPolygon* polygon )
	{
		NavigationPolygon::PolyEdgeList existingEdges = this->GetEdges( );

		for ( NavigationPolygon::PolyEdgeList::iterator existingEdge = existingEdges.begin( ); existingEdge != existingEdges.end( ); ++existingEdge )
		{
			NavigationPolygon::PolyEdgeList newEdges = polygon->GetEdges( );

			for ( NavigationPolygon::PolyEdgeList::iterator newEdge = newEdges.begin( ); newEdge != newEdges.end( ); ++newEdge )
			{
				if ( 
					( *existingEdge ).first == ( *newEdge ).first &&
					( *existingEdge ).second == ( *newEdge ).second ||
					( *existingEdge ).second == ( *newEdge ).first &&
					( *existingEdge ).first == ( *newEdge ).second
					)
				{
					m_neighbours.push_back( polygon );
					m_edgeLinks.push_back( *existingEdge );
				}
			}
		}
	}

	void NavigationPolygon::SetParent( NavigationPolygon* parent, const NavigationPolygon* finish )
	{
		m_parent = parent;

		for( unsigned int i = 0; i < this->GetNeighbours( ).size( ); i++ )
		{
			if ( this->GetNeighbours( )[ i ] == parent )
			{
				m_entryEdge = i;
			}
		}

		m_cost = ( this->GetCenter( ) - parent->GetCenter( ) ).Length( ) + parent->m_cost;

		float costToGoal = ( this->GetCenter( ) = finish->GetCenter( ) ).Length( );
		m_totalCost = m_cost + costToGoal;

		m_marked = false;
	}
	
}