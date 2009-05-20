/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IPhysicsSystemComponent.hpp
*  @date   2009/05/05
*/
#ifndef IPHYSICSSYSTEMCOMPONENT_HPP
#define IPHYSICSSYSTEMCOMPONENT_HPP

#include "../System/ISystemComponent.hpp"

#include <Physics/Dynamics/Entity/hkpRigidBody.h>

namespace Physics
{
	/*!
	 *  A Physics System Component 
	 */
	class IPhysicsSystemComponent : public ISystemComponent
	{

	public:

		typedef std::map< std::string, IPhysicsSystemComponent* > PhysicsSystemComponentList;

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IPhysicsSystemComponent( ) { };


		/*! Returns the RigidBody of the Component
		 *
		 *  @return (hkpRigidBody*)
		 */
		virtual hkpRigidBody* GetRigidBody( ) const = 0;

	};
};

#endif