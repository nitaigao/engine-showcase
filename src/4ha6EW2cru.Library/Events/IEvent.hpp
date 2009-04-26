/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\Events\IEvent.hpp
*  @date   2009/04/25
*/
#ifndef __IEVENT_H
#define __IEVENT_H

#include "EventType.hpp"
#include "IEventData.hpp"

namespace Events
{
	/*! 
	 *  Interface for all Events
	 */
	class IEvent
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IEvent( ) { };

		
		/*! Gets the type of the Event
		 *
		 *  @return (EventType)
		 */
		virtual EventType GetEventType( ) const = 0;

		
		/*! Returns Event Data attached to the event
		 *
		 *  @return (IEventData*)
		 */
		virtual IEventData* GetEventData( ) const = 0;

	};
};

#endif
