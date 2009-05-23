/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IInputSystemScene.hpp
*  @date   2009/04/26
*/
#ifndef IINPUTSYSTEMSCENE_H
#define IINPUTSYSTEMSCENE_H

#include "IInputSystem.hpp"
#include "../System/ISystemScene.hpp"

namespace Input
{
	class IInputSystemScene : public ISystemScene
	{

	public:

		virtual ~IInputSystemScene( ) { };

		virtual IInputSystem* GetSystem( ) = 0;

	};
};


#endif