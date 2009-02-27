#ifndef __IINPUTSYSTEM_H
#define __IINPUTSYSTEM_H

class IInputSystem
{

public:

	virtual ~IInputSystem( ) { };

	/* Initializes the Input System */
	virtual void Initialize( ) = 0;

	/* Performs a capture loop on all Input Devices */
	virtual void Update( ) const = 0;

};

#endif
