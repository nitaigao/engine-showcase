#ifndef __IGUICOMPONENT_H
#define __IGUICOMPONENT_H

class IGuiComponent
{

public:

	virtual ~IGuiComponent( ) { };

	virtual void Initialize( ) = 0;

};

#endif