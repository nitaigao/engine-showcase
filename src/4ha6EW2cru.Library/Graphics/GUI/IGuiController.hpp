#ifndef IGUICONTROLLER_H
#define IGUICONTROLLER_H

class IGuiController
{

public:

	virtual ~IGuiController( ) { };

	virtual void Initialize( ) = 0;

};

#endif