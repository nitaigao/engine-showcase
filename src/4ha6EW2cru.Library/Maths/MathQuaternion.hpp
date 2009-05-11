/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file  MathQuaternion.hpp
*  @date   2009/04/26
*/
#ifndef __MATHQUATERNION_H
#define __MATHQUATERNION_H

#include <OgreMatrix3.h>
#include <OgreQuaternion.h>
#include <Common/Base/hkBase.h>

#include "MathVector3.hpp"

namespace Maths
{
	/*! 
	 *  Representation of a Maths Quaternion
	 */
	class MathQuaternion
	{

	public:

		float X, Y, Z, W;

		/*! Default Constructor returning an IDENTITY Quaternion
		 *
		 *  @return ()
		 */
		MathQuaternion( )
			: X( 0 )
			, Y( 0 )
			, Z( 0 )
			, W( 1 )
		{

		}


		/*! Constructs from an Angle / Axis pair
		 *
		 *  @param[in] MathVector3 axis
		 *  @param[in] float angle
		 *  @return ()
		 */
		MathQuaternion( const MathVector3& axis, const float& angle )
		{
			W = cos( 0.5 * angle );
			X = sin( 0.5 * angle ) * axis.X; 
			Y = sin( 0.5 * angle ) * axis.Y; 
			Z = sin( 0.5 * angle ) * axis.Z; 

		}


		/*! Constructs from real values 
		 *
		 *  @param[in] float x
		 *  @param[in] float y
		 *  @param[in] float z
		 *  @param[in] float w
		 *  @return ()
		 */
		MathQuaternion( const float& x, const float& y, const float& z, const float& w )
			: X( x )
			, Y( y )
			, Z( z )
			, W( w )
		{

		}


		/*! Constructs from Ogre's representation of a Quaternion for convenience and speed!
		 *
		 *  @param[in] const Ogre::Quaternion & quaternion
		 *  @return ()
		 */
		MathQuaternion( const Ogre::Quaternion& quaternion )
			: X( quaternion.x )
			, Y( quaternion.y )
			, Z( quaternion.z )
			, W( quaternion.w )
		{

		}

		/*! Constructs from Havok's representation of a Quaternion for convenience and speed!
		 *
		 *  @param[in] const hkQuaternion & quaternion
		 *  @return ()
		 */
		MathQuaternion( const hkQuaternion& quaternion )
			: X( quaternion( 0 ) )
			, Y( quaternion( 1 ) )
			, Z( quaternion( 2 ) )
			, W( quaternion( 3 ) )
		{

		}


		/*! Returns an Ogre representation the Quaternion for convenience
		 *
		 *  @return (Ogre::Quaternion)
		 */
		inline Ogre::Quaternion AsOgreQuaternion( ) const { return Ogre::Quaternion( W, X, Y, Z ); };


		/*!  Returns a Havok representation the Quaternion for convenience
		 *
		 *  @return (hkQuaternion)
		 */
		inline hkQuaternion AshkQuaternion( ) const { return hkQuaternion( X, Y, Z, W ); };
		

		/*! Returns a Normalized version of the Quaternion
		 *
		 *  @return (Maths::MathQuaternion)
		 */
		inline MathQuaternion Normalize( ) const 
		{
			float length = sqrt( 
				X * X + Y * Y +
				Z * Z + W * W
				);

			return MathQuaternion(
				X / length,
				Y / length,
				Z / length,
				W / length
				);
		};


		/*! Fills the specified angle / axis pair with the Angle / Axis representation of
		 * the Quaternion
		 *
		 *  @param[out] float & angle
		 *  @param[out] MathVector3 & axis
		 *  @return (void)
		 */
		inline void ToAngleAxis( float& angle, MathVector3& axis )
		{
			float squaredLength = X * X + Y * Y + Z * Z;

			if( squaredLength > 0.0f )
			{
				angle = 2.0f * acos( W );
				float inverseLength = 1.0f / sqrt( squaredLength );
				
				axis = MathVector3(
					X * inverseLength,
					Y * inverseLength,
					Z * inverseLength
					);
			}
			else
			{
				angle = 0.0f;
				axis = MathVector3( 1.0f, 0.0f, 0.0f );
			}
		}


		/*! Returns the Dot Product of the Quaternion and the specified input
		 *
		 *  @param[in] const MathQuaternion & input
		 *  @return (Maths::MathQuaternion)
		 */
		inline MathQuaternion operator * ( const MathQuaternion& input )
		{
			return MathQuaternion(
				W * input.X + X * input.W + Y * input.Z - Z * input.Y,
				W * input.Y + Y * input.W + Z * input.X - X * input.Z,
				W * input.Z + Z * input.W + X * input.Y - Y * input.X,
				W * input.W - X * input.X - Y * input.Y - Z * input.Z
			);
		};


		/*! Returns an IDENTITY Quaternion
		 *
		 *  @return (Maths::MathQuaternion)
		 */
		static MathQuaternion Identity( ) { return MathQuaternion( ); }; 

	};
};

#endif