/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   NetworkSystemComponent.h
*  @date   2009/07/06
*/
#pragma once
#ifndef NETWORKSYSTEMCOMPONENT_H
#define NETWORKSYSTEMCOMPONENT_H

#include "INetworkSystemComponent.hpp"

namespace Network
{
	/*! 
	 *  A Network System Component
	 */
	class NetworkSystemComponent : public INetworkSystemComponent
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~NetworkSystemComponent( ) { };


		/*! Default Constructor
		*
		* @return (  )
		*/
		NetworkSystemComponent( )
			: m_observer( 0 )
		{

		}


		/*! Initializes the Component
		*
		*  @param[in] AnyType::AnyValueMap properties
		*  @return (void)
		*/
		inline void Initialize( ) { };


		/*! Steps the internal data of the Component
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds ) { };


		/*! Destroys the Component
		*
		*  @return (void)
		*/
		void Destroy( ) { };


		/*! Adds an Observer to the Component
		*
		*  @param[in] IObserver * observer
		*  @return (void)
		*/
		inline void AddObserver( IObserver* observer ) { m_observer = observer; };


		/*! Gets the properties of the Component
		*
		*  @return (AnyTypeKeyMap)
		*/
		AnyType::AnyTypeMap GetAttributes( ) const { return m_attributes; };


		/*! Sets an Attribute on the Component *
		*
		*  @param[in] const unsigned int attributeId
		*  @param[in] const AnyType & value
		*/
		inline void SetAttribute( const System::Attribute& attributeId, const AnyType& value ) { m_attributes[ attributeId ] = value; };


		/*! Posts a message to observers
		*
		*  @param[in] const std::string & message
		*  @param[in] AnyType::AnyValueMap parameters
		*  @return (AnyType)
		*/
		inline AnyType PushMessage( const System::Message& message, AnyType::AnyTypeMap parameters ) { return m_observer->Message( message, parameters ); };


		/*! Messages the Component to influence its internal state
		*
		*  @param[in] const std::string & message
		*  @return (AnyType)
		*/
		AnyType Message( const System::Message& message, AnyType::AnyTypeMap parameters );


		/*! Receives Messages from an inbound network connection
		*
		* @param[in] const std::string & message
		* @param[in] AnyType::AnyTypeMap parameters
		* @return ( void )
		*/
		void MessageFromNetwork( const std::string& message, AnyType::AnyTypeMap parameters );

	private:

		NetworkSystemComponent( const NetworkSystemComponent & copy ) { };
		NetworkSystemComponent & operator = ( const NetworkSystemComponent & copy ) { return *this; };

		AnyType::AnyTypeMap m_attributes;
		IObserver* m_observer;
		
	};
};

#endif