/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ISubject.hpp
*  @date   2009/04/25
*/
#ifndef __ISUBJECT
#define __ISUBJECT

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


	/*! Pushes the specified Changes to all Registered Observers
	 *
	 *  @param[in] const unsigned int& systemChanges
	 *  @return (void)
	 */
	virtual void PushChanges( const unsigned int& systemChanges ) = 0;

};

#endif