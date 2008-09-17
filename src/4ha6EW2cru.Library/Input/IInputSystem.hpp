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

	/*! Resets the input capture area of the Input System */
	virtual void SetCaptureArea( int width, int height ) = 0;
};

#endif