#ifndef __EVENTAPPENDER_H
#define __EVENTAPPENDER_H

#include "IAppender.hpp"

class EventAppender : public IAppender
{

public:

	void Append( const std::string message ) const;

};

#endif