#ifndef __ISERVICE_H
#define __ISERVICE_H

#include <vector>

#include "SystemType.hpp"
#include "SystemProperty.hpp"

class IService
{

public:

	typedef std::vector< IService* > ServiceList;

	virtual SystemType GetType( ) = 0;
	virtual AnyValue::AnyValueMap Execute( const std::string& actionName, AnyValueMap parameters ) = 0;

};

#endif