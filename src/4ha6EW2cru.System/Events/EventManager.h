/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   EventManager.h
*  @date   2008/10/25
*/
#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <queue>
#include <map>

#include "IEvent.hpp"
#include "EventListener.h"

#include "../Export.hpp"

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

		
		/*! Queues an Event for processing on the next call to Update
		 *
		 *  @param[in] const IEvent * event
		 *  @return (void)
		 */
		GAMEAPI void QueueEvent( const Events::IEvent* event );


		/*! Processes an Event immediately
		 *
		 *  @param[in] const IEvent * event
		 *  @return (void)
		 */
		GAMEAPI void TriggerEvent( const Events::IEvent* event );

		
		/*! Dispatched all events in the Event Queue to their Listening Event Handlers
		 *
		 *  @param[in] float deltaMilliseconds
		 *  @return (void)
		 */
		void Update( const float& deltaMilliseconds );

		
		/*! Adds an EventListener for Event processing
		 *
		 *  @param[in] const EventType eventType
		 *  @param[in] T * handlerTarget
		 *  @param[in] void 
		 *  @param[in] T:: * handlerFunctor 
		 *  @param[in] 
		 *  @param[in] const IEvent * event 
		 *  @return (void)
		 */
		template< class T >
		void AddEventListener( const EventType eventType, T* handlerTarget, void ( T::*handlerFunctor ) ( const IEvent* event ) )
		{
			if ( 0 == handlerTarget )
			{
				NullReferenceException nullTarget( "EventManager::AddEventListener - Event Target is NULL" );
				Logging::Logger::Get( )->Fatal( nullTarget.what( ) );
				throw nullTarget;
			}

			if ( 0 == handlerFunctor )
			{
				NullReferenceException nullFunctor( "EventManager::AddEventListener - Handler Functor is NULL" );
				Logging::Logger::Get( )->Fatal( nullFunctor.what( ) );
				throw nullFunctor;
			}

			EventListener< T >* eventListener = new EventListener< T >( eventType, handlerTarget, handlerFunctor );
			m_eventListeners.push_back( eventListener );
		}


		/*! Marks an Event Listener for removal on the next call to Update
		 *
		 *  @param[in] const EventType eventType
		 *  @param[in] T * handlerTarget
		 *  @param[in] void 
		 *  @param[in] T:: * handlerFunctor 
		 *  @param[in] 
		 *  @param[in] const IEvent * event 
		 *  @return (void)
		 */
		template< class T >
		void RemoveEventListener( const EventType eventType, T* handlerTarget, void ( T::*handlerFunctor ) ( const IEvent* event ) )
		{
			if ( 0 == handlerTarget )
			{
				NullReferenceException nullTarget( "EventManager::RemoveEventListener - Event Target is NULL" );
				Logging::Logger::Get( )->Fatal( nullTarget.what( ) );
				throw nullTarget;
			}

			if ( 0 == handlerFunctor )
			{
				NullReferenceException nullFunctor( "EventManager::RemoveEventListener - Handler Functor is NULL" );
				Logging::Logger::Get( )->Fatal( nullFunctor.what( ) );
				throw nullFunctor;
			}

			for ( EventListenerList::iterator i = m_eventListeners.begin( ); i != m_eventListeners.end( ); ++i )
			{
				EventListener< T >* eventListener = static_cast< EventListener< T >* >( ( *i ) );

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

		EventManager( const EventManager & copy ) { };
		EventManager & operator = ( const EventManager & copy ) { return *this; };

		EventQueue m_eventQueue;
		EventListenerList m_eventListeners;

	};
};

#endif
