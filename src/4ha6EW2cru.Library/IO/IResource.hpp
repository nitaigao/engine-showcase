#ifndef IRESOURCE_H
#define IRESOURCE_H

#include "FileBuffer.hpp"

namespace Resources
{
	/*! 
	*  A Game Resource
	*/
	class IResource
	{

	public:

		typedef std::map< std::string, IResource* > ResourceMap;

		/*! Default Destructor
		*
		*  @return ()
		*/
		virtual ~IResource( ) { };


		/*! Returns the file data of the Resource
		*
		*  @return (FileBuffer*)
		*/
		virtual IO::FileBuffer* GetFileBuffer( ) const = 0;


		/*! Increments the reference count for this resource 
		*   If it hit's 0 the resource will be queued for deletion
		*
		*  @return (void)
		*/
		virtual void AddReference( ) = 0;


		/*! De increments the reference count for this resource 
		 *  If it hit's 0 the resource will be queued for deletion
		 *
		 *  @return (void)
		 */
		virtual void RemoveReference( ) = 0;

	};
};

#endif