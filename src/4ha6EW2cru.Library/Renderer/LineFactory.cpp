#include "LineFactory.h"

namespace Renderer
{
	Ogre::String LineFactory::FACTORY_TYPE_NAME = "Line";

	const Ogre::String& LineFactory::getType( void ) const
	{
		return FACTORY_TYPE_NAME;
	}
}