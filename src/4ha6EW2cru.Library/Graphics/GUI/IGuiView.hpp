#ifndef __IGUIVIEW_H
#define __IGUIVIEW_H

class IGuiView
{

public:

	virtual ~IGuiView( ) { };

	virtual void Initialize( ) = 0;

};

#endif