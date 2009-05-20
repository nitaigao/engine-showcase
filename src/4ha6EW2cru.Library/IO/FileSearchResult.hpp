#ifndef FILESEARCHRESULT_H
#define FILESEARCHRESULT_H

#include <deque>

namespace IO
{
	struct FileSearchResult
	{

	public:

		typedef std::deque< FileSearchResult > FileSearchResultList;

		std::string FileName;
		std::string FilePath;

	};
};

#endif
