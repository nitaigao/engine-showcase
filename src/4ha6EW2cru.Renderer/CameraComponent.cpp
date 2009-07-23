#include "CameraComponent.h"
#include <Ogre.h>
using namespace Ogre;

namespace Renderer
{
	void CameraComponent::Initialize( )
	{
		RendererSystemComponent::Initialize( );

		std::string name = m_attributes[ System::Attributes::Name ].As< std::string >( );

		std::stringstream cameraName;
		cameraName << name << "_" << name;

		Camera* camera = m_scene->GetSceneManager( )->getCamera( cameraName.str( ) );
		m_scene->GetSceneManager( )->getCurrentViewport( )->setCamera( camera );
	}
}