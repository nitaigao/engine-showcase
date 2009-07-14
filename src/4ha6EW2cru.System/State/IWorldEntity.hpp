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
	class IWorldEntity : public IObserver
	{

	public:

		typedef std::deque< IWorldEntity* > WorldEntityList;

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


		/*! Initializes all of the components contained by the entity
		*
		* @return ( void )
		*/
		virtual void Initialize( ) = 0;


		/*! Adds a System Component to the Entity
		 *
		 *  @param[in] ISystemComponent * component
		 *  @return (void)
		 */
		virtual void AddComponent( ISystemComponent* component ) = 0;


		/*! Get a list of all System Components inside the Entity
		 *
		 *  @return (ISystemComponent::SystemComponentList&)
		 */
		virtual ISystemComponent::SystemComponentList GetComponents( ) const = 0;

		/*! Adds a Foreign Observer to the Entity
		*
		*  @param[in] IObserver * observer
		*  @return (void)
		*/
		virtual void AddObserver( const System::Message& message, IObserver* observer ) = 0;

	};
};

#endif