/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\Events\EventListener.h
*  @date   2009/04/25
*/
#ifndef __EVENTLISTENER_H
#define __EVENTLISTENER_H

#include <functional>

#include "../Logging/Logger.h"
#include "../Exceptions/NullReferenceException.hpp"

#include "IEvent.hpp"
#include "EventType.hpp"
#include "IEventListener.hpp"

namespace Events
{
	/*! 
	 *  Listener for Events and handles the appropriate action
	 */
	template< class T >
	class EventListener : public IEventListener
	{

		typedef void ( T::*HandlerFunctor ) ( const IEvent* event ) ;

	public:

		/*! Default Constructor
		 *
		 *  @param[in] const EventType eventType
		 *  @param[in] T * const handlerTarget
		 *  @param[in] HandlerFunctor handlerFunctor
		 *  @return ()
		 */
		EventListener( const EventType eventType, T* const handlerTarget, HandlerFunctor handlerFunctor )
			: _handlerFunctor( handlerFunctor )
			, _eventType( eventType )
			, _handlerTarget( handlerTarget )
			, _markedForDeletion( false )
		{

		}

		
		/*! Calls the subscribed function for the Event
		 *
		 *  @param[in] const IEvent * event
		 *  @return (void)
		 */
		void HandleEvent( const IEvent* event ) const
		{
			if ( _handlerTarget == 0 )
			{
				NullReferenceException nullTarget( "EventListener::HandleEvent - HandlerTarget is NULL" );
				Logger::GetInstance( )->Fatal( nullTarget.what( ) );
				throw nullTarget;
			}

			if ( _handlerFunctor == 0 )
			{
				NullReferenceException nullHandler( "EventListener::HandleEvent - HandlerFunctor is NULL" );
				Logger::GetInstance( )->Fatal( nullHandler.what( ) );
				throw nullHandler;
			}

			if ( 0 == event )
			{
				NullReferenceException nullEvent( "EventListener::HandleEvent - Event is NULL" );
				Logger::GetInstance( )->Fatal( nullEvent.what( ) );
				throw nullEvent;
			}

			if ( !_markedForDeletion )
			{
				( _handlerTarget->*_handlerFunctor )( event );
			}
		}

		
		/*! Retrieves the Type of Event the EventListener is listening for
		 *
		 *  @return (const EventType)
		 */
		inline const EventType GetEventType( ) const { return _eventType; };

		
		/*! Returns a pointer to the method assigned to handle the Event
		 *
		 *  @return (const HandlerFunctor)
		 */
		inline const HandlerFunctor GetHandlerFunctor( ) const { return _handlerFunctor; };

		
		/*! Retrieves the object instance for which the EventListener is Listening
		 *
		 *  @return (const T*)
		 */
		inline const T* GetHandlerTarget( ) const { return _handlerTarget; };

		
		/*! Marks the EventHandler for Deletion on the Next Update
		 *
		 *  @return (void)
		 */
		inline void MarkForDeletion( ) { _markedForDeletion = true; };

		
		/*! Gets whether the EventHandler is marked for deletion
		 *
		 *  @return (bool)
		 */
		inline bool IsMarkedForDeletion( ) const { return _markedForDeletion; };

	private:

		HandlerFunctor _handlerFunctor;
		EventType _eventType;
		T* _handlerTarget;
		bool _markedForDeletion;

		~EventListener( ) { };
		EventListener( ) { };
		EventListener( const EventListener & copy ) { };
		EventListener & operator = ( const EventListener & copy ) { return *this; };

	};
};

#endif
