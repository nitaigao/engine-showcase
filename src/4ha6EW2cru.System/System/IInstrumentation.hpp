#pragma once
#ifndef IINSTRUMENTATION_HPP
#define IINSTRUMENTATION_HPP

class IInstrumentation
{

public:

	virtual ~IInstrumentation( ) { };


	/*! Gets the Frames Per Second Statistic
	*
	* @return ( int )
	*/
	virtual int GetFPS( ) const = 0;


	/*! Sets the Frames Per Second Statistic
	*
	* @return ( void )
	*/
	virtual void SetFPS( const int& fps ) = 0;


	/*! Gets the Round Time of the Given Queue
	*
	* @param[in] const System::Queues::Queue & queue
	* @return ( float )
	*/
	virtual float GetRoundTime( const System::Queues::Queue& queue ) = 0;


	/*! Sets the Round Time of the Given Queue
	*
	* @param[in] const System::Queues::Queue & queue
	* @param[in] const float & roundTime
	* @return ( void )
	*/
	virtual void SetRoundTime( const System::Queues::Queue& queue, const float& roundTime ) = 0;
};

#endif