/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ComponentSerializerFactory.h
*  @date   2009/04/27
*/
#ifndef COMPONENTSERIALIZERFACTORY_H
#define COMPONENTSERIALIZERFACTORY_H

#include "IComponentSerializer.hpp"

namespace Serialization
{
	/*!
	 *  An Abstract Factory For Creating Component Serializers 
	 */
	class ComponentSerializerFactory
	{

	public:

		/*! Creates the correct Component Serializer given the System Type
		 *
		 *  @param[in] const System::Types::Type & systemType
		 *  @return (IComponentSerializer*)
		 */
		static IComponentSerializer* Create( const System::Types::Type& systemType );

	private:

		~ComponentSerializerFactory( ) { };
		ComponentSerializerFactory( ) { };
		ComponentSerializerFactory( const ComponentSerializerFactory & copy ) { };
		ComponentSerializerFactory & operator = ( const ComponentSerializerFactory & copy ) { return *this; };

	};
};

#endif