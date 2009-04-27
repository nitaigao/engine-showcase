/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IConfigurationFile.hpp
*  @date   2009/04/25
*/
#ifndef __ICONFIGURATIONFILE_H
#define __ICONFIGURATIONFILE_H

#include "../System/AnyValue.hpp"

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
		*  @param[in] const AnyValue & defaultValue
		*  @return (AnyValue)
		*/
		virtual AnyValue FindConfigItem( const std::string& section, const std::string& key, const AnyValue& defaultValue ) = 0;


		/*! Updates a configuration item based on the given section and key
		*
		*  @param[in] const std::string & section
		*  @param[in] const std::string & key
		*  @param[in] const AnyValue & value
		*  @return (void)
		*/
		virtual void Update( const std::string& section, const std::string& key, const AnyValue& value ) = 0;


		/*! Saves the configuration file to the File System
		*
		*  @param[in] const std::string & filePath
		*  @return (void)
		*/
		virtual void Save( const std::string& filePath ) = 0;

	};
};

#endif