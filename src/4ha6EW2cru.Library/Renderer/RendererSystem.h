/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   RendererSystem.h
*  @date   2009/04/27
*/
#ifndef __RENDERERSYSTEM_H
#define __RENDERERSYSTEM_H

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
			: _configuration( configuration )
			, _root( 0 )
			, _window( 0 )
			, _sceneManager( 0 )
			, _scene( 0 )
			, _configSectionName( "Graphics" )
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
		inline AnyValue::AnyValueMap GetProperties( ) const { return _properties; };


		/*! Sets a System Property
		*
		*  @param[in] const std::string & name
		*  @param[in] AnyValue value
		*  @return (void)
		*/
		inline void SetProperty( const std::string& name, AnyValue value );


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

		void LoadResources( );
		void CreateRenderWindow( const std::string& windowTitle, int width, int height, bool fullScreen );
		std::vector< std::string > GetVideoModes( ) const;

		AnyValue::AnyValueMap _properties;
		Configuration::IConfiguration* _configuration;
		IRenderSystemScene* _scene;

		std::string _configSectionName;
		std::string _skyBoxMaterial;

		Ogre::Root* _root;
		Ogre::SceneManager* _sceneManager;
		Ogre::RenderWindow* _window;

	};
};

#endif