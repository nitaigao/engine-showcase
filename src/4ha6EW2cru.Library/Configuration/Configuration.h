/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   Configuration.h
*  @date   2009/04/25
*/
#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

#include "IConfiguration.hpp"
#include "IConfigurationFile.hpp"

namespace Configuration
{
	/*! 
	 *  Contains configuration items from the file system
	 */
	class ClientConfiguration : public IConfiguration
	{
		typedef std::map< std::string, AnyValue > DefaultPropertyList;

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~ClientConfiguration( );


		/*! Loads the configuration from the specified file
		 *
		 *  @param[in] const std::string & filePath
		 *  @return (ClientConfiguration*)
		 */
		static IConfiguration* Load( const std::string& filePath );


		/*! Finds a configuration item based on the specified section and key
		 *
		 *  @param[in] const std::string & section
		 *  @param[in] const std::string & key
		 *  @return (AnyValue)
		 */
		AnyValue Find( const std::string& section, const std::string& key );


		/*! Sets the default value for a configuration item
		 *
		 *  @param[in] const std::string & section
		 *  @param[in] const std::string & key
		 *  @param[in] const AnyValue & value
		 *  @return (void)
		 */
		void SetDefault( const std::string& section, const std::string& key, const AnyValue& value );


		/*! Sets the value for a configuration item
		 *
		 *  @param[in] const std::string & section
		 *  @param[in] const std::string & key
		 *  @param[in] const AnyValue & value
		 *  @return (void)
		 */
		void Set( const std::string& section, const std::string& key, const AnyValue& value );

	private:

		ClientConfiguration( IConfigurationFile* configFile )
			: _configFile( configFile )
		{

		};

		IConfigurationFile* _configFile;
		DefaultPropertyList _defaultPropertyList;

		ClientConfiguration( const ClientConfiguration & copy ) { };
		ClientConfiguration & operator = ( const ClientConfiguration & copy ) { return *this; };
		
	};
};

#endif