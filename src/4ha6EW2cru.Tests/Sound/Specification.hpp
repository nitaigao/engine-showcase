/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   Specification.hpp
*  @date   2009/04/27
*/
#ifndef __SPECIFICATION_HPP
#define __SPECIFICATION_HPP

#include <cppunit/extensions/HelperMacros.h>

namespace Testing
{
	/*!
	 *  A BDD Specification 
	 */
	template< class T >
	class Specificaton : public CPPUNIT_NS::TestFixture
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~Specificaton( )
		{
			
		}


		/*! Default Constructor
		 *
		 *  @return ()
		 */
		Specificaton( )
			: _subject( 0 )
		{

		}


		/*! Standard Intercept to Setup the Test
		 *
		 *  @return (void)
		 */
		void setUp( )
		{
			this->SetupTest( );
			this->EstablishBaseContext( );
		}


		/*! Standard Intercept to TearDown the Test
		*
		*  @return (void)
		*/
		void tearDown( )
		{
			this->TearDownTest( );

			if ( _subject != 0 )
			{
				delete _subject;
			}
		}


		/*! override to set up test data in the base context
		 *
		 *  @return (void)
		 */
		virtual void SetupTest( ) { };


		/*! Override to tear the test data down
		 *
		 *  @return (void)
		 */
		virtual void TearDownTest( ) { };


		/*! Override to setup the test data in the BaseContext or derived classes
		 *
		 *  @return (void)
		 */
		virtual void EstablishBaseContext( ) { };


	protected:

		T* _subject;

	private:

		
		Specificaton( const Specificaton & copy ) { };
		Specificaton & operator = ( const Specificaton & copy ) { return *this; };

	};
};

#endif