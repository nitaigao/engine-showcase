/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IWorldSerializer.hpp
*  @date   2009/04/27
*/
#ifndef __IWORLDLOADER_HPP
#define __IWORLDLOADER_HPP

namespace Serialization
{
	/*!
	 *  De serializes a world from storage 
	 */
	class IWorldSerializer
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IWorldSerializer( ) { };


		/*! Loads a Serialized world from the File System
		 *
		 *  @param[in] const std::string & levelPath
		 *  @return (void)
		 */
		virtual void Load( const std::string& levelPath ) = 0;


		/*! Steps the loading process
		 *
		 *  @param[in] float deltaMilliseconds
		 *  @return (void)
		 */
		virtual void Update( const float& deltaMilliseconds ) = 0;

	};
};

#endif