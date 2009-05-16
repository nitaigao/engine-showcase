/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IBehavior.hpp
*  @date   2009/04/27
*/
#ifndef IBEHAVIOR_HPP
#define IBEHAVIOR_HPP

namespace AI
{
	/*!
	 *  An AI Behavior State 
	 */
	class IBehaviour
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IBehaviour( ) { };


		/*! Sets the Behavior
		*
		*  @param[in] const std::string & behavior
		*  @return (void)
		*/
		virtual void SetBehavior( const std::string& behavior ) = 0;


		/*! Gets the Behavior Name
		*
		*  @return (std::string)
		*/
		virtual std::string GetBehavior( ) const = 0;

	};
};

#endif