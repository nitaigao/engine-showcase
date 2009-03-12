#ifndef __ISYSTEMENTITY_H
#define __ISYSTEMENTITY_H

#include "SystemType.hpp"
#include "SystemProperty.hpp"

#include "IObserver.hpp"

class ISystemComponent : public IObserver, public ISubject
{

public:

	virtual ~ISystemComponent( ) { };

	virtual void Initialize( SystemPropertyList properties ) = 0;
	virtual void AddObserver( IObserver* observer ) = 0;

	virtual const std::string& GetName( ) = 0;
	virtual SystemType GetType( ) = 0;

};

typedef std::vector< ISystemComponent* > SystemComponentList;


#endif