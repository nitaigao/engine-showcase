/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   FPSCameraComponent.h
*  @date   2009/04/27
*/
#ifndef FPSCAMERACOMPONENT_H
#define FPSCAMERACOMPONENT_H

#include "RendererSystemComponent.h"
#include "IRenderSystemScene.h"

namespace Renderer
{
	/*!
	 *  A Camera Component within the Render System 
	 */
	class FPSCameraComponent : public RendererSystemComponent
	{

		typedef std::deque< float > InputHistory;

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~FPSCameraComponent( ) { };


		/*! Default Constructor
		 *
		 *  @param[in] const std::string & name
		 *  @param[in] RendererSystemScene * scene
		 *  @return ()
		 */
		FPSCameraComponent( const std::string& name, IRenderSystemScene* scene )
			: RendererSystemComponent( name, scene )
			, m_cameraNode( 0 )
			, m_totalYaw ( 0 )
			, m_totalPitch( 0 )
			, m_sensitivity( 0.01 )
		{

		}


		/*! Initializes the Component
		*
		*  @param[in] AnyType::AnyValueMap properties
		*  @return (void)
		*/
		void Initialize( );


		/*! Steps the internal data of the Component
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds );

		virtual AnyType Message( const System::Message& message, AnyType::AnyTypeMap parameters );

	private:

		FPSCameraComponent( ) { };
		FPSCameraComponent( const FPSCameraComponent & copy ) { };
		FPSCameraComponent & operator = ( const FPSCameraComponent & copy ) { return *this; };

		Ogre::SceneNode* m_cameraNode;

		float m_totalYaw;
		float m_totalPitch;

		float m_sensitivity;

	};
};

#endif