
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include "Ogre.h" 

void QuitProgram(void);
void SetViewportColor(Ogre::ColourValue c);

extern "C" int _QuitProgram(lua_State *L)
{
	QuitProgram();
	return 0;
}

extern "C" int _SetViewportColor(lua_State *L)
{
	Ogre::Real numValue = luaL_checknumber(L, 1);
	Ogre::ColourValue result;
	result.b = ((int)numValue % 256) / 255.0f;
	result.g = (((int)numValue / 256) % 256) / 255.0f;
	result.r = (((int)numValue / 256 / 256) % 256) / 255.0f;

	SetViewportColor(result);
	return 0;
}

void initLuaGlue(lua_State *pLua)
{
	lua_register(pLua, "QuitProgram", _QuitProgram);
	lua_register(pLua, "SetViewportColor", _SetViewportColor);
}