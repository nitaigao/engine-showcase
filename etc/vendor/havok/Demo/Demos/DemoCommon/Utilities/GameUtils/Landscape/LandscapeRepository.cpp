/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/LandscapeRepository.h>

// We will need these shapes
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>

#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>

#include <Common/Serialize/Packfile/hkPackfileData.h>

#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileWriter.h>
#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileReader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Physics/Utilities/Serialize/hkpHavokSnapshot.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>

LandscapeContainer::LandscapeContainer() 
{
	m_cameraFrom.set(-10, 2, -10); 
	m_cameraTo.set(0,0,0);
	m_shape = HK_NULL; 
	m_packfileData = HK_NULL;
}


LandscapeContainer::~LandscapeContainer()
{ 
	HK_ASSERT(0, m_shape != HK_NULL);
	if (m_shape)
	{
		m_shape->removeReference();
	}
	
	if ( m_packfileData )
	{
		m_packfileData->removeReference();
	}
}


LandscapeRepository::LandscapeRepository()
{
	m_currentLandscape = 0;
	LandscapeEntryRegister* e = s_landscapeList;
	while(e)
	{
		LandscapeEntry newEntry;
		newEntry.m_createFunc = e->m_createFunc;
		newEntry.m_name = e->m_name	;
		newEntry.m_param = e->m_param;
		m_landscapeEntries.pushBack( newEntry );
		e = e->m_next;
	}
	m_cachedLandscapes.setSize(m_landscapeEntries.getSize(), HK_NULL);

}

LandscapeRepository::~LandscapeRepository()
{
	for ( int i = 0; i < m_cachedLandscapes.getSize(); ++i)
	{
		if (m_cachedLandscapes[i] != HK_NULL )
		{
			m_cachedLandscapes[i]->removeReference();
		}
	}

}


int LandscapeRepository::getNumLandscapes()
{
	return m_landscapeEntries.getSize();
}

void LandscapeRepository::setCurrentLandscapeIndex( int i )
{
	HK_ASSERT(0, i >=0 && i < getNumLandscapes() );
	m_currentLandscape = i;
}

int LandscapeRepository::getCurrentLandscapeIndex()
{
	return m_currentLandscape;
}

LandscapeContainer* LandscapeRepository::getCurrentLandscape()
{
	return getLandscape( m_currentLandscape );
}

int LandscapeRepository::getLandscapeIndexByName( const char* name )
{
	for( int i = 0; i < m_landscapeEntries.getSize(); i++ )
	{
		if( hkString::strCmp(m_landscapeEntries[i].m_name, name ) == 0 )
		{
			return i;
		}
	}
	HK_WARN( 0x9420823a, "Couldn't find landscape repository " << name << ". Using landscape 0, " << m_landscapeEntries[0].m_name );
	return 0;
}


LandscapeContainer* LandscapeRepository::getLandscape( int i )
{
	if (m_cachedLandscapes[i] != HK_NULL )
	{
		return m_cachedLandscapes[i];
	}

	LandscapeContainer* landscapeContainer = m_landscapeEntries[i].m_createFunc( m_landscapeEntries[i].m_param );

	m_cachedLandscapes[i] = landscapeContainer;
	return landscapeContainer;

/*
	// Version which saves out landscape

	hkString fileName("resources/landscape/");
	fileName += m_landscapeEntries[i].m_name;
	hkIstream infile( fileName.cString() );

	hkpMoppBvTreeShape* preLoadedMopp = HK_NULL;

	hkPackfileReader* reader;

	if (infile.isOk())
	{
		reader = new hkBinaryPackfileReader();
		reader->loadEntireFile(infile.getStreamReader());

		// XXX - this currently doesnt work
		//			if( hkVersionUtil::updateToCurrentVersion( *reader, hkVersionRegistry::getInstance() ) != HK_SUCCESS )
		//{
		//				HK_ASSERT2(0, 0, "Couldn't update version.");
		//}


		hkRootLevelContainer* container = static_cast<hkRootLevelContainer*>( reader->getContents( hkRootLevelContainerClass.getName() ) );
		HK_ASSERT2(0xa6451543, container != HK_NULL, "Could not load root level obejct" );

		preLoadedMopp = static_cast<hkpMoppBvTreeShape*>( container->findObjectByType( hkpMoppBvTreeShapeClass.getName() ) );
		HK_ASSERT2(0xa6451544, preLoadedMopp != HK_NULL, "Could not find mopp in root level object" );
	}

	LandscapeContainer* landscapeContainer = m_landscapeEntries[i].m_createFunc( m_landscapeEntries[i].m_param, preLoadedMopp );

	if ( preLoadedMopp != HK_NULL )
	{
		landscapeContainer->m_packfileData = reader->getPackfileData();
		landscapeContainer->m_packfileData->addReference();
	}
	else
	{
		hkOstream outfile( fileName.cString() );
		hkRootLevelContainer::NamedVariant genericData(const_cast<char *>(hkpMoppBvTreeShapeClass.getName()), (void*)(landscapeContainer->m_shape), &hkpMoppBvTreeShapeClass);
		hkRootLevelContainer container;
		container.m_numNamedVariants = 1;
		container.m_namedVariants = &genericData;

		// Save landscape for next time

		hkpHavokSnapshot::ConvertListener convertListener;

		hkPackfileWriter* pw;
		pw = new hkBinaryPackfileWriter();
		pw->setContents( (const void*)&container, hkRootLevelContainerClass, &convertListener );

		hkPackfileWriter::Options o;
		pw->save( outfile.getStreamWriter(), o );
		pw->removeReference();
	}

	return landscapeContainer;
	*/
}


AabbSpawnUtil::AabbSpawnUtil( hkArray<hkAabb>& spawnVolumes )
:	m_spawnVolumes( spawnVolumes ),
	m_pseudoRandomGenerator(1098)
{
	m_spawnedAabbs.setSize(m_spawnVolumes.getSize());
	m_currentSpawnVolume = 0;
	m_allowOverlaps = false;
}

void AabbSpawnUtil::getNewSpawnPosition(const hkVector4& aabbDims, hkVector4& positionOut)
{
	// Try each volume in turn
	while (1)
	{
		hkVector4 avaliableDiag; avaliableDiag.setSub4( m_spawnVolumes[m_currentSpawnVolume].m_max, m_spawnVolumes[m_currentSpawnVolume].m_min );
		avaliableDiag.sub4(aabbDims);
		if ( avaliableDiag(0) < 0  || avaliableDiag(2) < 0 )
		{
			HK_ASSERT2(0, 0, "No spawn space large enough for requested aabb");
			return;
		}

		// Try 100 random positions in the volume
		int numTries = m_allowOverlaps ? 1 : 100;
		for (int j = 0; j < numTries; ++j)
		{
			hkVector4 minPos(m_pseudoRandomGenerator.getRandReal01(), m_pseudoRandomGenerator.getRandReal01(), m_pseudoRandomGenerator.getRandReal01() );
			
			if (avaliableDiag(1) < 0)
			{
				minPos(1) = 0;
			}

			minPos.mul4(avaliableDiag);
			minPos.add4(m_spawnVolumes[m_currentSpawnVolume].m_min);


			hkVector4 maxPos; maxPos.setAdd4( minPos, aabbDims );
			hkAabb candidate( minPos, maxPos );
			bool aabbCollides = false;

			if (!m_allowOverlaps)
			{
				for ( int k = 0; k < m_spawnedAabbs[m_currentSpawnVolume].getSize(); k++ )
				{
					if ( m_spawnedAabbs[m_currentSpawnVolume][k].overlaps(candidate))
					{
						aabbCollides = true;
						break;
					}
				}
			}
			if ( !aabbCollides )
			{
				m_spawnedAabbs[m_currentSpawnVolume].pushBack(candidate);
				hkVector4 position; positionOut.setInterpolate4( candidate.m_min, candidate.m_max, .5f );

				// If we allow penetrations, take each spawn volume in turn
				if ( m_allowOverlaps )
				{
					m_currentSpawnVolume++;
					if (m_currentSpawnVolume == m_spawnVolumes.getSize())
					{
						m_currentSpawnVolume = 0;
					}
				}
				return;
			}
		}
		// If we couldn't find a space, try the next volume
		m_currentSpawnVolume++;
		if (m_currentSpawnVolume == m_spawnVolumes.getSize())
		{
			m_currentSpawnVolume = 0;
			m_allowOverlaps = true;
		}
	}
}

LandscapeEntryRegister* LandscapeRepository::s_landscapeList = HK_NULL;
HK_SINGLETON_IMPLEMENTATION(LandscapeRepository);

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
