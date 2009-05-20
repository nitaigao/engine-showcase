/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MESHWELDINGDEMO_H
#define HK_MESHWELDINGDEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Common/Serialize/Packfile/hkPackfileReader.h>


class MeshWeldingDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		MeshWeldingDemo(hkDemoEnvironment* env);
		~MeshWeldingDemo();

		Result stepDemo(); 


	protected:

		hkpRigidBody* createStaticBox(const hkVector4& position, float friction, float restitution, hkpWorld* world) const;
		hkpRigidBody* createDynamicBox(const hkVector4& position, float mass, float friction, float restitution, hkpWorld* world) const;
		hkpRigidBody* loadAndInitSimpleMesh(const hkVector4& position, hkpWorld* world, hkPackfileReader::AllocatedData*& allocatedData);
		hkpRigidBody* loadRigidBodyFromXmlFile(const hkString& path, hkPackfileReader::AllocatedData*& allocatedData) const;

	protected:

		hkPackfileReader::AllocatedData* m_ringData;
		hkpRigidBody* m_movingBody;
		hkReal m_bodySpeed;
		hkArray<hkUint16> triangleList[2];
		hkpBoxShape* m_boxShape;
		hkpConvexShape* cxShapes[10];
};


#endif // HK_MESHWELDINGDEMO_H

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
