/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\Events\IEventListener.hpp
*  @date   2009/04/25
*/
#ifndef __IEVENTLISTENER_H
#define __IEVENTLISTENER_H

#include <boost/function.hpp>

#include "IEvent.hpp"
#include "EventType.hpp"

namespace Events
{
	/*! 
	 *  Interface that listens for Events and handles the appropriate action
	 */
	class IEventListener
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IEventListener( ) { };

		
		/*! Handles the Event that is being listened for
		 *
		 *  @param[in] const IEvent *
		 *  @return (void)
		 */
		virtual void HandleEvent( const IEvent* ) const = 0;


		/*! Gets the EventType that is being listened for
		 *
		 *  @return (const EventType)
		 */
		virtual inline const EventType GetEventType( ) const = 0;

		
		/*! Marks the EventHandler for Deletion on the Next Update
		 *
		 *  @return (void)
		 */
		virtual inline void MarkForDeletion( ) = 0;


		/*! Gets whether the EventHandler is marked for deletion
		 *
		 *  @return (bool)
		 */
		virtual inline bool IsMarkedForDeletion( ) const = 0;

	};
};

#endif
