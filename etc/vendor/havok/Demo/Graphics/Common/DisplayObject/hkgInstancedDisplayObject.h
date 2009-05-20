/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_INSTANCED_DISPLAY_OBJECT_H
#define HK_GRAPHICS_INSTANCED_DISPLAY_OBJECT_H

#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Common/DisplayObject/hkgInstancedDisplayObjectDefines.h>

class hkgInstancedDisplayObject : public hkgDisplayObject
{
	public:

		static hkgInstancedDisplayObject* (HK_CALL *create)( hkgDisplayContext* ctx );

			/// Default create function, use ::create instead.
		static hkgInstancedDisplayObject* defaultCreateInternal( hkgDisplayContext* ctx );

		virtual hkgDisplayObject* copy( HKG_DISPLAY_OBJECT_COPY_FLAGS f, hkgDisplayContext* ctx /*only required if doing deeper copies (facesets and/or verts)*/ );

		virtual void setMaxNumObjects( int numObjects, bool allocateBuffers = true ) = 0;
		virtual void setNumObjects( int numObjects );

		virtual void setRenderMode(HKG_INSTANCED_DISPLAY_MODE mode) = 0;
		inline HKG_INSTANCED_DISPLAY_MODE getRenderMode() { return m_renderMode; }

		inline int getNumObjects() const { return m_currentNumObjects; }

		virtual void setTransform( const float* transform, int objectIdx ) = 0;
		virtual void setTransformAligned( const hkTransform& transform, int objectIdx ) { setTransform( (const float*)(&transform), objectIdx ); }

		virtual const float* getTransform( int objectIdx ) const = 0;

		void setStartTransformIndex( int index );
		int getStartTransformIndex() const;

		inline hkUlong getUserShapeId() const { return m_userShapeId; }
		inline void setUserShapeId(hkUlong id) { m_userShapeId = id; }

			// per index user data. This will use mem, so only set if you need it (for picking perhaps)
		inline void setUserDataAtIndex( int index, hkUlong d ) { if (m_perIndexData.getSize() <= index) { m_perIndexData.setSize(index + 1, 0); } m_perIndexData[index] = d; }
		inline hkUlong getUserDataFromIndex( int index ) const { if (m_perIndexData.getSize() > index) return m_perIndexData[index]; else return 0; }

	protected:

		inline hkgInstancedDisplayObject( )
			: m_renderMode(HKG_INSTANCED_DISPLAY_TCOORD123), m_maxNumObjects(0), m_currentNumObjects(0), m_startTransformIndex(0), m_userShapeId(0), m_perInstanceBoundingRadius(-1)
		{
			setStatusFlags( getStatusFlags() | HKG_DISPLAY_OBJECT_INSTANCED );
		}

		HKG_INSTANCED_DISPLAY_MODE m_renderMode;
		
		int m_maxNumObjects;
		int m_currentNumObjects;
		int m_startTransformIndex;

		hkUlong m_userShapeId;
		hkArray<hkUlong> m_perIndexData;
		mutable float m_perInstanceBoundingRadius;
};

#endif // HK_GRAPHICS_INSTANCED_DISPLAY_OBJECT_H


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
