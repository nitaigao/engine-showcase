/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   WorldEntity.h
*  @date   2009/04/27
*/
#ifndef WORLDENTITY_H
#define WORLDENTITY_H

#include "IWorldEntity.hpp"

namespace State
{
	/*!
	 *  A container for components that make up an Entity 
	 */
	class WorldEntity : public IWorldEntity
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~WorldEntity( ) { };


		/*! Default Constructor
		 *
		 *  @param[in] const std::string & name
		 *  @return ()
		 */
		WorldEntity( const std::string& name )
			: m_name( name )
		{

		}


		/*! Returns the name of the Entity
		*
		*  @return (const std::string&)
		*/
		inline const std::string& GetName( ) const { return m_name; };


		/*! Adds an Observer to the Subject
		*
		*  @param[in] IObserver * observer
		*  @return (void)
		*/
		void AddObserver( const unsigned int& messageId, IObserver* observer ) { m_observers.insert( std::make_pair( messageId, observer ) ); };

		/*! Adds a System Component to the Entity
		*
		*  @param[in] ISystemComponent * component
		*  @return (void)
		*/
		void AddComponent( ISystemComponent* component );

		/*! Get a list of all System Components inside the Entity
		*
		*  @return (SystemComponentList&)
		*/
		inline SystemComponentList GetComponents( ) const { return m_components; };


		/*! Messages the Component to influence its internal state
		 *
		 *  @param[in] const std::string & message
		 *  @return (AnyValue)
		 */
		AnyValue Message( const unsigned int& messageId, AnyValue::AnyValueKeyMap parameters );

	private:

		WorldEntity( const WorldEntity & copy ) { };
		WorldEntity & operator = ( const WorldEntity & copy ) { return *this; };

		std::string m_name;
		SystemComponentList m_components;
		ObserverMap m_observers;

	};
};

#endif