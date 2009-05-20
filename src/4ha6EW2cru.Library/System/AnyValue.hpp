/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   AnyValue.hpp
*  @date   2009/04/25
*/
#ifndef ANYVALUE_H
#define ANYVALUE_H

#include <map>
#include <string>
#include <boost/any.hpp>

/*! 
 *  A Container for Any Value
 */
class AnyValue
{

public:

	typedef std::map< std::string, AnyValue > AnyValue::AnyValueMap;
	typedef std::map< unsigned int, AnyValue > AnyValue::AnyValueKeyMap;


	/*! Default Destructor
	 *
	 *  @return ()
	 */
	~AnyValue( ) { };


	/*! Default Constructor
	 *
	 *  @return ()
	 */
	AnyValue( ) { };


	/*! Constructor to accept specific int values
	 *
	 *  @param[in] const int & value
	 *  @return ()
	 */
	AnyValue( const int& value )
		: m_value( value )
	{

	}


	/*! Constructor to accept specific char* values
	 *
	 *  @param[in] const char * value
	 *  @return ()
	 */
	AnyValue( const char* value )
		: m_value( std::string( value ) )
	{

	}


	/*! Constructor that accepts any value
	 *
	 *  @param[in] const T & value
	 *  @return ()
	 */
	template< class T >
	AnyValue( const T& value )
		: m_value( value )
	{

	}


	/*! Gets the contained Value and casts it to the specified type
	 *
	 *  @return (boost::T)
	 */
	template< class T >
	T GetValue( ) const { return boost::any_cast< T >( m_value ); };


	/*! Gets the Type of the contained Value
	 *
	 *  @return (const std::type_info&)
	 */
	const std::type_info& GetType( ) { return m_value.type( ); };

private:

	boost::any m_value;

};

#endif