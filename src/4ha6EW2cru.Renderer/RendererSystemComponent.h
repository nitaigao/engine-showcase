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
#include "IRenderSystemScene.h"

namespace Renderer
{
	/*!
	 *  A normal Render System Component 
	 */
	class RendererSystemComponent : public IRendererSystemComponent
	{

		typedef std::deque< Ogre::Skeleton* > SkeletonList;

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
		*  @param[in] AnyType::AnyTypeMap properties
		*  @return (void)
		*/
		void Initialize( );


		/*! Steps the internal data of the Component
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		virtual void Update( const float& deltaMilliseconds ) { };


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
		*  @return (AnyTypeKeyMap)
		*/
		AnyType::AnyTypeMap GetAttributes( ) const { return m_attributes; };


		/*! Sets an Attribute on the Component *
		*
		*  @param[in] const unsigned int attributeId
		*  @param[in] const AnyType & value
		*/
		inline void SetAttribute( const System::Attribute& attributeId, const AnyType& value ) { m_attributes[ attributeId ] = value; };


		/*! Sets the Properties of the Component
		*
		*  @param[in] AnyType::AnyTypeMap systemProperties
		*  @return (void)
		*/
		void SetAttributes( AnyType::AnyTypeMap& properties ) { };


		/*! Posts a message to observers
		*
		*  @param[in] const std::string & message
		*  @param[in] AnyType::AnyTypeMap parameters
		*  @return (AnyType)
		*/
		AnyType PushMessage( const System::Message& message, AnyType::AnyTypeMap parameters );


		/*! Messages the Component to influence its internal state
		*
		*  @param[in] const std::string & message
		*  @return (AnyType)
		*/
		virtual AnyType Message( const System::Message& message, AnyType::AnyTypeMap parameters );

	protected:

		RendererSystemComponent( ) { };

		void LoadModel( Ogre::SceneNode* sceneNode, const std::string& modelPath );
		void DestroySceneNode( Ogre::SceneNode* sceneNode );
		void LinkSkeletons( Ogre::SceneNode* sceneNode, RendererSystemComponent::SkeletonList* skeletons );

		std::string m_name;

		AnyType::AnyTypeMap m_attributes;

		IObserver* m_observer;
		IRenderSystemScene* m_scene;
		Ogre::SceneNode* m_sceneNode;

	private:

		RendererSystemComponent( const RendererSystemComponent & copy ) { };
		RendererSystemComponent & operator = ( const RendererSystemComponent & copy ) { return *this; };

	};
};

#endif