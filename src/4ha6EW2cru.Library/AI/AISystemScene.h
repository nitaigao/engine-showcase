#ifndef __AISYSTEMSCENE_H
#define __AISYSTEMSCENE_H

#include "../Scripting/ScriptSystemScene.h"

class AISystemScene : public ISystemScene
{

public:

	virtual ~AISystemScene( );

	AISystemScene( );

	ISystemComponent* CreateComponent( const std::string& name, const std::string& type );
	inline void DestroyComponent( ISystemComponent* component );
	inline SystemType GetType( ) { return AISystemType; };
	inline void Update( float deltaMilliseconds );

	void Initialize( );
	inline ISystemScene* GetScriptScene( ) { return _scriptScene; };

private:

	ScriptSystemScene* _scriptScene;
	SystemComponentList _components;

};

#endif