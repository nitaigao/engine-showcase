/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   Event.h
*  @date   2009/04/25
*/
#ifndef __EVENT_H
#define __EVENT_H

#include "IEvent.hpp"
#include "IEventData.hpp"
#include "EventType.hpp"

namespace Events
{
	/*! 
	 *  A Game Event
	 */
	class Event : public IEvent
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~Event( )
		{
			if ( _eventData != 0 )
			{
				delete _eventData;
			}
		}


		/*! Default Constructor
		 *
		 *  @param[in] const EventType & eventType
		 *  @param[in] IEventData * eventData
		 *  @return ()
		 */
		Event( const EventType& eventType, IEventData* eventData = 0 )
			: _eventType( eventType )
			, _eventData( eventData )
		{

		};

		
		/*! Gets the Type of the Event
		 *
		 *  @return (EventType)
		 */
		inline EventType GetEventType( ) const { return _eventType; };

		
		/*! Returns Event Data attached to the Event
		 *
		 *  @return (IEventData*)
		 */
		inline IEventData* GetEventData( ) const { return _eventData; };

	private:

		EventType _eventType;
		IEventData* _eventData;

		Event( ) { };
		Event( const Event & copy ) { };
		Event & operator = ( const Event & copy ) { return *this; };

	};
};

#endif
