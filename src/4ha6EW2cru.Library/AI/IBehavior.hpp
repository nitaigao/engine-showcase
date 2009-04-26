/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\AI\IBehavior.hpp
*  @date   2009/04/25
*/
#ifndef __IBEHAVIOR_H
#define __IBEHAVIOR_H

/*! 
*  The Artificial Intelligence Behavior
*/
class IBehaviour
{

public:


	/*! Default Destructor
	*
	*  @return ()
	*/
	~IBehaviour( ) { };


	/*! Sets the Behavior
	*
	*  @param[in] const std::string & behavior
	*  @return (void)
	*/
	virtual void SetBehavior( const std::string& behavior ) = 0;


	/*! Gets the Behavior Name
	*
	*  @return (std::string)
	*/
	virtual std::string GetBehavior( ) const = 0;

};

#endif