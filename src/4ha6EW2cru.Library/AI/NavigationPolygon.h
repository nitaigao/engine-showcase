#pragma once
#ifndef NAVIGATIONPOLYGON_H
#define NAVIGATIONPOLYGON_H

#include "../Maths/MathVector3.hpp"

namespace AI
{
	class NavigationPolygon
	{

	public:

		typedef std::deque< NavigationPolygon* > NavPolyList;
		typedef std::pair< Maths::MathVector3, Maths::MathVector3 > PolyEdge;
		typedef std::deque< PolyEdge > PolyEdgeList;


		/*! Default Destructor
		*
		* @return (  )
		*/
		~NavigationPolygon( ) { };


		/*! Default Constructor
		*
		* @return (  )
		*/
		NavigationPolygon( )
			: m_cost( 0 )
			, m_totalCost( 0 )
			, m_parent( 0 )
			, m_marked( false )
			, m_entryEdge( 0 )
		{ 

		};


		/*! Adds a Vertex to the Polygon
		*
		* @param[in] const Maths::MathVector3 & position
		* @return ( void )
		*/
		inline void AddVertex( const Maths::MathVector3& position ) { m_vertices.push_back( position ); };


		/*! Returns the number of Vertices's inside this Polygon
		*
		* @return ( int )
		*/
		inline int CountVertices( ) const {	return m_vertices.size( ); }


		/*! Returns a list of Vertices's that make this Polygon
		*
		* @return ( const Maths::MathVector3::MathVector3List& )
		*/
		inline const Maths::MathVector3::MathVector3List& GetVertices( ) const { return m_vertices; };


		/*! Returns whether the given polygon is a neighbor of this Polygon
		*
		* @param[in] NavigationPolygon * polygon
		* @return ( bool )
		*/
		bool IsNeighbour( const NavigationPolygon* polygon ) const;


		/*! Adds a Polygon as a Neighbor to this Polygon
		*
		* @param[in] NavigationPolygon * polygon
		* @return ( void )
		*/
		void AddNeighbour( NavigationPolygon* polygon );


		/*! Returns all registered Neighbors of this Polygon
		*
		* @return ( const NavPolyList& )
		*/
		inline const NavPolyList& GetNeighbours( ) const { return m_neighbours; };


		/*! Returns a List of Edges that are linked to other Polygons
		*
		* @return ( const PolyEdgeList& )
		*/
		inline const PolyEdgeList& GetEdgeLinks( ) const { return m_edgeLinks; };
		

		/*! Returns the Center Coordinates of this Polygon
		*
		* @return ( Maths::MathVector3 )
		*/
		Maths::MathVector3 GetCenter( ) const;
		

		/*! Returns whether or not the given point resides inside this Polygon
		*
		* @param[in] const Maths::MathVector3 & point
		* @return ( bool )
		*/
		bool IsPointInside( const Maths::MathVector3& point ) const;


		NavigationPolygon* GetParent( ) const { return m_parent; };

		void SetParent( NavigationPolygon* parent, const NavigationPolygon* finish )
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

		float GetCost( ) { return m_cost; };

		bool IsMarked( ) const { return m_marked; };
		void Mark( ) { m_marked = true; };

		int GetEntryEdge( ) const { return m_entryEdge; };

		void Reset( )
		{
			m_cost = 0;
			m_totalCost = 0;
			m_entryEdge = 0;
			m_parent = 0;
			m_marked = false;
		}

	private:

		Maths::MathVector3::MathVector3List m_vertices;
		NavPolyList m_neighbours;
		PolyEdgeList m_edgeLinks;


		NavigationPolygon* m_parent;
		float m_cost;
		float m_totalCost;
		int m_entryEdge;
		bool m_marked;

		PolyEdgeList GetEdges( ) const;
		bool SameSide( const Maths::MathVector3& point1, const Maths::MathVector3& point2, const Maths::MathVector3& a, const Maths::MathVector3& b ) const;

	};
};


#endif
