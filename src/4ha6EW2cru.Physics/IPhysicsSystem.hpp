#ifndef IPHYSICSSYSTEM_HPP
#define IPHYSICSSYSTEM_HPP

#include "System/ISystem.hpp"

class IPhysicsSystem : public ISystem
{

public:

	~IPhysicsSystem( ) { };

	/*! Returns the Physics World
	*
	*  @return (hkpWorld*)
	*/
	virtual hkpWorld* GetWorld( ) const = 0;

};

#endif