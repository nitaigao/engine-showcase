/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ISubject.hpp
*  @date   2009/04/25
*/
#ifndef ISUBJECT
#define ISUBJECT

#include "../System/AnyType.hpp"
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
	*  @param[in] AnyType::AnyValueMap parameters
	*  @return (AnyType)
	*/
	virtual AnyType PushMessage( const System::Message& message, AnyType::AnyTypeMap parameters ) = 0;

};

#endif