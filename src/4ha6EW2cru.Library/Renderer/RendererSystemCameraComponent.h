#ifndef __RENDERERSYSTEMCAMERACOMPONENT_H
#define __RENDERERSYSTEMCAMERACOMPONENT_H

#include "RendererSystemComponent.h"

class RendererSystemCameraComponent : public RendererSystemComponent
{

	typedef std::deque< float > History;

public:

	virtual ~RendererSystemCameraComponent( ) { };

	RendererSystemCameraComponent( const std::string& name, RendererSystemScene* scene )
		: RendererSystemComponent( name, scene )
		, _observer( 0 )
		, _historySize( 10 )
		, _weightModifier( 0.01f ) 
	{
	
	}

	void Initialize( AnyValueMap properties );
	void Update( float deltaMilliseconds );

	void AddObserver( IObserver* observer ) { _observer = observer; };
	void Observe( ISubject* subject, unsigned int systemChanges );
	void PushChanges( unsigned int systemChanges );

private:

	float AverageInputHistory( const History& inputHistory, const float& weightModifier );

	IObserver* _observer;
	History _xHistory;
	History _yHistory;

	float _weightModifier;
	int _historySize;
};

#endif