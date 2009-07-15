/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   Instrumentation.hpp
*  @date   2009/07/07
*/
#pragma once
#ifndef INSTRUMENTATION_HPP
#define INSTRUMENTATION_HPP

/*! 
 *  A provider for performance statistics
 */
class Instrumentation : public IInstrumentation
{

	typedef std::map< System::Queues::Queue, float > RoundTimeList;

public:

	/*! Default Destructor
	 *
	 *  @return ()
	 */
	~Instrumentation( ) { };


	/*! Default Constructor
	*
	* @return (  )
	*/
	Instrumentation( ) { };


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


	/*! Sets the name of the active level
	*
	* @param[in] const std::string & levelName
	* @return ( void )
	*/
	inline void SetLevelName( const std::string& levelName ) { m_levelName = levelName; };


	/*! Gets the name of the active level
	*
	* @return ( std::string )
	*/
	inline std::string GetLevelName( ) const { return m_levelName; }

private:

	Instrumentation( const Instrumentation & copy ) { };
	Instrumentation & operator = ( const Instrumentation & copy ) { return *this; };

	int m_fps;
	RoundTimeList m_roundTimes;
	std::string m_levelName;
};

#endif