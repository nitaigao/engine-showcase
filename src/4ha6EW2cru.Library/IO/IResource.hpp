#ifndef __IRESOURCE_H
#define __IRESOURCE_H

#include "FileBuffer.hpp"

namespace Resources
{
	/*! 
	*  A Game Resource
	*/
	class IResource
	{

	public:

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

	};
};

#endif