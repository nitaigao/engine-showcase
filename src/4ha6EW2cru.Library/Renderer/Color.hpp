/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   Color.hpp
*  @date   2009/04/26
*/
#ifndef __COLOR_H
#define __COLOR_H

namespace Renderer
{
	/*! 
	 *  Internal Representation of a Color
	 */
	class Color
	{

	public:

		float Red, Green, Blue;

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~Color( ) { };


		/*! Default Constructor
		 *
		 *  @param[in] const float & red
		 *  @param[in] const float & green
		 *  @param[in] const float & blue
		 *  @return ()
		 */
		Color( const float& red, const float& green, const float& blue )
			: Red( red )
			, Green( green )
			, Blue( blue )
		{

		}


		/*! Default Copy Constructor
		 *
		 *  @param[in] const Color & copy
		 *  @return ()
		 */
		Color( const Color & copy ) { };

	private:
		
		Color & operator = ( const Color & copy ) { return *this; };

	};
};


#endif