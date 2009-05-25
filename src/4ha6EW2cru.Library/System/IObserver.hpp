/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IObserver.hpp
*  @date   2009/04/25
*/
#ifndef IOBSERVER_H
#define IOBSERVER_H

#include "AnyType.hpp"

#include "../System/SystemType.hpp"

#include <deque>

/*! 
 *  Observes a change from an ISubject
 */
class IObserver
{

public:

	typedef std::multimap< System::Message, IObserver* > ObserverMap;
	typedef std::deque< IObserver* > ObserverList;

	/*! Messages the Component to influence its internal state
	*
	*  @param[in] const std::string & message
	*  @return (AnyType)
	*/
	virtual AnyType Message( const System::Message& message, AnyType::AnyTypeMap parameters ) = 0;

};

#endif