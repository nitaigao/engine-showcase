/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   CameraComponent.h
*  @date   2009/07/21
*/
#pragma once
#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "RendererSystemComponent.h"

namespace Renderer
{
	/*! 
	 *  A Camera within the Scene
	 */
	class CameraComponent : public RendererSystemComponent
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~CameraComponent( ) { };


		/*! Default Constructor
		*
		*  @param[in] const std::string & name
		*  @param[in] RendererSystemScene * scene
		*  @return ()
		*/
		CameraComponent( const std::string& name, IRenderSystemScene* scene )
			: RendererSystemComponent( name, scene )
		{

		}

		/*! Initializes the Component
		*
		*  @param[in] AnyType::AnyValueMap properties
		*  @return (void)
		*/
		void Initialize( );

	private:

		CameraComponent( const CameraComponent & copy ) { };
		CameraComponent & operator = ( const CameraComponent & copy ) { return *this; };
		
	};
};

#endif