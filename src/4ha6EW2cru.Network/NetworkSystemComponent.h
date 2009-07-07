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
		NetworkSystemComponent( ) { };


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
		inline void AddObserver( IObserver* observer ) { };


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
		inline AnyType PushMessage( const System::Message& message, AnyType::AnyTypeMap parameters ) { return AnyType( ); };


		/*! Messages the Component to influence its internal state
		*
		*  @param[in] const std::string & message
		*  @return (AnyType)
		*/
		AnyType Message( const System::Message& message, AnyType::AnyTypeMap parameters ) { return AnyType( ); };

	private:

		NetworkSystemComponent( const NetworkSystemComponent & copy ) { };
		NetworkSystemComponent & operator = ( const NetworkSystemComponent & copy ) { return *this; };

		AnyType::AnyTypeMap m_attributes;
		
	};
};

#endif