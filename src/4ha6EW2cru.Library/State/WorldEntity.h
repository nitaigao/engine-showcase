/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   WorldEntity.h
*  @date   2009/04/27
*/
#ifndef __WORLDENTITY_H
#define __WORLDENTITY_H

#include "IWorldEntity.hpp"

namespace State
{
	/*!
	 *  A container for components that make up an Entity 
	 */
	class WorldEntity : public IWorldEntity, public IObserver
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
		WorldEntity( const std::string& name, const unsigned int& id )
			: _name( name )
			, _id( id )
		{

		}


		/*! Returns the name of the Entity
		*
		*  @return (const std::string&)
		*/
		inline const std::string& GetName( ) const { return _name; };


		/*! Returns a numerical Id for the whole Entity
		*
		*  @return (unsigned int)
		*/
		inline unsigned int GetId( ) const { return _id; };


		/*! Adds a System Component to the Entity
		*
		*  @param[in] ISystemComponent * component
		*  @return (void)
		*/
		void AddComponent( ISystemComponent* component );


		/*! Finds a System Component from within the Entity
		*
		*  @param[in] System::Types::Type systemType
		*  @return (ISystemComponent*)
		*/
		ISystemComponent* FindComponent( const System::Types::Type& systemType ) const;

		/*! Get a list of all System Components inside the Entity
		*
		*  @return (SystemComponentList&)
		*/
		inline SystemComponentList GetComponents( ) const { return _components; };


		/*! Gets the Changes the Observer is interested in
		*
		*  @return (unsigned int)
		*/
		inline unsigned int GetRequestedChanges( ) const  { return System::Changes::None; };


		/*! Observes the changes when the ISubject notifies
		*
		*  @param[in] ISubject * subject
		*  @param[in] const unsigned int& systemChanges
		*  @return (void)
		*/
		void Observe( ISubject* subject, const unsigned int& systemChanges );

	private:

		WorldEntity( const WorldEntity & copy ) { };
		WorldEntity & operator = ( const WorldEntity & copy ) { return *this; };

		std::string _name;
		unsigned int _id;
		SystemComponentList _components;

	};
};

#endif