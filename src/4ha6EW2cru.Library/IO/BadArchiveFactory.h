/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   BadArchiveFactory.h
*  @date   2009/04/26
*/
#ifndef __BADARCHIVEFACTORY_H
#define __BADARCHIVEFACTORY_H

#include "OgreArchiveFactory.h"
#include "OgreArchive.h"

namespace IO
{
	/*! 
	 *  A Factory class to produce BadArchives for Ogre
	 */
	class BadArchiveFactory : public Ogre::ArchiveFactory
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~BadArchiveFactory( ) { };


		/*! Default Constructor
		 *
		 *  @return ()
		 */
		BadArchiveFactory( ) { };


		/*! Returns the archive type of the BadArchive, in this case it's "BAD"
		 *
		 *  @return (const Ogre::String&)
		 */
		const Ogre::String& getType( ) const;


		/*! Creates an instance of a BadArchive
		 *
		 *  @param[in] const Ogre::String & name
		 *  @return (Ogre::Archive*)
		 */
		Ogre::Archive* createInstance( const Ogre::String& name ); 


		/*! Destroys an instance of a BadArchive
		 *
		 *  @param[in] Ogre::Archive *
		 *  @return (void)
		 */
		void destroyInstance( Ogre::Archive* );  

	private:

		BadArchiveFactory( const BadArchiveFactory & copy ) { };
		BadArchiveFactory & operator = ( const BadArchiveFactory & copy ) { return *this; };

	};
};

#endif