/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IWorldEntity.hpp
*  @date   2009/04/27
*/
#ifndef IWORLDENTITY_HPP
#define IWORLDENTITY_HPP

#include "../System/ISystemComponent.hpp"

namespace State
{
	/*!
	 *  A World Entity Container 
	 */
	class IWorldEntity
	{

	public:

		typedef std::vector< IWorldEntity* > WorldEntityList;

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IWorldEntity( ) { };


		/*! Returns the name of the Entity
		 *
		 *  @return (const std::string&)
		 */
		virtual const std::string& GetName( ) const = 0;


		/*! Returns a numerical Id for the whole Entity
		 *
		 *  @return (unsigned int)
		 */
		virtual unsigned int GetId( ) const = 0;


		/*! Adds a System Component to the Entity
		 *
		 *  @param[in] ISystemComponent * component
		 *  @return (void)
		 */
		virtual void AddComponent( ISystemComponent* component ) = 0;


		/*! Finds a System Component from within the Entity
		 *
		 *  @param[in] System::Types::Type systemType
		 *  @return (ISystemComponent*)
		 */
		virtual ISystemComponent* FindComponent( const System::Types::Type& systemType ) const = 0;


		/*! Get a list of all System Components inside the Entity
		 *
		 *  @return (SystemComponentList&)
		 */
		virtual SystemComponentList GetComponents( ) const = 0;

	};
};

#endif