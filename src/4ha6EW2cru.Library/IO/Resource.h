/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   Resource.h
*  @date   2009/04/26
*/
#ifndef RESOURCE_H
#define RESOURCE_H

#include "../Exceptions/OutOfRangeException.hpp"

#include "IResource.hpp"

namespace Resources
{
	/*! 
	 *  A Game Resource
	 */
	class Resource : public IResource
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~Resource( )
		{
			delete m_fileBuffer;
		};


		/*! Default Constructor
		 *
		 *  @param[in] FileBuffer * fileBuffer
		 *  @return ()
		 */
		Resource( IO::FileBuffer* fileBuffer )
			: m_fileBuffer( fileBuffer )
		{

		};


		/*! Returns the file data of the Resource
		 *
		 *  @return (FileBuffer*)
		 */
		inline IO::FileBuffer* GetFileBuffer( ) const { return m_fileBuffer; };


		/*! Increments the reference count for this resource 
		*   If it hit's 0 the resource will be queued for deletion
		*
		*  @return (void)
		*/
		inline void AddReference( ) { m_referenceCount++; };


		/*! De increments the reference count for this resource 
		*  If it hit's 0 the resource will be queued for deletion
		*
		*  @return (void)
		*/
		inline void RemoveReference( )
		{
			if ( m_referenceCount == 0 )
			{
				throw OutOfRangeException( "Attempted to de increment a reference on a Resource that already has a reference count of 0" );
			}

			m_referenceCount--;
		}

	private:

		IO::FileBuffer* m_fileBuffer;
		unsigned int m_referenceCount;

	};
}

#endif