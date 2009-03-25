struct lua_State;

lua_State *initHikariLua(lua_State *pLua, Ogre::Viewport *pView, const char *pPath);
bool RunLuaScript(const char *pFilename);