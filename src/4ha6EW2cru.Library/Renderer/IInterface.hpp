#ifndef __IINTERFACE_H
#define __IINTERFACE_H

#include <Ogre.h>

class IInterface
{

public:

	virtual ~IInterface( ) { };

	virtual void Initialize( ) = 0;
	virtual void Update( const float deltaMilliseconds ) = 0;

	virtual void ResetWidgetPositions( ) = 0;

};

#endif