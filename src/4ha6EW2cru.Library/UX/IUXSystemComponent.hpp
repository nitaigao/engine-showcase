/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IUXSystemComponent.hpp
*  @date   2009/04/26
*/
#ifndef IUXSYSTEMCOMPONENT_H
#define IUXSYSTEMCOMPONENT_H

#include "../System/ISystemComponent.hpp"

namespace UX
{
	/*! 
	*  A UX System Scene Component
	*/
	class IUXSystemComponent : public ISystemComponent
	{

	public:

		typedef std::deque< IUXSystemComponent* > UXSystemComponentList;

		virtual ~IUXSystemComponent( ) { };

	};
};

#endif