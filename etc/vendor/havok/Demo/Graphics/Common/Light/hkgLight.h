/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_LIGHT_H
#define HK_GRAPHICS_LIGHT_H

#include <Graphics/Common/Light/hkgLightDefines.h>

class hkgLightManager;

/// A simple vertex light class. It can represent directional, point (omni) and spot lights.
/// It is meant to be used through the light manager class so that lights will remain in
/// a consistant state and more than the hardware limit of lights can be defined and the 
/// manager will enable them as it sees fit.

class hkgLight : public hkgReferencedObject
{
		friend class hkgLightManager;

	public:
			/// Create a platform specific light object. This object will only really implement
			/// the updateState() method below as that is all that is required at the present time.
		static hkgLight* (HK_CALL *create)();
 
			/// Get the desired (not actual) state of this light. As multiple lights may not be
			/// due to hardware restrictions, this represents the desired state of the light.
		inline bool getDesiredEnabledState() const;

			/// The manager will decide which actually get enabled based on resources,
			/// but you can use this to indicate that a light is to be always off or try and be on.
		inline void setDesiredEnabledState(bool on);

			/// Query for the light state.
		inline bool getIsEnabled() const; // is actuall on

			/// Get the light number. This will range from 0 to the ((max number of hardware lights)-1), with
			/// the special value of -1 representing no light number (the light is not enabled).
		inline int getLightNumber() const; 

			/// Enable this light using the given light manager.
			/// Note that the light manager is required so that is always notified of state change,
		void enable( hkgLightManager* manager );

			/// Disable this light using the given light manager.
			/// Note that the light manager is required so that is always notified of state change,
		void disable( hkgLightManager* manager );

			/// Get the type of this light. It can be one of three values:
			/// HKG_LIGHT_DIRECTIONAL:  the cheapest of light sources where the light just has direction, no position.
			/// HKG_LIGHT_OMNI: a point light source.
			/// HKG_LIGHT_SPOT: a spot light with direction and falloff angles, both hotspot and outer most angle. This is the most expensive.
		inline HKG_LIGHT_TYPE getType() const;

			/// Set the for this light. Depending on which type of light the object will use
			/// different data in the class. See the getType() method for the three types.
		inline void setType(HKG_LIGHT_TYPE t);

			/// Get the world position of the light. Used for omni and spots.
		inline void   getPosition(float p[3]) const;

			/// Get the world position of the light as a pointer to the internal structure
			/// Used for omni and spots.
		inline float* getPositionPtr();

			/// Set the world position of the light. Used for omni and spots.		
		inline void   setPosition(const float p[3]);

			/// Get the direction of the light. Used for spot and directional lights.
		inline void   getDirection(float d[3]) const;
		
			/// Get the direction of the light as a pointer to the internal structure.
			/// Used for spot and directional lights.
		inline float* getDirectionPtr();
			
			/// Set the direction of the light. Used for spot and directional lights.
		inline void   setDirection(const float p[3]);

			/// Get the diffuse color as vector of 3 floats in the range 0-->1. Used by all light types.
		inline void   getDiffuse(float d[3]) const;

			/// Get the diffuse color as vector of 3 floats in the range 0-->1 as a pointer to
			/// the internal structure. Used by all light types.
		inline float* getDiffusePtr();

			/// Set the diffuse color as vector of 3 floats in the range 0-->1. Used by all light types.
		inline void   setDiffuse(const float p[3]);

			/// Get the specular color as vector of 3 floats in the range 0-->1. Used by all light types.
		inline void   getSpecular(float s[3]) const;

			/// Get the specular color as vector of 3 floats in the range 0-->1 as a pointer to
			/// the internal structure. Used by all light types.
		inline float* getSpecularPtr();

			/// Set the specular color as vector of 3 floats in the range 0-->1. Used by all light types.
		inline void   setSpecular(const float p[3]);

			/// Get the ambient color as vector of 3 floats in the range 0-->1. Used by all light types, but will
			/// usually get placed in one ambient term for all enabled lights and applied to the scene.
		inline void   getAmbient(float s[3]) const;
	
			/// Get the ambient color as vector of 3 floats in the range 0-->1 as a pointer to the internal structure. 
			/// Used by all light types, but will usually get placed in one ambient term for all enabled lights and applied to the scene.
		inline float* getAmbientPtr();

			/// Set the ambient color as vector of 3 floats in the range 0-->1. 
			/// Used by all light types, but will usually get placed in one ambient term for all enabled lights and applied to the scene.
		inline void   setAmbient(const float p[3]);

			/// Get the inner angle cutoff, This angle is the limit of the intense part of the 
			/// cone that represents the fully lit part of the spot. Measured in degrees, and limit of 180.
			/// Some systems hardware spots support only one angle (no falloff), so the outer angle will be the spot angle in that case.
		inline float getSpotInnerAngle() const;

			/// Get the outer angle cutoff, This angle is the limit of the lit part of the 
			/// cone that represents the end of the falloff of the spot. Measured in degrees, and limit of 180.
			/// Some systems hardware spots support only one angle (no falloff), so this will be the spot angle in that case.
		inline float getSpotOuterAngle() const;

			/// Get the exponent to use for the falloff between the inner and outer angle for the spots.
			/// Not used on most platforms.
		inline float getSpotFalloffExponent() const;

			/// Set the inner angle cutoff. See the getSpotInnerAngle() description for more info.
		inline void  setSpotInnerAngle(float a);

			/// Set the outer angle cutoff. See the getSpotOuterAngle() description for more info.
		inline void  setSpotOuterAngle(float a);

			/// Set falloff exponent between the inner and outer angle. Not used on most platforms.
		inline void  setSpotFalloffExponent(float a);

			/// Platform specific update. It will actually set the light up on the hardware with the 
			/// given data, in the given context.
		virtual void updateState(hkgDisplayContext* context) = 0; // call this after you alter type or parameters above

			/// For some simple shadow planes, we can add planes to a given light / set of lights
		inline void addShadowPlane( float plane[4] );

			/// Get the ith shadow matrix (for planar shadows)
		inline const float* getShadowMatrix(int i) const ;

			/// For some simple shadow planes, we can add planes to a given light / set of lights
		inline int getNumShadowPlanes() const;

	protected:

		inline   hkgLight();
		virtual ~hkgLight();

		inline void setLightNumber(int l);

		int		m_lightNum; 
		bool	m_desiredEnableState;
		
		float	m_pos[3];		// world position
		float   m_direction[3];	// world directiuon for direction and spot lights
		float   m_diffuse[3];	// diffuse color
		float   m_specular[3];	// ambient color added to scene
		float   m_ambient[3];
			
		float	m_spotInnerAngle;
		float	m_spotOuterAngle;
		float	m_spotExponent;

		HKG_LIGHT_TYPE m_type;

		struct ShadowPlane
		{
			float m_plane[4];
			float m_shadowMatrix[16];

			void compute(const float* lightPos /*4 component, w==1 for pos, 0 for dir*/);
		};
		hkArray<ShadowPlane> m_planes;
};

#include <Graphics/Common/Light/hkgLight.inl>

#endif //  HK_GRAPHICS_LIGHT_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20090216)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2009
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
