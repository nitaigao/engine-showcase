/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_ENTITY_TIME_QUEUE
#define HK_ENTITY_TIME_QUEUE

class hkpEntity;

#include <Common/Base/Thread/CriticalSection/hkCriticalSection.h>

	/// This queue allows you to store entities for a specified period of time.
	/// You specify the storage time upon the addition of an entity.
	/// With every call to stepDeltaTime, the queue is informed about the amount
	/// of time passed, and it returns entities, whose storage time has been exceeded.
class EntityTimeQueue
{
	public:
		EntityTimeQueue() : m_totalExtraTime(0.0f), m_criticalSection(1000) { }

		~EntityTimeQueue();

			/// Inserts a new body into the queue and sets its storage time
		void addEntity( hkpEntity* entity, hkReal storageTime );

			/// Steps time and returns entities that have exceeded their storage time.
			/// Note: The calling function 'owns' the returned entities and must
			/// call removeReference() on every entity in the entitiesOut array.
		void stepDeltaTime( hkReal deltaTime, hkArray<hkpEntity*>& entitiesOut );

	public:

		hkReal m_totalExtraTime;

		struct TemporaryEntityInfo
		{
			hkpEntity* m_entity;
			hkReal   m_extraTimeLeft;
		};

		hkArray<TemporaryEntityInfo> m_entityInfos;

			// Cleanup + init
		void resetTemporaryEntities( );

		hkCriticalSection m_criticalSection;

};

#endif // HK_ENTITY_TIME_QUEUE

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
