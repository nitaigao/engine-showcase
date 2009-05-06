/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   RendererSystemCameraComponent.h
*  @date   2009/04/27
*/
#ifndef __RENDERERSYSTEMCAMERACOMPONENT_H
#define __RENDERERSYSTEMCAMERACOMPONENT_H

#include "RendererSystemComponent.h"
#include "IRenderSystemScene.h"

namespace Renderer
{
	/*!
	 *  A Camera Component within the Render System 
	 */
	class RendererSystemCameraComponent : public RendererSystemComponent
	{

		typedef std::deque< float > History;

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~RendererSystemCameraComponent( ) { };


		/*! Default Constructor
		 *
		 *  @param[in] const std::string & name
		 *  @param[in] RendererSystemScene * scene
		 *  @return ()
		 */
		RendererSystemCameraComponent( const std::string& name, IRenderSystemScene* scene )
			: RendererSystemComponent( name, scene )
			, _observer( 0 )
			, _historySize( 10 )
			, _weightModifier( 0.5f ) 
			, _cameraNode( 0 )
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
		void Update( const float& deltaMilliseconds );


		/*! Adds an Observer to the Component
		*
		*  @param[in] IObserver * observer
		*  @return (void)
		*/
		void AddObserver( IObserver* observer ) { _observer = observer; };


		/*! Observes a change in the Subject
		*
		*  @param[in] ISubject * subject
		*  @param[in] const unsigned int& systemChanges
		*  @return (void)
		*/
		void Observe( ISubject* subject, const unsigned int& systemChanges );


		/*! Pushes any Changes to the Observers
		*
		*  @param[in] const unsigned int& systemChanges
		*  @return (void)
		*/
		void PushChanges( const unsigned int& systemChanges );

	protected:

		void InitializeSceneNode( Ogre::SceneNode* sceneNode );

	private:

		RendererSystemCameraComponent( ) { };
		RendererSystemCameraComponent( const RendererSystemCameraComponent & copy ) { };
		RendererSystemCameraComponent & operator = ( const RendererSystemCameraComponent & copy ) { return *this; };

		float AverageInputHistory( const History& inputHistory, const float& weightModifier );

		IObserver* _observer;
		History _xHistory;
		History _yHistory;
		Ogre::SceneNode* _cameraNode;

		float _weightModifier;
		int _historySize;

	};
};

#endif