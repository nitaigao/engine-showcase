/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IService.hpp
*  @date   2009/04/25
*/
#pragma once
#ifndef ISERVICE_H
#define ISERVICE_H

#include <vector>

#include "../System/SystemType.hpp"
#include "../System/AnyType.hpp"

namespace Services
{
	/*! 
	 *  An Anonymous Service Interface
	 */
	class IService
	{

	public:

		typedef std::vector< IService* > ServiceList;


		/*! Gets the System::Types::Type of the Service
		 *
		 *  @return (System::Types::Type)
		 */
		virtual System::Types::Type GetType( ) const = 0;


		/*! Executes a command on the Service
		 *
		 *  @param[in] const std::string & actionName
		 *  @param[in] AnyType::AnyTypeMap & parameters
		 *  @return (AnyType::AnyTypeMap)
		 */
		virtual AnyType::AnyTypeMap Execute( const std::string& actionName, AnyType::AnyTypeMap& parameters ) = 0;

	};
};

#endif