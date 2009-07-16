/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IConfigurationFile.hpp
*  @date   2009/04/25
*/
#ifndef ICONFIGURATIONFILE_H
#define ICONFIGURATIONFILE_H

#include "../System/AnyType.hpp"

namespace Configuration
{
	/*! 
	*  Loads and Saves Configuration Information to the File System
	*/
	class IConfigurationFile
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		virtual ~IConfigurationFile( ) { };


		/*! Finds a Configuration Item based on the given section and key
		*
		*  @param[in] const std::string & section
		*  @param[in] const std::string & key
		*  @param[in] const AnyType & defaultValue
		*  @return (AnyType)
		*/
		virtual AnyType FindConfigItem( const std::string& section, const std::string& key, const AnyType& defaultValue ) = 0;


		/*! Returns an Entire Config Section
		*
		* @param[in] const std::string & section
		* @return ( AnyType::AnyTypeMap )
		*/
		virtual AnyType::AnyTypeMap FindConfigSection( const std::string& section ) = 0;


		/*! Updates a configuration item based on the given section and key
		*
		*  @param[in] const std::string & section
		*  @param[in] const std::string & key
		*  @param[in] const AnyType & value
		*  @return (void)
		*/
		virtual void Update( const std::string& section, const std::string& key, const AnyType& value ) = 0;


		/*! Saves the configuration file to the File System
		*
		*  @return (void)
		*/
		virtual void Save( ) = 0;

	};
};

#endif