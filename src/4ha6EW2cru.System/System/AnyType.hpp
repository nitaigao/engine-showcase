/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   AnyType.hpp
*  @date   2009/04/25
*/
#ifndef ANYTYPE_H
#define ANYTYPE_H

#include <map>
#include <string>
#include <boost/any.hpp>

/*! 
 *  A Container for Any Value
 */
class AnyType
{

public:

	typedef std::map< std::string, AnyType > AnyType::AnyTypeMap;
	typedef std::map< unsigned int, AnyType > AnyType::AnyTypeKeyMap;


	/*! Default Destructor
	 *
	 *  @return ()
	 */
	~AnyType( ) { };


	/*! Default Constructor
	 *
	 *  @return ()
	 */
	AnyType( ) { };


	/*! Constructor to accept specific int values
	 *
	 *  @param[in] const int & value
	 *  @return ()
	 */
	AnyType( const int& value )
		: m_value( value )
	{

	}


	/*! Constructor to accept specific char* values
	 *
	 *  @param[in] const char * value
	 *  @return ()
	 */
	AnyType( const char* value )
		: m_value( std::string( value ) )
	{

	}


	/*! Constructor that accepts any value
	 *
	 *  @param[in] const T & value
	 *  @return ()
	 */
	template< class T >
	AnyType( const T& value )
		: m_value( value )
	{

	}


	/*! Gets the contained Value and casts it to the specified type
	 *
	 *  @return (boost::T)
	 */
	template< class T >
	T As( ) const { return boost::any_cast< T >( m_value ); };


	/*! Gets the Type of the contained Value
	 *
	 *  @return (const std::type_info&)
	 */
	const std::type_info& GetType( ) { return m_value.type( ); };

private:

	boost::any m_value;

};

#endif