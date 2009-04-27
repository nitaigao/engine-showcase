/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IConfiguration.hpp
*  @date   2009/04/25
*/
#ifndef __ICONFIGURATION_H
#define __ICONFIGURATION_H

#include "../System/AnyValue.hpp"

namespace Configuration
{
	/*! 
	 *  The Game Configuration Source
	 */
	class IConfiguration
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IConfiguration( ) { };


		/*! Finds the value for the given section and key
		 *
		 *  @param[in] const std::string & section
		 *  @param[in] const std::string & key
		 *  @return (void)
		 */
		virtual AnyValue Find( const std::string& section, const std::string& key ) = 0;


		/*! Sets the Default value of an Item
		 *
		 *  @param[in] const std::string & section
		 *  @param[in] const std::string & key
		 *  @param[in] const boost::any & value
		 *  @return (void)
		 */
		virtual void SetDefault( const std::string& section, const std::string& key, const AnyValue& value ) = 0;


		/*! Sets the value of an already defaulted item
		 *
		 *  @param[in] const std::string & section
		 *  @param[in] const std::string & key
		 *  @param[in] const std::string & value
		 *  @return (void)
		 */
		virtual void Set( const std::string& section, const std::string& key, const AnyValue& value ) = 0;

	};
};

#endif