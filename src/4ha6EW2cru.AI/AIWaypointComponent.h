/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   AIWaypointComponent.h
*  @date   2009/07/08
*/
#pragma once
#ifndef AIWAYPOINTCOMPONENT_H
#define AIWAYPOINTCOMPONENT_H

namespace AI
{
	/*! 
	 *  A Waypoint component used in conjunction with the Navigation Mesh
	 */
	class AIWaypointComponent : public AISystemComponent
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~AIWaypointComponent( ) { };


		AIWaypointComponent( const std::string& name )
			: AISystemComponent( name )
		{

		}

	private:

		AIWaypointComponent( ) { };
		AIWaypointComponent( const AIWaypointComponent & copy ) { };
		AIWaypointComponent & operator = ( const AIWaypointComponent & copy ) { return *this; };
		
	};
};

#endif