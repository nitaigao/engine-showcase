#pragma once
#ifndef NAVMESH_HPP
#define NAVMESH_HPP

#include "INavigationMesh.hpp"
#include "NavigationPolygon.h"

#include "../Maths/MathVector3.hpp"

namespace AI
{
	class NavigationMesh : public INavigationMesh
	{

	public:

		/*! Default Destructor
		*
		* @return (  )
		*/
		~NavigationMesh( );


		/*! Adds a Polygon to the Mesh
		*
		* @param[in] NavigationPolygon * polygon
		* @return ( void )
		*/
		void AddPolygon( NavigationPolygon* polygon );


		/*! Returns a Path in way points from the start point to the destination point
		*
		* @param[in] const Maths::MathVector3 & start
		* @param[in] const Maths::MathVector3 & finish
		* @return ( Maths::MathVector3::MathVector3List )
		*/
		Maths::MathVector3::MathVector3List FindPath( const Maths::MathVector3& start, const Maths::MathVector3& finish );


		/*! Returns a Path in way points from the start Polygon to the destination Polygon
		*
		* @param[in] NavigationPolygon * start
		* @param[in] NavigationPolygon * finish
		* @return ( Maths::MathVector3::MathVector3List )
		*/
		Maths::MathVector3::MathVector3List FindPath( NavigationPolygon* start, NavigationPolygon* finish  );



		void Render( );

	private:

		NavigationPolygon::NavPolyList m_polygons;

	};
};

#endif