#ifndef __RESOURCE_H
#define __RESOURCE_H

#include "IResource.hpp"

namespace Resource
{
	class Resource : public IResource
	{

	public:

		virtual ~Resource( )
		{
			delete _fileBuffer;
		};

		Resource( FileBuffer* fileBuffer )
			: _fileBuffer( fileBuffer )
		{

		};

		inline FileBuffer* GetFileBuffer( ) const { return _fileBuffer; };

	private:

		FileBuffer* _fileBuffer;

	};
}

#endif