/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IEventListener.hpp
*  @date   2009/04/25
*/
#ifndef IEVENTLISTENER_H
#define IEVENTLISTENER_H

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
		virtual inline EventType GetEventType( ) const = 0;

		
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
