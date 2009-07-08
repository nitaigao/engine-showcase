/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   NavigationPolygon.h
*  @date   2009/07/08
*/
#pragma once
#ifndef NAVIGATIONPOLYGON_H
#define NAVIGATIONPOLYGON_H

#include "Maths/MathVector3.hpp"

namespace AI
{
	/*! 
	 *  A Navigation Polygon used inside the Navigation Mesh
	 */
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


		/*! Sets the Position of the Polygon
		*
		* @param[in] const MathVector3 & position
		* @return ( void )
		*/
		void SetPosition( const Maths::MathVector3& position );


		/*! Adds a Vertex to the Polygon
		*
		* @param[in] const Maths::MathVector3 & vertex
		* @return ( void )
		*/
		inline void AddVertex( const Maths::MathVector3& vertex ) { m_vertices.push_back( vertex ); m_offsetVertices.push_back( vertex ); };


		/*! Returns the number of Vertices's inside this Polygon
		*
		* @return ( int )
		*/
		inline int CountVertices( ) const {	return m_offsetVertices.size( ); }


		/*! Returns a list of Vertices's that make this Polygon
		*
		* @return ( const Maths::MathVector3::MathVector3List& )
		*/
		inline const Maths::MathVector3::MathVector3List& GetVertices( ) const { return m_offsetVertices; }


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


		/*! Resets the list of Neighbors for this Polygon
		*
		* @return ( void )
		*/
		inline void ResetNeighbours( ) { m_neighbours.clear( ); };


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


		/*! Gets the Parent Node of this Polygon
		*
		* @return ( NavigationPolygon* )
		*/
		NavigationPolygon* GetParent( ) const { return m_parent; };


		/*! Sets the Parent Node of this Polygon
		*
		* @param[in] NavigationPolygon * parent
		* @param[in] const NavigationPolygon * finish
		* @return ( void )
		*/
		void SetParent( NavigationPolygon* parent, const NavigationPolygon* finish );


		/*! Gets the Cost of this Polygon
		*
		* @return ( float )
		*/
		float GetCost( ) { return m_cost; };


		/*! Returns whether the Polygon was Marked in a Search
		*
		* @return ( bool )
		*/
		bool IsMarked( ) const { return m_marked; };


		/*! Marks the Polygon for use in a Search
		*
		* @return ( void )
		*/
		void Mark( ) { m_marked = true; };


		/*! Returns the Entry Edge set within a Search
		*
		* @return ( int )
		*/
		int GetEntryEdge( ) const { return m_entryEdge; };


		/*! Resets all the Search Properties for this Polygon
		*
		* @return ( void )
		*/
		void Reset( )
		{
			m_cost = 0;
			m_totalCost = 0;
			m_entryEdge = 0;
			m_parent = 0;
			m_marked = false;
		}

	private:

		NavigationPolygon( const NavigationPolygon & copy ) { };
		NavigationPolygon & operator = ( const NavigationPolygon & copy ) { return *this; };

		Maths::MathVector3::MathVector3List m_vertices;
		Maths::MathVector3::MathVector3List m_offsetVertices;

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