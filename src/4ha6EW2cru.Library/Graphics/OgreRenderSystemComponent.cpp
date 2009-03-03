#include "OgreRenderSystemComponent.h"

#include <Ogre.h>
using namespace Ogre;

#include "../Utility/OgreMax/OgreMaxModel.hpp"
using namespace OgreMax;

void OgreRenderSystemComponent::Initialize( SystemPropertyList properties )
{
	for ( SystemPropertyList::iterator i = properties.begin( ); i != properties.end( ); ++i )
	{
		if ( ( *i ).GetName( ) == "model" )
		{
			std::string modelPath = ( *i ).GetValue< std::string >( );

			OgreMaxModel* model = new OgreMaxModel( );
			model->Load( modelPath );

			SceneNode* modelNode = model->CreateInstance( this->getCreator( ), Ogre::StringUtil::BLANK, 0, OgreMaxModel::NO_OPTIONS, 0, 
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, this );
		}
	}

	this->getCreator( )->getRootSceneNode( )->addChild( this );
}