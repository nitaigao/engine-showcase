/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_LANDSCAPE_REPOSITORY_H
#define HK_LANDSCAPE_REPOSITORY_H

#include <Common/Base/hkBase.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>

class hkpBvTreeShape;

class LandscapeContainer : public hkReferencedObject
{
	public:

		LandscapeContainer( );

		virtual ~LandscapeContainer();

	public:

		hkpBvTreeShape* m_shape;

		class hkPackfileData* m_packfileData;

		hkArray<hkAabb> m_spawnVolumes;

		hkVector4 m_cameraFrom;
		hkVector4 m_cameraTo;
};

class AabbSpawnUtil : public hkReferencedObject
{
	public:

		AabbSpawnUtil( hkArray<hkAabb>& m_spawnVolumes );

		void getNewSpawnPosition(const hkVector4& aabbDims, hkVector4& positionOut);

		hkArray<hkAabb>& m_spawnVolumes;
		bool m_allowOverlaps;
		int m_currentSpawnVolume;

		hkObjectArray<hkArray<hkAabb> > m_spawnedAabbs;

		hkPseudoRandomGenerator m_pseudoRandomGenerator;

};


typedef LandscapeContainer* (HK_CALL *createLandscapeFunc)( void* param );

class LandscapeRepository: public hkReferencedObject, public hkSingleton<LandscapeRepository>
{
	public:

		LandscapeRepository();
		~LandscapeRepository();

		int getNumLandscapes();
		void setCurrentLandscapeIndex( int i );

		int getCurrentLandscapeIndex();

		LandscapeContainer* getLandscape( int i );
		int getLandscapeIndexByName( const char* name );

		LandscapeContainer* getCurrentLandscape();


	protected:

		struct LandscapeEntry
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO_FRAMEWORK, LandscapeEntry);

			createLandscapeFunc m_createFunc;
			char* m_name;
			void* m_param;
		};

		hkArray< LandscapeEntry > m_landscapeEntries;
		int m_currentLandscape;

		hkArray< LandscapeContainer* > m_cachedLandscapes; 

	public:
		static struct LandscapeEntryRegister* s_landscapeList;

};

struct LandscapeEntryRegister
{
	LandscapeEntryRegister( createLandscapeFunc func, char* name, void* param )
	{
		m_createFunc = func;
		m_name = name;
		m_param = param;

		m_next = LandscapeRepository::s_landscapeList;
		LandscapeRepository::s_landscapeList = this;
	}

	createLandscapeFunc m_createFunc;
	char* m_name;
	void* m_param;
	LandscapeEntryRegister* m_next;
};

#define HK_DECLARE_LANDSCAPE( CREATE_FUNC, NAME, PARAM ) \
	LandscapeEntryRegister LandscapeEntryRegister##CREATE_FUNC##NAME (CREATE_FUNC, #NAME, PARAM)

//#define HK_DECLARE_LANDSCAPE_FILE( filename, name )
//	HK_DECLARE_LANDSCAPE( loadHkxFunc, name, filename )



#endif //HK_LANDSCAPE_REPOSITORY_H


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
