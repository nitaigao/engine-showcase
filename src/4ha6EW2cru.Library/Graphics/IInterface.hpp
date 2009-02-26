#ifndef __IINTERFACE_H
#define __IINTERFACE_H

class IInterface
{

public:

	virtual ~IInterface( ) { };

	virtual void Initialize( ) = 0;
	virtual void Update( const float deltaMilliseconds ) const = 0;

};

#endif