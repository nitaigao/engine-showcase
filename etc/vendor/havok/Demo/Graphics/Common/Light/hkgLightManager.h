/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_LIGHT_MANAGER
#define HK_GRAPHICS_LIGHT_MANAGER

#include <Common/Base/Types/hkBaseTypes.h>
#include <Graphics/Common/Light/hkgLight.h>

/// The light manager, similar to the material manager, makes sure that the
/// given set of lights is in a proper state at all times. It can decide which
/// lights to enable based on current hardware limitations (for instance just
/// picking the nearest lights).
class hkgLightManager : public hkgReferencedObject
{
	public:

			/// Create a Light Manager. This does not need to be platform specific
			/// so you will see that there are no virtual calls in this object.
		static hkgLightManager* (HK_CALL *create)();
			/// Default create function, use ::create instead.
		inline static hkgLightManager* defaultCreateInternal();

			/// Get the number of lights in the manager. These lights may be in three
			/// states: on, desired on (but disabled due to hardware constraints), or disabled.
		inline int getNumLights() const;

			/// Get the i-th light in the manager.
		inline hkgLight* getLight(int i);

			/// Get the const pointer to the i-th light in the manager.
		inline const hkgLight* getLight(int i) const;

			/// Add the given light to this manager. The manager will increment the
			/// reference count of the light.
		inline int addLight(hkgLight* l);

			/// Find the light 'l'. It will return the index to the light, or -1 if not found.
		inline int findLight(hkgLight* l) const;

			/// Remove the i-th light from the manager. This will not decrement the reference count
			/// so that may be used elsewhere. Decrement the ref count yourself when you no longer
			/// require the light pointer.
		inline hkgLight* removeLight(int i);

			/// The active set is the current set of lights that are actually enabled by the display.
			/// Due to hardware constraints we can't have all lights active at once in large scenes
			/// so this is a simple way to have a set of active lights to use, given the current point
			/// on interest in the scene.
		inline int getActiveSetSize() const;

			/// Get the const pointer i-th light from the current active light set. Use getActiveSetSize()
			/// to find the list size.
		inline const hkgLight* getActiveSetLight(int i) const;

			/// Get the i-th light from the current active light set. Use getActiveSetSize()
			/// to find the list size.
		inline hkgLight* getActiveSetLight(int i);

			/// Set the limit of the active set size. Usually 8, but can be higher or lower.
		inline void setMaxActiveSetSize(int i);

			/// Compute the active set given the current point of interest. At the moment, this
			/// will just pick the first lights it finds that want to be enabled.. so this
			/// will change in future.
		void computeActiveSet( const float worldPointOfInterest[3]);

			/// Make all active lights in the active set realized in the given
			/// context. This will give the underlying hardware the light set information.
		void realizeActiveLights(hkgDisplayContext* context) const;

			/// Enable the given light (as the index into the full light list). Not fully implemented yet.
		void enable(int light);

			/// Disable the given light (as the index into the full light list). Not fully implemented yet.
		void disable(int light);

			/// Enable the light as given. Not fully implemented yet.
		void enable(hkgLight* light);

			/// Disable the light as given. Not fully implemented yet.
		void disable(hkgLight* light);

			/// Add to the scene ambient value for the scene in the form of 3 floats in the range 0-->1.
		inline void addSceneAmbient( const float a[3] );

			/// Set the ambient value for the scene in the form of 3 floats in the range 0-->1.
		inline void setSceneAmbient( const float a[3] );

			/// Get the ambient value for the scene in the form of 3 floats in the range 0-->1.
		inline void getSceneAmbient( float a[3] ) const;

			/// Get the ambient value for the scene in the form of 3 floats in the range 0-->1 as a pointer to the internal structure.
		inline float* getSceneAmbientPtr();

			/// Create 'numLights', with the given 'distanceFromOrigin', such that they lie on the plane perpendicular to 'up'.
		void addDefaultLights( float distanceFromOrigin, const float up[3], const float front[3], int numLights);

		const hkgLight* getBestShadowCaster() const;

			/// Print the light manager information to the given stream, assuming that the stream is text based.
		void print(hkOstream& stream) const;

		void lock() const;
		void unlock() const;

	protected:

		hkgLightManager();
		virtual ~hkgLightManager();

		hkgArray<hkgLight*>	m_lights;
		hkgArray<hkgLight*>	m_activeSet;
		int					m_maxActiveSetSize;

		float				m_sceneAmbient[3];

		mutable class hkCriticalSection*	m_lightLock;

};

#include <Graphics/Common/Light/hkgLightManager.inl>

#endif // HK_GRAPHICS_LIGHT_MANAGER

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
