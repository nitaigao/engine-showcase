/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   Resource.h
*  @date   2009/04/26
*/
#ifndef __RESOURCE_H
#define __RESOURCE_H

#include "IResource.hpp"

namespace Resources
{
	/*! 
	 *  A Game Resource
	 */
	class Resource : public IResource
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~Resource( )
		{
			delete _fileBuffer;
		};


		/*! Default Constructor
		 *
		 *  @param[in] FileBuffer * fileBuffer
		 *  @return ()
		 */
		Resource( IO::FileBuffer* fileBuffer )
			: _fileBuffer( fileBuffer )
		{

		};


		/*! Returns the file data of the Resource
		 *
		 *  @return (FileBuffer*)
		 */
		inline IO::FileBuffer* GetFileBuffer( ) const { return _fileBuffer; };

	private:

		IO::FileBuffer* _fileBuffer;

	};
}

#endif