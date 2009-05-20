/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Fracture/EntityTimeQueue.h>

#include <Physics/Dynamics/Entity/hkpEntity.h>
#include <Physics/Dynamics/World/hkpWorld.h>

	// <todo.aa make this an hkUtilityClass
void EntityTimeQueue::addEntity( hkpEntity* entity, hkReal storageTime )
{
	hkCriticalSectionLock lock(&m_criticalSection);
	
	TemporaryEntityInfo info;
	info.m_entity = entity;
	info.m_entity->addReference();
	if (storageTime >= m_totalExtraTime)
	{
		// Append on end. (The storageTime is longer than the remaining storage time of any object in the tempEntities array.)
		info.m_extraTimeLeft = storageTime - m_totalExtraTime;
		m_totalExtraTime = storageTime;
		m_entityInfos.pushBack(info);
	}
	else 
	{
		// Find position and insert. (So that each next object in the array has a longer remaining storageTime.)
		hkReal extraTime = m_totalExtraTime;
		
		int i; // index to insert after
		for (i = m_entityInfos.getSize() - 1; i >= 0 && storageTime < extraTime; i--)
		{
			extraTime -= m_entityInfos[i].m_extraTimeLeft;
		}

		// Found the index. Insert and update m_extraTimeLeft of the next body.
		int index = i + 1;

		info.m_extraTimeLeft = storageTime - extraTime;
		m_entityInfos[index].m_extraTimeLeft -= info.m_extraTimeLeft;

		HK_ASSERT2(0xad34ffbb, info.m_extraTimeLeft >= 0.0f && m_entityInfos[index].m_extraTimeLeft >= 0.0f, "All extra times must be non-negative.");

		m_entityInfos.insertAt(index, info);
	}
}

	// Warning: the calling function take ownership of the entities returned in enetitiesOut, and must
	// call removeReference() on them.
void EntityTimeQueue::stepDeltaTime( hkReal deltaTime, hkArray<hkpEntity*>& entitiesOut )
{
	if (m_entityInfos.getSize() && deltaTime)
	{
		m_totalExtraTime -= deltaTime;

		while (m_entityInfos.getSize() && deltaTime > 0.0f)
		{
			TemporaryEntityInfo& info = *m_entityInfos.begin();
			info.m_extraTimeLeft -= deltaTime;
			deltaTime = - info.m_extraTimeLeft;

			if (info.m_extraTimeLeft <= 0.0f )
			{
				entitiesOut.pushBack(info.m_entity);
				// todo - this could be made faster
				m_entityInfos.removeAtAndCopy(0);
			}
		}
	}
	else
	{
		m_totalExtraTime = 0.0f;
	}
}


void EntityTimeQueue::resetTemporaryEntities( )
{
	for (int i = 0; i < m_entityInfos.getSize(); i++)
	{
		m_entityInfos[i].m_entity->removeReference();
	}
	m_entityInfos.clear();
	m_totalExtraTime = 0.0f;
}



EntityTimeQueue::~EntityTimeQueue()
{
	resetTemporaryEntities();
}




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
