/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   RendererSystem.h
*  @date   2009/04/27
*/
#ifndef RENDERERSYSTEM_H
#define RENDERERSYSTEM_H

#include <Ogre.h>

#include "../Service/IService.hpp"
#include "../Configuration/IConfiguration.hpp"

#include "IRendererSystem.hpp"
#include "IRenderSystemScene.h"

namespace Renderer 
{
	/*!
	 *  The Render System 
	 */
	class RendererSystem : public IRendererSystem, public IService, public Ogre::WindowEventListener
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~RendererSystem( );


		/*! Default Constructor
		*
		*  @return ()
		*/
		RendererSystem( Configuration::IConfiguration* configuration )
			: m_configuration( configuration )
			, m_root( 0 )
			, m_window( 0 )
			, m_sceneManager( 0 )
			, m_scene( 0 )
			, m_configSectionName( "Graphics" )
		{

		}


		/*! Initializes the System
		*
		*  @return (void)
		*/
		void Initialize( );


		/*! Steps the System's internal data
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds );


		/*! Releases internal data of the System
		*
		*  @return (System::Types::Type)
		*/
		void Release( ) { };


		/*! Returns the type of the System
		*
		*  @return (System::Types::Type)
		*/
		inline System::Types::Type GetType( ) const { return System::Types::RENDER; };


		/*! Creates a System Scene
		*
		*  @return (ISystemScene*)
		*/
		inline ISystemScene* CreateScene( );


		/*! Gets the System's Properties
		*
		*  @return (AnyValueMap)
		*/
		inline AnyValue::AnyValueMap GetAttributes( ) const { return m_attributes; };


		/*! Sets a System Property
		*
		*  @param[in] const std::string & name
		*  @param[in] AnyValue value
		*  @return (void)
		*/
		inline void SetAttribute( const std::string& name, AnyValue value );


		/*! Renderer Service Interface
		 *
		 *  @param[in] const std::string & actionName
		 *  @param[in] AnyValue::AnyValueMap & parameters
		 *  @return (AnyValue::AnyValueMap)
		 */
		AnyValue::AnyValueMap Execute( const std::string& actionName, AnyValue::AnyValueMap& parameters );
		

	private:

		RendererSystem( const RendererSystem & copy ) { };
		RendererSystem & operator = ( const RendererSystem & copy ) { return *this; };

		void windowClosed( Ogre::RenderWindow* rw );

		void CreateRenderWindow( const std::string& windowTitle, int width, int height, bool fullScreen );
		std::vector< std::string > GetVideoModes( ) const;

		AnyValue::AnyValueMap m_attributes;
		Configuration::IConfiguration* m_configuration;
		IRenderSystemScene* m_scene;

		std::string m_configSectionName;
		std::string m_skyBoxMaterial;

		Ogre::Root* m_root;
		Ogre::SceneManager* m_sceneManager;
		Ogre::RenderWindow* m_window;

	};
};

#endif