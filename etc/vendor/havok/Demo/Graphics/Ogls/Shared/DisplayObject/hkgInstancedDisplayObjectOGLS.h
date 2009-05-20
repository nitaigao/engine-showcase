/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKG_INSTANCED_DISPLAY_OBJECT_OGLS__H
#define HKG_INSTANCED_DISPLAY_OBJECT_OGLS__H

#include <Graphics/Common/DisplayObject/hkgInstancedDisplayObject.h>

class hkgInstancedDisplayObjectOGLS : public hkgInstancedDisplayObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DISPLAY);

		static hkgInstancedDisplayObject* HK_CALL createInstancedDisplayObjectOGLS( hkgDisplayContext* ctx );

		hkgInstancedDisplayObjectOGLS( );

		virtual ~hkgInstancedDisplayObjectOGLS();

			/// special render call to make use of hardware instancing support if any
		virtual void render( hkgDisplayContext* context ) const;

			/// special picking support
		virtual int intersect( const hkgRay& ray, bool ignoreBackfaces, hkgDisplayObjectPickData& data) const;

			/// set the total number of instanced objects, must be called BEFORE cloning or rendering ( finalises the VBO )
			/// No need to call if you are going to use replace buffers anyway
		virtual void setMaxNumObjects( int numObjects, bool allocateBuffers = true );

			/// adds a shallow copy (shares hkgGeometry down) with the same transform and same flags to m_displayObjects
		virtual hkgDisplayObject* clone( float* transform = HK_NULL );

		virtual void setRenderMode(HKG_INSTANCED_DISPLAY_MODE mode);
		
		virtual void setTransform( const float* transform, int objectIdx );
		virtual void setTransformAligned( const hkTransform& transform, int objectIdx );

		virtual const float* getTransform( int objectIdx ) const;

			// For Readback rendering:
		void setTransformRowPointer(int r, float* tp) { m_transformRowPtr[r] = tp; }
		float* getTransformRowPointer(int r) const { return m_transformRowPtr[r]; }
	
		void setElementTypeAndSize( unsigned int type, unsigned int elementSize); 
		inline void setTransformBuffer(int r, unsigned int vb) { m_gpuVB[r] = vb; }

protected:

		float* m_transformRowPtr[3];
		hkArray<float> m_cpuTransforms;

		unsigned int m_elementType;
		unsigned int m_elementSize;

		unsigned int m_gpuVB[3];
};

#endif // HKG_INSTANCED_DISPLAY_OBJECT_OGLS__H

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
