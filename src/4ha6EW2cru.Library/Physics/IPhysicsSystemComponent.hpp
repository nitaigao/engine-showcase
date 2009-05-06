/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IPhysicsSystemComponent.hpp
*  @date   2009/05/05
*/
#ifndef __IPHYSICSSYSTEMCOMPONENT_HPP
#define __IPHYSICSSYSTEMCOMPONENT_HPP

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