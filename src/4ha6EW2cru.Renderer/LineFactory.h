#pragma once
#ifndef LINEFACTORY_H
#define LINEFACTORY_H

#include "Line3d.h"

#include <Ogre.h>

namespace Renderer
{
	class LineFactory : public Ogre::MovableObjectFactory
	{

	protected: 

		/*! Creates an Instance of a Line
		*
		* @param[in] const String & name
		* @param[in] const NameValuePairList * params
		* @return ( MovableObject* )
		*/
		inline Ogre::MovableObject* createInstanceImpl( const Ogre::String& name, const Ogre::NameValuePairList* params = 0 ) { return new Line3D( name ); };

	public:

		/*! Default Destructor
		*
		* @return (  )
		*/
		~LineFactory( ) { }


		/*! Returns the Type that the Factory Serves
		*
		* @param[in] void
		* @return ( const String& )
		*/
		const Ogre::String& getType( void ) const;


		/*! Destroys an instance of Line
		*
		* @param[in] MovableObject * obj
		* @return ( void )
		*/
		inline void destroyInstance( Ogre::MovableObject* obj ) { delete obj; };

		static Ogre::String FACTORY_TYPE_NAME;

	};
}

#endif