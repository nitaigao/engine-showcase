/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\System\ISubject.hpp
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
	 *  @param[in] unsigned int systemChanges
	 *  @return (void)
	 */
	virtual void PushChanges( unsigned int systemChanges ) = 0;

};

#endif