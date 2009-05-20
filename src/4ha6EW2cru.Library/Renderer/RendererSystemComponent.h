/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   RendererSystemComponent.h
*  @date   2009/04/27
*/
#ifndef RENDERERSYSTEMCOMPONENT_H
#define RENDERERSYSTEMCOMPONENT_H

#include <Ogre.h>

#include "IRendererSystemComponent.hpp"
#include "IAnimationBlender.hpp"
#include "IRenderSystemScene.h"

namespace Renderer
{
	/*!
	 *  A normal Render System Component 
	 */
	class RendererSystemComponent : public IRendererSystemComponent
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		virtual ~RendererSystemComponent( );


		/*! Default Constructor
		*
		*  @param[in] const std::string & name
		*  @return ()
		*/
		RendererSystemComponent( const std::string& name, IRenderSystemScene* scene )
			: m_name( name )
			, m_sceneNode( 0 )
			, m_scene( scene )
			, m_observer( 0 )
		{

		}


		/*! Initializes the Component
		*
		*  @param[in] AnyValue::AnyValueMap properties
		*  @return (void)
		*/
		void Initialize( );


		/*! Steps the internal data of the Component
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		virtual void Update( const float& deltaMilliseconds );


		/*! Destroys the Component
		*
		*  @return (void)
		*/
		void Destroy( );


		/*! Adds an Observer to the Component
		*
		*  @param[in] IObserver * observer
		*  @return (void)
		*/
		void AddObserver( IObserver* observer ) { m_observer = observer; };


		/*! Gets the properties of the Component
		*
		*  @return (AnyValueKeyMap)
		*/
		AnyValue::AnyValueKeyMap GetAttributes( ) const { return m_attributes; };


		/*! Sets an Attribute on the Component *
		*
		*  @param[in] const unsigned int attributeId
		*  @param[in] const AnyValue & value
		*/
		inline void SetAttribute( const unsigned int& attributeId, const AnyValue& value ) { m_attributes[ attributeId ] = value; };


		/*! Sets the Properties of the Component
		*
		*  @param[in] AnyValue::AnyValueMap systemProperties
		*  @return (void)
		*/
		void SetAttributes( AnyValue::AnyValueMap& properties ) { };

		void PlayAnimation( const std::string& animationName, const bool& loopAnimation );


		/*! Posts a message to observers
		*
		*  @param[in] const std::string & message
		*  @param[in] AnyValue::AnyValueMap parameters
		*  @return (AnyValue)
		*/
		AnyValue PushMessage( const unsigned int& messageId, AnyValue::AnyValueKeyMap parameters );


		/*! Messages the Component to influence its internal state
		*
		*  @param[in] const std::string & message
		*  @return (AnyValue)
		*/
		virtual AnyValue Message( const unsigned int& messageId, AnyValue::AnyValueKeyMap parameters );

	protected:

		RendererSystemComponent( ) { };

		virtual void InitializeSceneNode( Ogre::SceneNode* sceneNode );
		void LoadModel( Ogre::SceneNode* sceneNode, const std::string& modelPath );
		void DestroySceneNode( Ogre::SceneNode* sceneNode );

		std::string m_name;

		AnyValue::AnyValueKeyMap m_attributes;

		IObserver* m_observer;
		IRenderSystemScene* m_scene;
		Ogre::SceneNode* m_sceneNode;

		IAnimationBlender::AnimationBlenderList m_animationBlenders;

	private:

		RendererSystemComponent( const RendererSystemComponent & copy ) { };
		RendererSystemComponent & operator = ( const RendererSystemComponent & copy ) { return *this; };

	};
};

#endif