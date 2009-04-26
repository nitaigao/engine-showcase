/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\System\IObserver.hpp
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
	virtual unsigned int GetRequestedChanges( ) = 0;


	/*! Observes the changes when the ISubject notifies
	 *
	 *  @param[in] ISubject * subject
	 *  @param[in] unsigned int systemChanges
	 *  @return (void)
	 */
	virtual void Observe( ISubject* subject, unsigned int systemChanges ) = 0;

};

typedef std::vector< IObserver* > ObserverList;

#endif