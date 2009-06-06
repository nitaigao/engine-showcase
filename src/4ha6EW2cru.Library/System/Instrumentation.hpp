#pragma once
#ifndef INSTRUMENTATION_HPP
#define INSTRUMENTATION_HPP

#include "IInstrumentation.hpp"

class Instrumentation : public IInstrumentation
{

	typedef std::map< System::Queues::Queue, float > RoundTimeList;

public:

	/*! Gets the Frames Per Second Statistic
	*
	* @return ( int )
	*/
	inline int GetFPS( ) const { return m_fps; };


	/*! Sets the Frames Per Second Statistic
	*
	* @return ( void )
	*/
	inline void SetFPS( const int& fps ) { m_fps = fps; };


	/*! Gets the Round Time of the Given Queue
	*
	* @param[in] const System::Queues::Queue & queue
	* @return ( float )
	*/
	inline float GetRoundTime( const System::Queues::Queue& queue ) { return m_roundTimes[ queue ]; };


	/*! Sets the Round Time of the Given Queue
	*
	* @param[in] const System::Queues::Queue & queue
	* @param[in] const float & roundTime
	* @return ( void )
	*/
	inline void SetRoundTime( const System::Queues::Queue& queue, const float& roundTime ) { m_roundTimes[ queue ] = roundTime; };

private:

	int m_fps;
	RoundTimeList m_roundTimes;

};

#endif