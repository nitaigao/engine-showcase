/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IResourceManager.hpp
*  @date   2009/04/26
*/
#ifndef IRESOURCECACHE_H
#define IRESOURCECACHE_H

#include "IResource.hpp"

namespace Resources
{
	/*! 
	 *  A Cache of data retrieved from the FileSystem for fast access
	 */
	class IResourceCache
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IResourceCache( ) { };


		/*! Returns a Resource from the given file path
		 *
		 *  @param[in] const std::string & filePath
		 *  @return (Resources::IResource*)
		 */
		virtual Resources::IResource* GetResource( const std::string& filePath ) = 0;


		/*! Steps the internal data of the cache
		 *
		 *  @param[in] float deltaMilliseconds
		 *  @return (void)
		 */
		virtual void Update( const float& deltaMilliseconds ) = 0;

	};
};

#endif