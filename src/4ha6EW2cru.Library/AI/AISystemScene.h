#ifndef __AISYSTEMSCENE_H
#define __AISYSTEMSCENE_H

#include "../Scripting/ScriptSystemScene.h"

class AISystemScene : public ISystemScene
{

public:

	virtual ~AISystemScene( );

	AISystemScene( );

	void Initialize( );
	void Update( float deltaMilliseconds );

	inline SystemType GetType( ) { return AISystemType; };

	ISystemComponent* CreateComponent( const std::string& name, const std::string& type );
	void DestroyComponent( ISystemComponent* component );

	inline ISystemScene* GetScriptScene( ) { return _scriptScene; };

private:

	ScriptSystemScene* _scriptScene;
	SystemComponentList _components;

};

#endif