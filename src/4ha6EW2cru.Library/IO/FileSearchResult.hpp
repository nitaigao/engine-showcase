#ifndef FILESEARCHRESULT_H
#define FILESEARCHRESULT_H

#include <vector>

namespace IO
{
	struct FileSearchResult
	{

	public:

		typedef std::vector< FileSearchResult > FileSearchResultList;

		std::string FileName;
		std::string FilePath;

	};
};

#endif
