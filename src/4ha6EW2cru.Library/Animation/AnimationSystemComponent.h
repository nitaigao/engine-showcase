/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   AnimationSystemComponent.h
*  @date   2009/04/25
*/
#ifndef ANIMATIONSYSTEMCOMPONENT_H
#define ANIMATIONSYSTEMCOMPONENT_H

#include "IAnimationSystemComponent.hpp"
#include "IAnimationSystemScene.hpp"
#include "IAnimationNode.hpp"

#include <Common/Base/hkBase.h>
#include <Common/Serialize/Util/hkLoader.h>

#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>
#include <Animation/Animation/Deform/Skinning/hkaMeshBinding.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>


namespace Animation
{
	/*! 
	 *  An Animation System Component
	 */
	class AnimationSystemComponent : public IAnimationSystemComponent
	{

		typedef std::deque< hkaAnimationBinding* > AnimationBindingList;
		typedef std::deque< hkaAnimation* > AnimationList;
		typedef std::map< std::string, hkaDefaultAnimationControl* > AnimationControlList;

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~AnimationSystemComponent( );


		/*! Default Constructor
		*
		* @return (  )
		*/
		AnimationSystemComponent( IAnimationSystemScene* scene )
			: m_scene( scene )
			, m_observer( 0 )
			, m_skeletonInstance( 0 )
			, m_loadedData( 0 )
		{ 
		
		};


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


		/*! Destroys the Component
		*
		*  @return (void)
		*/
		void Destroy( ) { };


		/*! Adds an Observer to the Component
		*
		*  @param[in] IObserver * observer
		*  @return (void)
		*/
		inline void AddObserver( IObserver* observer ) { m_observer = observer; };


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


		/*! Posts a message to observers
		*
		*  @param[in] const std::string & message
		*  @param[in] AnyType::AnyValueMap parameters
		*  @return (AnyType)
		*/
		inline AnyType PushMessage( const System::Message& message, AnyType::AnyTypeMap parameters ) { return m_observer->Message( message, parameters ); };


		/*! Messages the Component to influence its internal state
		*
		*  @param[in] const std::string & message
		*  @return (AnyType)
		*/
		AnyType Message( const System::Message& message, AnyType::AnyTypeMap parameters );

		void TransformBone( hkaPose* pose, Ogre::Node* bone );

	private:

		AnimationSystemComponent( const AnimationSystemComponent & copy ) { };
		AnimationSystemComponent & operator = ( const AnimationSystemComponent & copy ) { return *this; };

		void LoadAnimation( const std::string& animationName, const std::string& animationPath );

		IAnimationSystemScene* m_scene;
		AnyType::AnyTypeMap m_attributes;

		IObserver* m_observer;

		hkPackfileData* m_loadedData;
		hkaSkeleton* m_skeleton;
		hkaAnimatedSkeleton* m_skeletonInstance;

		AnimationList m_animations;
		AnimationBindingList m_animationBindings;
		AnimationControlList m_animationControls;

		Ogre::SkeletonInstance* m_ogreSkeleton;
		
	};
};

#endif