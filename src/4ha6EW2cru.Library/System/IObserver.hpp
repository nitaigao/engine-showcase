/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IObserver.hpp
*  @date   2009/04/25
*/
#ifndef IOBSERVER_H
#define IOBSERVER_H

#include "AnyValue.hpp"

#include "ISubject.hpp"

#include <vector>

/*! 
 *  Observes a change from an ISubject
 */
class IObserver
{

public:

	/*! Gets the Changes the Observer is interested in
	 *
	 *  @return (unsigned int)
	 */
	virtual unsigned int GetRequestedChanges( ) const = 0;


	/*! Observes the changes when the ISubject notifies
	 *
	 *  @param[in] ISubject * subject
	 *  @param[in] const unsigned int& systemChanges
	 *  @return (void)
	 */
	virtual void Observe( ISubject* subject, const unsigned int& systemChanges ) = 0;


	/*! Messages the Component to influence its internal state
	*
	*  @param[in] const std::string & message
	*  @return (AnyValue)
	*/
	virtual AnyValue Message( const std::string& message, AnyValue::AnyValueMap parameters ) = 0;

};

typedef std::vector< IObserver* > ObserverList;

#endif