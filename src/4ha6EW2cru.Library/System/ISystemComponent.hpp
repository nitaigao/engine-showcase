#ifndef __ISYSTEMENTITY_H
#define __ISYSTEMENTITY_H

#include <vector>

#include "SystemType.hpp"
#include "SystemProperty.hpp"

#include "IObserver.hpp"

class ISystemComponent : public IObserver, public ISubject
{

protected:

	typedef std::map< std::string, AnyValue > AnyValueMap;

public:

	virtual ~ISystemComponent( ) { };

	virtual void Initialize( AnyValueMap properties ) = 0;
	virtual void AddObserver( IObserver* observer ) = 0;

	virtual const std::string& GetName( ) = 0;
	virtual SystemType GetType( ) = 0;

};

typedef std::vector< ISystemComponent* > SystemComponentList;


#endif