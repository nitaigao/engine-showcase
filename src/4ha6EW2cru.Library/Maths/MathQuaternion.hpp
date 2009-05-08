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

		/*! Default Constructor returning an IDENTITY Quaternion
		 *
		 *  @return ()
		 */
		MathQuaternion( )
			: _x( 0 )
			, _y( 0 )
			, _z( 0 )
			, _w( 1 )
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
			_w = cos( 0.5 * angle );
			_x = sin( 0.5 * angle ) * axis.X; 
			_y = sin( 0.5 * angle ) * axis.Y; 
			_z = sin( 0.5 * angle ) * axis.Z; 

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
			: _x( x )
			, _y( y )
			, _z( z )
			, _w( w )
		{

		}


		/*! Constructs from Ogre's representation of a Quaternion for convenience and speed!
		 *
		 *  @param[in] const Ogre::Quaternion & quaternion
		 *  @return ()
		 */
		MathQuaternion( const Ogre::Quaternion& quaternion )
			: _x( quaternion.x )
			, _y( quaternion.y )
			, _z( quaternion.z )
			, _w( quaternion.w )
		{

		}

		/*! Constructs from Havok's representation of a Quaternion for convenience and speed!
		 *
		 *  @param[in] const hkQuaternion & quaternion
		 *  @return ()
		 */
		MathQuaternion( const hkQuaternion& quaternion )
			: _x( quaternion( 0 ) )
			, _y( quaternion( 1 ) )
			, _z( quaternion( 2 ) )
			, _w( quaternion( 3 ) )
		{

		}


		/*! Returns an Ogre representation the Quaternion for convenience
		 *
		 *  @return (Ogre::Quaternion)
		 */
		inline Ogre::Quaternion AsOgreQuaternion( ) const { return Ogre::Quaternion( _w, _x, _y, _z ); };


		/*!  Returns a Havok representation the Quaternion for convenience
		 *
		 *  @return (hkQuaternion)
		 */
		inline hkQuaternion AshkQuaternion( ) const { return hkQuaternion( _x, _y, _z, _w ); };
		

		/*! Returns a Normalized version of the Quaternion
		 *
		 *  @return (Maths::MathQuaternion)
		 */
		inline MathQuaternion Normalize( ) const 
		{
			float length = sqrt( 
				_x * _x + _y * _y +
				_z * _z + _w * _w
				);

			return MathQuaternion(
				_x / length,
				_y / length,
				_z / length,
				_w / length
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
			float squaredLength = _x * _x + _y * _y + _z * _z;

			if( squaredLength > 0.0f )
			{
				angle = 2.0f * acos( _w );
				float inverseLength = 1.0f / sqrt( squaredLength );
				
				axis = MathVector3(
					_x * inverseLength,
					_y * inverseLength,
					_z * inverseLength
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
				_w * input._x + _x * input._w + _y * input._z - _z * input._y,
				_w * input._y + _y * input._w + _z * input._x - _x * input._z,
				_w * input._z + _z * input._w + _x * input._y - _y * input._x,
				_w * input._w - _x * input._x - _y * input._y - _z * input._z
			);
		};


		/*! Returns an IDENTITY Quaternion
		 *
		 *  @return (Maths::MathQuaternion)
		 */
		static MathQuaternion Identity( ) { return MathQuaternion( ); }; 

	private:

		float _x;
		float _y;
		float _z;
		float _w;

	};
};

#endif