#ifndef __IRESOURCE_H
#define __IRESOURCE_H

#include <string>
#include "FileBuffer.hpp"

namespace Resource
{
	class IResource
	{

	public:

		virtual ~IResource( ) { };
		virtual FileBuffer* GetFileBuffer( ) const = 0;

	};
};

#endif