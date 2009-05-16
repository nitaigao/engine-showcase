/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   RendererSystemCameraComponent.h
*  @date   2009/04/27
*/
#ifndef RENDERERSYSTEMCAMERACOMPONENT_H
#define RENDERERSYSTEMCAMERACOMPONENT_H

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
			, m_observer( 0 )
			, m_cameraNode( 0 )
			, m_weightModifier( 0.5f )
			, m_historySize( 10 )
			, m_totalYaw ( 0 )
			, m_totalPitch( 0 )
			, m_sensitivity( 0.01 )
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
		void AddObserver( IObserver* observer ) { m_observer = observer; };


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
		float AverageInputHistory( const History& inputHistory, const float& weightModifier );

	private:

		RendererSystemCameraComponent( ) { };
		RendererSystemCameraComponent( const RendererSystemCameraComponent & copy ) { };
		RendererSystemCameraComponent & operator = ( const RendererSystemCameraComponent & copy ) { return *this; };

		IObserver* m_observer;
		Ogre::SceneNode* m_cameraNode;

		History m_xHistory;
		History m_yHistory;

		float m_weightModifier;
		int m_historySize;

		float m_totalYaw;
		float m_totalPitch;

		float m_sensitivity;

	};
};

#endif