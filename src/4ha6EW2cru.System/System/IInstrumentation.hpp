/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IInstrumentation.hpp
*  @date   2009/07/14
*/
#pragma once
#ifndef IINSTRUMENTATION_HPP
#define IINSTRUMENTATION_HPP

namespace 
{
	/*! 
	 *  The Instrumentation Provider of Statistics about the Game
	 */
	class IInstrumentation
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
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


		/*! Sets the name of the active level
		*
		* @param[in] const std::string & levelName
		* @return ( void )
		*/
		virtual void SetLevelName( const std::string& levelName ) = 0;


		/*! Gets the name of the active level
		*
		* @return ( std::string )
		*/
		virtual std::string GetLevelName( ) const = 0;
		
	};
};

#endif