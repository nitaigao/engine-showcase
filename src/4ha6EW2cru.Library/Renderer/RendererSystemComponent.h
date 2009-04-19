#ifndef __OGRERENDERSYSTEMCOMPONENT_H
#define __OGRERENDERSYSTEMCOMPONENT_H

#include "../Utility/OgreMax/OgreMaxModel.hpp"
#include "../System/ISystemComponent.hpp"

#include "IAnimationBlender.hpp"
#include "RendererSystemScene.h"


class RendererSystemComponent : public ISystemComponent
{

	typedef std::vector< AnimationState* > AnimationStateList;
	typedef std::vector< IAnimationBlender* > AnimationBlenderList;

public:

	virtual ~RendererSystemComponent( );

	RendererSystemComponent( const std::string& name, RendererSystemScene* scene )
		: _name( name )
		, _sceneNode( 0 )
		, _scene( scene )
	{

	}

	void Initialize( AnyValueMap properties );
	void Destroy( );

	virtual void AddObserver( IObserver* observer ) { };
	virtual void PushChanges( unsigned int systemChanges ) { };
	virtual void Observe( ISubject* subject, unsigned int systemChanges );

	inline unsigned int GetRequestedChanges( ) 
	{ 
		return 
			System::Changes::Geometry::Position | 
			System::Changes::Geometry::Orientation | 
			System::Changes::Geometry::Scale |
			System::Changes::Geometry::All |
			System::Changes::Input::Mouse_Moved |
			System::Changes::AI::Behavior;
	};

	inline const std::string& GetName( ) { return _name; };
	inline SystemType GetType( ) { return RenderSystemType; };

	virtual void Update( float deltaMilliseconds );

	inline MathVector3 GetPosition( ) { return MathVector3( _sceneNode->getPosition( ) ); };
	inline MathVector3 GetScale( ) { return MathVector3( _sceneNode->getScale( ) ); };
	inline MathQuaternion GetOrientation( ) { return MathQuaternion( _sceneNode->getOrientation( ) ); };

protected:

	void InitializeSceneNode( Ogre::SceneNode* sceneNode );
	void DestroySceneNode( Ogre::SceneNode* sceneNode );

	std::string _name;
	RendererSystemScene* _scene;
	Ogre::SceneNode* _sceneNode;

	AnimationBlenderList _animationBlenders;

private:

	RendererSystemComponent( ) { };
	RendererSystemComponent( const RendererSystemComponent & copy ) { };
	RendererSystemComponent & operator = ( const RendererSystemComponent & copy ) { return *this; };

};

#endif