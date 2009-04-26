/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\System\AnyValue.hpp
*  @date   2009/04/25
*/
#ifndef __ANYVALUE_H
#define __ANYVALUE_H

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
		: _value( value )
	{

	}


	/*! Constructor to accept specific char* values
	 *
	 *  @param[in] const char * value
	 *  @return ()
	 */
	AnyValue( const char* value )
		: _value( std::string( value ) )
	{

	}


	/*! Constructor that accepts any value
	 *
	 *  @param[in] const T & value
	 *  @return ()
	 */
	template< class T >
	AnyValue( const T& value )
		: _value( value )
	{

	}


	/*! Gets the contained Value and casts it to the specified type
	 *
	 *  @return (boost::T)
	 */
	template< class T >
	T GetValue( ) { return boost::any_cast< T >( _value ); };


	/*! Gets the Type of the contained Value
	 *
	 *  @return (const std::type_info&)
	 */
	const std::type_info& GetType( ) { return _value.type( ); };

private:

	boost::any _value;

};

#endif