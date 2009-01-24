#ifndef __FILESEARCHRESULT_H
#define __FILESEARCHRESULT_H

#include <vector>

struct FileSearchResult 
{

public:

	std::string FileName;
	std::string FilePath;

};

typedef std::vector< FileSearchResult > FileSearchResultList;

#endif