/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IObserver.hpp
*  @date   2009/04/25
*/
#ifndef __IOBSERVER_H
#define __IOBSERVER_H

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
	virtual unsigned int GetRequestedChanges( ) const  = 0;


	/*! Observes the changes when the ISubject notifies
	 *
	 *  @param[in] ISubject * subject
	 *  @param[in] const unsigned int& systemChanges
	 *  @return (void)
	 */
	virtual void Observe( ISubject* subject, const unsigned int& systemChanges ) = 0;

};

typedef std::vector< IObserver* > ObserverList;

#endif