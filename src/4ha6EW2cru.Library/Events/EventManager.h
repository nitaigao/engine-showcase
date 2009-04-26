/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\Events\EventManager.h
*  @date   2009/04/25
*/
#ifndef __EVENTMANAGER_H
#define __EVENTMANAGER_H

#include <queue>
#include <map>

#include "IEvent.hpp"
#include "EventListener.h"
#include "IEventListener.hpp"

namespace Events
{
	/*! 
	 *  Manages all Events distributed throughout the Game
	 */
	class EventManager
	{

		typedef std::queue< const IEvent* > EventQueue;
		typedef std::vector< IEventListener* > EventListenerList;

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~EventManager( );


		/*! Default Constructor
		 *
		 *  @return ()
		 */
		EventManager( ) { };

		
		/*! Queues an Event for processing on the next Tick
		 *
		 *  @param[in] const IEvent * event
		 *  @return (void)
		 */
		void QueueEvent( const Events::IEvent* event );


		/*!  Processes an Event immediately
		 *
		 *  @param[in] const IEvent * event
		 *  @return (void)
		 */
		void TriggerEvent( const Events::IEvent* event );

		
		/*! Fires all events in the Event Queue
		 *
		 *  @param[in] float deltaMilliseconds
		 *  @return (void)
		 */
		void Update( float deltaMilliseconds );

		
		/*! Adds an EventListener for Event processing
		 *
		 *  @param[in] const EventType eventType
		 *  @param[in] AT * handlerTarget
		 *  @param[in] void 
		 *  @param[in] AT:: * handlerFunctor 
		 *  @param[in] 
		 *  @param[in] const IEvent * event 
		 *  @return (void)
		 */
		template< class AT >
		void AddEventListener( const EventType eventType, AT* handlerTarget, void ( AT::*handlerFunctor ) ( const IEvent* event ) )
		{
			if ( 0 == handlerTarget )
			{
				NullReferenceException nullTarget( "EventManager::AddEventListener - Event Target is NULL" );
				Logger::GetInstance( )->Fatal( nullTarget.what( ) );
				throw nullTarget;
			}

			if ( 0 == handlerFunctor )
			{
				NullReferenceException nullFunctor( "EventManager::AddEventListener - Handler Functor is NULL" );
				Logger::GetInstance( )->Fatal( nullFunctor.what( ) );
				throw nullFunctor;
			}

			EventListener< AT >* eventListener = new EventListener< AT >( eventType, handlerTarget, handlerFunctor );
			_eventListeners.push_back( eventListener );
		}

		
		/*! Marks an Event Listener for Removal on the Next Update
		 *
		 *  @param[in] const EventType eventType
		 *  @param[in] RT * handlerTarget
		 *  @param[in] void 
		 *  @param[in] RT:: * handlerFunctor 
		 *  @param[in] 
		 *  @param[in] const IEvent * event 
		 *  @return (void)
		 */
		template< class RT >
		void RemoveEventListener( const EventType eventType, RT* handlerTarget, void ( RT::*handlerFunctor ) ( const IEvent* event ) )
		{
			if ( 0 == handlerTarget )
			{
				NullReferenceException nullTarget( "EventManager::RemoveEventListener - Event Target is NULL" );
				Logger::GetInstance( )->Fatal( nullTarget.what( ) );
				throw nullTarget;
			}

			if ( 0 == handlerFunctor )
			{
				NullReferenceException nullFunctor( "EventManager::RemoveEventListener - Handler Functor is NULL" );
				Logger::GetInstance( )->Fatal( nullFunctor.what( ) );
				throw nullFunctor;
			}

			for ( EventListenerList::iterator i = _eventListeners.begin( ); i != _eventListeners.end( ); ++i )
			{
				EventListener< RT >* eventListener = static_cast< EventListener< RT >* >( ( *i ) );

				if (
					handlerFunctor == eventListener->GetHandlerFunctor( ) &&
					eventType == eventListener->GetEventType( ) &&
					handlerTarget == eventListener->GetHandlerTarget( )
					)
				{
					( *i )->MarkForDeletion( );
					return;
				}
			}
		}

	private:

		EventQueue _eventQueue;
		EventListenerList _eventListeners;

		EventManager( const EventManager & copy ) { };
		EventManager & operator = ( const EventManager & copy ) { return *this; };

	};
};

#endif
