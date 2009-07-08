/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   AINavigationMeshComponent.h
*  @date   2009/07/08
*/
#pragma once
#ifndef AINAVIGATIONMESHCOMPONENT_H
#define AINAVIGATIONMESHCOMPONENT_H

#include "AISystemComponent.h"
#include "NavigationPolygon.h"

#include "Maths/MathVector3.hpp"

namespace AI
{
	/*! 
	 *  A Component that contains the Navigation Mesh for the AI Characters
	 */
	class AINavigationMeshComponent : public AISystemComponent
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~AINavigationMeshComponent( );


		AINavigationMeshComponent( const std::string& name )
			: AISystemComponent( name )
		{

		}


		/*! Initializes the Component
		*
		* @return ( void )
		*/
		void Initialize( );


		/*! Destroys the Component
		*
		* @return ( void )
		*/
		void Destroy( );


		AnyType Message( const System::Message& message, AnyType::AnyTypeMap parameters );

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

	private:

		AINavigationMeshComponent( ) { };
		AINavigationMeshComponent( const AINavigationMeshComponent & copy ) { };
		AINavigationMeshComponent & operator = ( const AINavigationMeshComponent & copy ) { return *this; };

		void RecalculateNeighbours( );

		NavigationPolygon::NavPolyList m_polygons;
		
	};
};

#endif