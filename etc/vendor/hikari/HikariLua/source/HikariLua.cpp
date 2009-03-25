#include "stdafx.h"
#include <updatable.h>
#include <Hikari.h>
#include <HikariLua.h>
#include "luna.h"
#include <Ogre.h>

#include <winnls.h>

static Hikari::Arguments lua2arg(lua_State*, int);
static lua_State *g_pLua = NULL;
static Ogre::Viewport *g_viewport = NULL;
static Hikari::HikariManager *g_hikariMgr = NULL;

class LuaHikariUpdate : public CUpdatable
{
public:
	void Update(float fTime);
};

void LuaHikariUpdate::Update(float fTime)
{
	g_hikariMgr->update();
}


class LuaFlashControl;

class LuaDispatcher
{
public:
	LuaDispatcher(LuaFlashControl *pParent, const char *functionName) 
	{
		flashFunction = functionName;
		m_pParent = pParent;
	}
	~LuaDispatcher() {}

	Hikari::FlashValue ASEntryPoint(Hikari::FlashControl* caller, const Hikari::Arguments& args);
private:
	std::string flashFunction;
	LuaFlashControl *m_pParent;
};


class LuaFlashControl
{
public:
	LuaFlashControl(lua_State *L);
	~LuaFlashControl();

	// Luna Required static information
	static const char className[];
	static Luna<LuaFlashControl>::RegType methods[];

	int lua_load(lua_State *L); //const Ogre::String& movieFilename);
	int luaPlay(lua_State *L); //();
	int luaStop(lua_State *L); //();
	int luaRewind(lua_State *L); //();
	int luaSetLoop(lua_State *L); //(bool shouldLoop);
	int luaSetTransparent(lua_State *L); //(bool isTransparent, bool useAlphaHack = false);
	int luaSetQuality(lua_State *L); //(short renderQuality);
	int luaSetDraggable(lua_State *L); //(bool isDraggable);
	int luaSetIgnoreTransparentPixels(lua_State *L); //(bool shouldIgnore, Ogre::Real threshold = 0.04);

	int lua_hide(lua_State *L); //();
	int lua_show(lua_State *L); //();
	int lua_getVisibility(lua_State *L); //() const; return bool
	int lua_setOpacity(lua_State *L); //(Ogre::Real opacity);
	int lua_focus(lua_State *L); //();
	int lua_move(lua_State *L); //(int deltaX, int deltaY);


	int lua_bind(lua_State *L); //(const Ogre::DisplayString& funcName, const FlashDelegate& callback);
	int lua_unbind(lua_State *L); //(const Ogre::DisplayString& funcName);
	int lua_callFunction(lua_State *L); //(Ogre::DisplayString funcName, const Arguments& args = Args()); return FlashValue

	int lua_getExtents(lua_State *L); //(unsigned short &width, unsigned short &height) const;
	int lua_getUVScale(lua_State *L); //(Ogre::Real &uScale, Ogre::Real &vScale) const;


	int lua_createFlashOverlay(lua_State *L);	//(const char *name, int height, int width, zorder)

	std::map<std::string,LuaDispatcher *> m_mapDispatchers;

protected:
	Hikari::FlashControl *m_pControl;
};

// Define the Lua ClassName
const char LuaFlashControl::className[] = "FlashControl";

// Define the methods we will expose to Lua
// Check luaobject.h for the definitions...
#define method(class, name, luaName) {#name, &class::name, #luaName}
Luna<LuaFlashControl>::RegType LuaFlashControl::methods[] = {
   method(LuaFlashControl, lua_load, load),
   method(LuaFlashControl, luaPlay, play),
   method(LuaFlashControl, luaStop, stop),
   method(LuaFlashControl, luaRewind, rewind),
   method(LuaFlashControl, luaSetLoop, setLoop),
   method(LuaFlashControl, luaSetTransparent, setTransparent),
   method(LuaFlashControl, luaSetQuality, setQuality),
   method(LuaFlashControl, luaSetDraggable, setDraggable),
   method(LuaFlashControl, luaSetIgnoreTransparentPixels, setIgnoreTransparentPixels),
   method(LuaFlashControl, lua_bind, bind),
   method(LuaFlashControl, lua_unbind, unbind),
   method(LuaFlashControl, lua_callFunction, callFunction),
   method(LuaFlashControl, lua_hide, hide),
   method(LuaFlashControl, lua_show, show),
   method(LuaFlashControl, lua_getVisibility, getVisibility),
   method(LuaFlashControl, lua_setOpacity, setOpacity),
   method(LuaFlashControl, lua_focus, focus),
   method(LuaFlashControl, lua_move, move),
   method(LuaFlashControl, lua_getExtents, getExtents),
   method(LuaFlashControl, lua_getUVScale, getUVScale),
   method(LuaFlashControl, lua_createFlashOverlay, init),
   {0,0}
};

LuaFlashControl::LuaFlashControl(lua_State *L)
{
	m_pControl = NULL;
}

int LuaFlashControl::lua_createFlashOverlay(lua_State *L)
{
	int argNum =1;
	const char *pName = luaL_checkstring(L, argNum++);
	int w = luaL_checkint(L, argNum++);
	int h = luaL_checkint(L, argNum++);
	std::string sPosition = luaL_checkstring(L, argNum++);
	int zOrder = luaL_optint(L, argNum++, 0);

	Hikari::Position position(Hikari::Center);

	if(sPosition == "Left")
		position = Hikari::Left;

	if(sPosition == "TopLeft")
		position = Hikari::TopLeft;
	
	if(sPosition == "TopCenter")
		position = Hikari::TopCenter;
	
	if(sPosition == "TopRight")
		position = Hikari::TopRight;
	
	if(sPosition == "Right")
		position = Hikari::Right;
	
	if(sPosition == "BottomRight")
		position = Hikari::BottomRight;
	
	if(sPosition == "BottomCenter")
		position = Hikari::BottomCenter;
	
	if(sPosition == "BottomLeft")
		position = Hikari::BottomLeft;
		

	m_pControl = Hikari::HikariManager::Get().createFlashOverlay(pName, g_viewport, w, h, position, zOrder);

	return 0;
}

LuaFlashControl::~LuaFlashControl()
{
	if(m_pControl)
	{
		Hikari::HikariManager::Get().destroyFlashControl(m_pControl);
	}
	// delete any flash dispatch pointers

}

int LuaFlashControl::lua_load(lua_State *L) //const Ogre::String& movieFilename);
{
	m_pControl->load(luaL_checkstring(L, 1));
	return 0;
}


int LuaFlashControl::luaPlay(lua_State *L)//();
{
	m_pControl->play();
	return 0;
}

int LuaFlashControl::luaStop(lua_State *L)  //();
{
	m_pControl->stop();
	return 0;
}

int LuaFlashControl::luaRewind(lua_State *L)  //();
{
	m_pControl->rewind();
	return 0;
}

int LuaFlashControl::luaSetLoop(lua_State *L)  //(bool shouldLoop);
{
	int argNum = 1;
	bool shouldLoop = false;
	if(lua_type(L, argNum) == LUA_TBOOLEAN)
		shouldLoop = lua_toboolean(L, argNum) != 0;
	argNum++;

	m_pControl->setLoop(shouldLoop);
	return 0;
}

int LuaFlashControl::luaSetTransparent(lua_State *L)  //(bool isTransparent, bool useAlphaHack = false);
{
	int argNum = 1;
	bool isTransparent = false;
	if(lua_type(L, argNum) == LUA_TBOOLEAN)
		isTransparent = lua_toboolean(L, argNum) != 0;
	argNum++;

	bool useAlphaHack = false;
	if(lua_type(L, argNum) == LUA_TBOOLEAN)
		useAlphaHack = lua_toboolean(L, argNum) != 0;
	argNum++;

	m_pControl->setTransparent(isTransparent, useAlphaHack);
	return 0;
}

int LuaFlashControl::luaSetQuality(lua_State *L)  //(short renderQuality);
{
	m_pControl->setQuality((short) luaL_checkint(L, 1));
	return 0;
}

int LuaFlashControl::luaSetDraggable(lua_State *L)  //(bool isDraggable);
{
	int argNum = 1;
	bool isDraggable = false;
	if(lua_type(L, argNum) == LUA_TBOOLEAN)
		isDraggable = lua_toboolean(L, argNum) != 0;
	argNum++;

	m_pControl->setDraggable(isDraggable);
	return 0;
}

int LuaFlashControl::luaSetIgnoreTransparentPixels(lua_State *L)  //(bool shouldIgnore, Ogre::Real threshold = 0.04);
{
	int argNum = 1;
	bool shouldIgnore = false;
	if(lua_type(L, argNum) == LUA_TBOOLEAN)
		shouldIgnore = lua_toboolean(L, argNum) != 0;
	argNum++;

	Ogre::Real threshold = luaL_optnumber(L, argNum++, 0.04);

	m_pControl->setIgnoreTransparentPixels(shouldIgnore, threshold);
	return 0;
}


int LuaFlashControl::lua_hide(lua_State *L)  //();
{
	m_pControl->hide();
	return 0;
}

int LuaFlashControl::lua_show(lua_State *L)  //();
{
	m_pControl->show();
	return 0;
}

int LuaFlashControl::lua_getVisibility(lua_State *L)  //() const; return bool
{
	bool r = m_pControl->getVisibility();
	lua_pushboolean(L, r);
	return 1;
}

int LuaFlashControl::lua_setOpacity(lua_State *L)  //(Ogre::Real opacity);
{
	m_pControl->setOpacity(luaL_checknumber(L, 1)/100.0);
	return 0;
}

int LuaFlashControl::lua_focus(lua_State *L)  //();
{
	m_pControl->focus();
	return 0;
}

int LuaFlashControl::lua_move(lua_State *L)  //(int deltaX, int deltaY);
{
	m_pControl->move(luaL_checkint(L, 1), luaL_checkint(L, 2));
	return 0;
}

int LuaFlashControl::lua_bind(lua_State *L)  //(const Ogre::DisplayString& funcName, const FlashDelegate& callback);
{
	const char *pFuncName = luaL_checkstring(L, 1);
	const char *pLuaFuncName = luaL_checkstring(L, 2);
	LuaDispatcher *pDispatch = new LuaDispatcher(this, pLuaFuncName);
	m_mapDispatchers[pFuncName] = pDispatch;
	m_pControl->bind(pFuncName, Hikari::FlashDelegate(pDispatch, &LuaDispatcher::ASEntryPoint));
	return 0;
}

int LuaFlashControl::lua_unbind(lua_State *L)  //(const Ogre::DisplayString& funcName);
{
	const char *funcName = luaL_checkstring(L, 1);
	m_pControl->unbind(funcName);
	std::map<std::string,LuaDispatcher *>::iterator it = m_mapDispatchers.find(funcName);
	if(it != m_mapDispatchers.end())
	{
		delete (*it).second;
		m_mapDispatchers.erase(it);
	}
	return 0;
}

int LuaFlashControl::lua_callFunction(lua_State *L)  //(Ogre::DisplayString funcName, const Arguments& args = Args()); return FlashValue
{
	const char *funcName = luaL_checkstring(L, 1);

	// get all lua arguments and convert to "args"
	Hikari::Arguments args = lua2arg(L, 2);

	Ogre::DisplayString wideName(funcName);
	Hikari::FlashValue v = m_pControl->callFunction(wideName, args);

	lua_pushinteger(L, (int) v.getNumber());
	return 1;
}

int LuaFlashControl::lua_getExtents(lua_State *L)  //(unsigned short &width, unsigned short &height) const;
{
	// gonna return w, h instead of by reference
	unsigned short w, h;
	m_pControl->getExtents(w, h);
	lua_pushinteger(L, w);
	lua_pushinteger(L, h);

	return 2;
}

int LuaFlashControl::lua_getUVScale(lua_State *L)  //(Ogre::Real &uScale, Ogre::Real &vScale) const;
{
	Ogre::Real u, v;
	m_pControl->getUVScale(u, v);
	lua_pushnumber(L, u);
	lua_pushnumber(L, v);
	return 2;
}


////////////////////////////

static Hikari::Arguments lua2arg(lua_State *L, int argNum)
{
	Hikari::Arguments args;
	Hikari::FlashValue v;

	while(lua_type(L, argNum) != LUA_TNONE)
	{
		int t = lua_type(L, argNum);
		switch(t)
		{
		case LUA_TNIL:
			v = Hikari::FlashValue();
			break;
		case LUA_TBOOLEAN:
			v = lua_toboolean(L, argNum);
			break;
		case LUA_TNUMBER:
			v = (Ogre::Real)(luaL_checknumber(L, argNum));
			break;
		case LUA_TSTRING:
			v = luaL_checkstring(L, argNum);
			break;

		case LUA_TTABLE:
			// ???
			break;
		case LUA_TLIGHTUSERDATA:	// assume this is a LuaFlashControl type
			break;

		}
		args.push_back(v);
		argNum++;
	}
	return args;
}

Hikari::FlashValue LuaDispatcher::ASEntryPoint(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	lua_State *L = g_pLua;

	// send your stuff to a Lua func that looks like this:
	//		function luaHandler(nameOfFlashControl, ...)

	/* push functions and arguments */
	lua_getglobal(L, this->flashFunction.c_str());  /* function to be called */


	// name of the FlashControl, need to find the actual lightuserdata for the container
//	lua_pushstring(L, caller->getName().c_str());
	Luna<LuaFlashControl>::userdataType *ud = static_cast<Luna<LuaFlashControl>::userdataType*>(lua_newuserdata(L, sizeof(Luna<LuaFlashControl>::userdataType)));
    ud->pT = m_pParent;  // store pointer to object in userdata
	luaL_getmetatable(L, LuaFlashControl::className);  // lookup metatable in Lua registry
    lua_setmetatable(L, -2);

	Hikari::Arguments::const_iterator it = args.begin();
	while(it != args.end())
	{
		switch((*it).getType())
		{
		case Hikari::FT_BOOLEAN:
			lua_pushboolean(L, (*it).getBool());   /* push argument */
			break;
		case Hikari::FT_NUMBER:
			lua_pushnumber(L, (*it).getNumber());   /* push argument */
			break;
		case Hikari::FT_STRING:
			lua_pushstring(L, (*it).getString().asUTF8_c_str());   /* push argument */
			break;
		case Hikari::FT_NULL:
			lua_pushnil(L);
			break;
		}
		++it;
	}
	/* do the call (size() arguments, numRets results) */
	if (lua_pcall(L, (int) args.size()+1, 1, 0) != 0)
	{
		//error
		std::string error = "Lua Error - Function Call:";
		error += luaL_checkstring(L, -1);
		throw(error.c_str());
	}
	// pull the return value
	Hikari::FlashValue v;
	switch(lua_type(L, -1))
	{
		case LUA_TNIL:
			v = Hikari::FlashValue();
			break;
		case LUA_TBOOLEAN:
			v = lua_toboolean(L, -1);
			break;
		case LUA_TNUMBER:
			v = (Ogre::Real)(luaL_checknumber(L, -1));
			break;
		case LUA_TSTRING:
			v = luaL_checkstring(L, -1);
			break;
	}
	ud->pT = NULL;

	return v;
}


lua_State *initHikariLua(lua_State *pLua, Ogre::Viewport *pView, const char *pPath)
{
	g_viewport = pView;
	if(!pLua)
	{
		pLua = lua_open();
		luaL_openlibs(pLua);
	}
	g_pLua = pLua;

	g_hikariMgr = new Hikari::HikariManager(pPath);
	new LuaHikariUpdate();

	Luna<LuaFlashControl>::Register(g_pLua);
	return g_pLua;
}

bool RunLuaScript(const char *pFilename)
{
	if (0 != luaL_loadfile(g_pLua, pFilename))
	{
		throw(luaL_checkstring(g_pLua, -1));
		return false;
	}
	if (0 != lua_pcall(g_pLua, 0, LUA_MULTRET, 0))
	{
		throw(luaL_checkstring(g_pLua, -1));
		return false;
	}

	return true;
}
