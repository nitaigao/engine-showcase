/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

//
// Havok 2.0 Visual Debug Display Manager
//

#ifndef HKG_DISPLAY_HANDLER
#define HKG_DISPLAY_HANDLER

#include <Common/Base/hkBase.h>
#include <Common/Base/Container/PointerMap/hkPointerMap.h>

#include <Common/Visualize/hkDebugDisplayHandler.h>

static const int HKG_DISPLAY_HANDLER_DEFAULT_GRID_SPAN = 200;
static const int HKG_DISPLAY_HANDLER_DEFAULT_GRID_BASE = 10;
static const hkReal HKG_DISPLAY_HANDLER_DEFAULT_LINE_GAP = 5.0f; // in pixels

class hkDisplayGeometry;
class hkgDisplayWorld;
class hkgDisplayContext;
class hkgDisplayWindow;
class hkgDisplayObject;
class hkgCamera;

class hkgKeyboard;

class hkgDisplayHandler : public hkReferencedObject, public hkDebugDisplayHandler
{
	public:

		HKG_DECLARE_CLASS_ALLOCATOR();

		hkgDisplayHandler(hkgDisplayWorld* displayWorld, hkgDisplayContext* displayContext, class hkgWindow* window);
		virtual ~hkgDisplayHandler();

		//
		// Display handler functions
		//
		virtual hkResult addGeometry(const hkArray<hkDisplayGeometry*>& geometries, const hkTransform& transform, hkUlong id, int tag, hkUlong shapeIdHint);
		virtual hkResult addGeometryInstance(hkUlong originalInstanceId, const hkTransform& transform, hkUlong id, int tag, hkUlong shapeIdHint);
		virtual hkResult setGeometryColor( int color, hkUlong id, int tag);
		virtual hkResult updateGeometry(const hkTransform& transform, hkUlong id, int tag);
		virtual hkResult removeGeometry(hkUlong id, int tag, hkUlong shapeIdHint);

		virtual hkResult updateCamera(const hkVector4& from, const hkVector4& to, const hkVector4& up, hkReal nearPlane, hkReal farPlane, hkReal fov, const char* name);
		virtual hkResult updateBehavior(hkArray<int>& wordVarIdx, hkArray<int>& wordStack, hkArray<int>& quadVarIdx, hkArray<hkVector4>& quadStack,
			 hkArray<char*>& activeNodes, hkArray<int>& activeStateIds, hkArray<int>& activeTransitions, hkArray<hkQsTransform>& transforms);

		virtual hkResult displayPoint(const hkVector4& position, int colour, int tag);
		virtual hkResult displayLine(const hkVector4& start, const hkVector4& end, int colour, int tag);
		virtual hkResult displayTriangle(const hkVector4& a, const hkVector4& b, const hkVector4& c, int colour, int tag);

		virtual hkResult displayText(const char* text, int color, int tag);
		virtual hkResult display3dText(const char* text, const hkVector4& pos, int color, int tag);

		virtual hkResult displayGeometry(const hkArray<hkDisplayGeometry*>& geometries, const hkTransform& transform, int color, int tag);
		virtual hkResult displayGeometry(const hkArray<hkDisplayGeometry*>& geometries, int color, int tag);

		virtual hkResult sendMemStatsDump(const char* data, int length);

		virtual hkResult holdImmediate(); // does nothing as single buffered
		virtual hkResult step(hkReal frameTimeInMs);

		virtual void lockForUpdate();
		virtual void unlockForUpdate();

		void drawImmediate(); // draw
		void clear(); // clear the lists

		//
		// Functions specific to the hkg display handler
		//

		//
		// Mapping: "id/type" to "display object"
		//
		hkUlong getDisplayObjectId(const hkgDisplayObject* displayObject);

			// will not hold ref to obj
			// Note that the id at the moment is the addr of the collidable for the obj
		void addPrecreatedDisplayObject( hkUlong id, hkgDisplayObject* obj );
		void removePrecreatedDisplayObject( hkUlong id );

		hkgDisplayObject* findDisplayObject( hkUlong id );
		const hkPointerMap<hkUlong, hkgDisplayObject*>& getObjectMap() { return m_objectMap; }

		void clearCache(); // will just remove all cached display objs
		void clearDisplay(); // will remove all added geoms etc, including the cached objs

			/// By default if there are no precreated display objects registered
			/// for a body that comes in through addGeometry, this handler will
			/// create an ugly but usefull display version for it.
			/// You can turn this off with this flag.
		void setAutoDisplayCreationState( bool on );

		void setAutoInstancingSizeHint( int numObjectsPerGeom );
		int getAutoInstancingSizeHint() const;

		void setDisplayBodyCachingEnabled( bool on );
		bool getDisplayBodyCachingEnabled() const;

		void setAllowColorChangeOnPrecreated( bool on );
		void setTextureMapScale(float s);
		void setTextureMapLocalMode(bool local); // local or world verts used for texture vert creation
		void setFixedTextureMapMode(bool on); // if on, will give each tri the tcoords { 0,0  1,0  1,1  }

		void setDefaultMaterial(class hkgMaterial* mat);
		hkgMaterial* getDefaultMaterial() const;

		hkgDisplayContext* getContext() { return m_displayContext; }
		const hkgDisplayContext* getContext() const { return m_displayContext; }

		void setDepthTest( hkBool depthRead,  hkBool depthWrite ) { m_useDepthRead = depthRead; m_useDepthWrite = depthWrite; } // If enabled, triangles and lines will be drawn depth-tested

	protected:

		class hkgDisplayContext*	m_displayContext;
		class hkgWindow*			m_window;
		class hkgDisplayWorld*	m_displayWorld;
		class hkgMaterial*     m_defaultMaterial;

		class hkgFont*			m_font;
		class hkgCamera*			m_fontCam;

		float m_textureMapScale;
		bool m_textureMapLocal;
		bool m_fixedMapMode;

		hkUint32 m_instanceSizeHint;

		// Mapping: id (collidable) to hkg display object
		bool m_autoCreateEnabled;
		bool m_allowColorChangeOnPrecreated;
		bool m_displayCachingEnabled;

		hkPointerMap<hkUlong, hkgDisplayObject*> m_objectMap;
		hkPointerMap<const hkgDisplayObject*, hkUlong> m_reverseObjectMap;
		hkPointerMap<hkUlong, hkgDisplayObject*> m_precreatedObjectMap;

		struct GeomCache
		{
			HKG_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(GeomCache);
			hkgArray< class hkgGeometry* > m_geoms;
		};
		hkPointerMap<hkUlong, GeomCache*> m_shapeObjectCache;

		hkArray<class hkgInstancedDisplayObject*> m_instances;


		// Immediate mode data
		struct DisplayPoint
		{
			hkVector4 m_position;
			int m_color;
		};

		struct DisplayLine
		{
			hkVector4 m_start;
			hkVector4 m_end;
			int m_color;
		};

		struct DisplayTriangle
		{
			hkVector4 m_a;
			hkVector4 m_b;
			hkVector4 m_c;
			int m_color;
		};

		struct DisplayText
		{
			char m_buffer[256];
			hkVector4 m_pos;
			int m_color;
		};

		// single buffered. What you get is what you just asked for so no longer a frame behind..
		hkArray<DisplayPoint> m_displayPoints;
		hkArray<DisplayLine> m_displayLines;
		hkArray<DisplayTriangle> m_displayTriangles;
		hkArray<DisplayText> m_displayTexts;

		mutable class hkCriticalSection* m_dataLock;

		hkBool m_useDepthRead;
		hkBool m_useDepthWrite;

};

#endif // HKG_DISPLAY_HANDLER

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
