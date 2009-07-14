/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IConfiguration.hpp
*  @date   2009/04/25
*/
#ifndef ICONFIGURATION_H
#define ICONFIGURATION_H

#include "../System/AnyType.hpp"

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
		virtual AnyType Find( const std::string& section, const std::string& key ) = 0;


		/*! Finds an entire section
		*
		* @param[in] const std::string & section
		* @return ( AnyType::AnyTypeMap )
		*/
		virtual AnyType::AnyTypeMap FindSection( const std::string& section ) = 0;


		/*! Sets the Default value of an Item
		 *
		 *  @param[in] const std::string & section
		 *  @param[in] const std::string & key
		 *  @param[in] const boost::any & value
		 *  @return (void)
		 */
		virtual void SetDefault( const std::string& section, const std::string& key, const AnyType& value ) = 0;


		/*! Sets the value of an already defaulted item
		 *
		 *  @param[in] const std::string & section
		 *  @param[in] const std::string & key
		 *  @param[in] const std::string & value
		 *  @return (void)
		 */
		virtual void Set( const std::string& section, const std::string& key, const AnyType& value ) = 0;

	};
};

#endif