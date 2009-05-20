/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ISubject.hpp
*  @date   2009/04/25
*/
#ifndef ISUBJECT
#define ISUBJECT

#include "../System/AnyValue.hpp"
#include "../System/IObserver.hpp"

/*! 
 *  The Subject of a System Change
 */
class ISubject
{

public:

	/*! Default Destructor
	 *
	 *  @return ()
	 */
	virtual ~ISubject( ) { };


	/*! Adds an Observer to the Subject
	*
	*  @param[in] IObserver * observer
	*  @return (void)
	*/
	virtual void AddObserver( IObserver* observer ) = 0;


	/*! Posts a message to observers
	*
	*  @param[in] const std::string & message
	*  @param[in] AnyValue::AnyValueMap parameters
	*  @return (AnyValue)
	*/
	virtual AnyValue PushMessage( const unsigned int& messageId, AnyValue::AnyValueKeyMap parameters ) = 0;

};

#endif