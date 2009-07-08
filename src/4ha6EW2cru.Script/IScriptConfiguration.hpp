/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IScriptConfiguration.hpp
*  @date   2009/04/27
*/
#ifndef ISCRIPTCONFIGURATION_HPP
#define ISCRIPTCONFIGURATION_HPP

namespace Script
{
	/*!
	 *  A Configuration Object used to obtain game data from within a Script 
	 */
	class IScriptConfiguration
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IScriptConfiguration( ) { };


		/*! Returns whether or not the Game is in FullScreen
		 *
		 *  @return (bool)
		 */
		virtual bool IsFullScreen( ) const  = 0;


		/*! Sets whether the game is in Full Screen
		 *
		 *  @param[in] bool isFullScreen
		 *  @return (void)
		 */
		virtual void SetFullScreen( bool isFullScreen ) = 0;


		/*! Returns the Game windows Width
		 *
		 *  @return (int)
		 */
		virtual int GetDisplayWidth ( ) const = 0;


		/*! Sets the Game window Width
		 *
		 *  @param[in] int width
		 *  @return (void)
		 */
		virtual void SetDisplayWidth( int width )  = 0;


		/*! Returns the Game window Height
		 *
		 *  @return (int)
		 */
		virtual int GetDisplayHeight ( ) const = 0;


		/*! Sets the Game window Height
		 *
		 *  @param[in] int height
		 *  @return (void)
		 */
		virtual void SetDisplayHeight( int height ) = 0;


		/*! Returns the Game window Color Depth
		 *
		 *  @return (int)
		 */
		virtual int GetColorDepth( ) const = 0;


		/*! Returns whether the Developer Console is Enabled
		 *
		 *  @return (bool)
		 */
		virtual bool IsConsole( ) const = 0;


		/*! Sets whether the Developer Console is Enabled
		 *
		 *  @param[in] bool isConsole
		 *  @return (void)
		 */
		virtual void SetConsole( bool isConsole ) = 0;

	};
};

#endif