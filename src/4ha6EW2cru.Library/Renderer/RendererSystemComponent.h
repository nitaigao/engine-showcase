/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   RendererSystemComponent.h
*  @date   2009/04/27
*/
#ifndef __RENDERERSYSTEMCOMPONENT_H
#define __RENDERERSYSTEMCOMPONENT_H

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

		typedef std::vector< IAnimationBlender* > AnimationBlenderList;

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
			: _name( name )
			, _id( 0 )
			, _sceneNode( 0 )
			, _scene( scene )
		{

		}


		/*! Initializes the Component
		*
		*  @param[in] AnyValue::AnyValueMap properties
		*  @return (void)
		*/
		void Initialize( AnyValue::AnyValueMap& properties );


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
		virtual void AddObserver( IObserver* observer ) { };


		/*! Observes a change in the Subject
		*
		*  @param[in] ISubject * subject
		*  @param[in] const unsigned int& systemChanges
		*  @return (void)
		*/
		virtual void Observe( ISubject* subject, const unsigned int& systemChanges );


		/*! Pushes any Changes to the Observers
		*
		*  @param[in] const unsigned int& systemChanges
		*  @return (void)
		*/
		virtual void PushChanges( const unsigned int& systemChanges ) { };


		/*! Gets the types of Changes this component is interested in
		*
		*  @return (unsigned int)
		*/
		inline unsigned int GetRequestedChanges( ) const  
		{ 
			return 
				System::Changes::Geometry::All |
				System::Changes::Input::Mouse_Moved |
				System::Changes::AI::Behavior;
		};


		/*! Gets the Name of the Component
		*
		*  @return (const std::string&)
		*/
		inline const std::string& GetName( ) const { return _name; };


		/*! Sets the Id of the component unique to its containing World Entity
		*
		*  @param[in] const unsigned int & id
		*  @return (void)
		*/
		inline void SetId( const unsigned int& id ) { _id = id; };


		/*! Returns a numerical Id for the component unique to its containing World Entity
		*
		*  @return (unsigned int)
		*/
		inline unsigned int GetId( ) const { return _id; };


		/*! Gets the System::Types::Type of the Component
		*
		*  @return (System::Types::Type)
		*/
		inline System::Types::Type GetType( ) const { return System::Types::RENDER; };


		/*! Gets the properties of the Component
		*
		*  @return (AnyValueMap)
		*/
		AnyValue::AnyValueMap GetProperties( ) const { return _properties; };


		/*! Sets the Properties of the Component
		*
		*  @param[in] AnyValue::AnyValueMap systemProperties
		*  @return (void)
		*/
		void SetProperties( AnyValue::AnyValueMap& properties ) { };


		/*! Gets the Position of the Component
		*
		*  @return (MathVector3)
		*/
		inline Maths::MathVector3 GetPosition( ) const { return Maths::MathVector3( _sceneNode->getPosition( ) ); };


		/*! Gets the Scale of the Component
		*
		*  @return (MathVector3)
		*/
		inline Maths::MathVector3 GetScale( ) const { return Maths::MathVector3( _sceneNode->getScale( ) ); };


		/*! Gets the Orientation of the Component
		*
		*  @return (MathQuaternion)
		*/
		inline Maths::MathQuaternion GetOrientation( ) const { return Maths::MathQuaternion( _sceneNode->getOrientation( ) ); };

		void PlayAnimation( const std::string& animationName, const bool& loopAnimation );

	protected:

		RendererSystemComponent( ) { };

		virtual void InitializeSceneNode( Ogre::SceneNode* sceneNode );
		void LoadModel( Ogre::SceneNode* sceneNode, const std::string& modelPath );
		void DestroySceneNode( Ogre::SceneNode* sceneNode );

		std::string _name;
		unsigned int _id;

		AnyValue::AnyValueMap _properties;

		IRenderSystemScene* _scene;
		Ogre::SceneNode* _sceneNode;

		AnimationBlenderList _animationBlenders;

	private:

		RendererSystemComponent( const RendererSystemComponent & copy ) { };
		RendererSystemComponent & operator = ( const RendererSystemComponent & copy ) { return *this; };

	};
};

#endif