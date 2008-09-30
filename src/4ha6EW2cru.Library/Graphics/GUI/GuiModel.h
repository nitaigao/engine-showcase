#ifndef __GUIMODEL_H
#define __GUIMODEL_H

#include "IGuiModel.hpp"

#include <MyGUI.h>
#include "../../Scripting/Script.h"
#include "../../Events/EventType.hpp"

#include <luabind/luabind.hpp>
using namespace luabind;

class GuiModel : public IGuiModel
{

public:

	virtual ~GuiModel( ) { };

};

struct GuiModel_Wrapper : GuiModel, luabind::wrap_base
{
	virtual ~GuiModel_Wrapper( ) { };

	GuiModel_Wrapper( )
	{

	}

};


#endif