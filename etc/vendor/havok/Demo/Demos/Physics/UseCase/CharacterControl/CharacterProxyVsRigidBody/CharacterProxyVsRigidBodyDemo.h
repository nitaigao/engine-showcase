/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CHARACTER_PROXY_VS_RIGIDBODY_DEMO_H
#define HK_CHARACTER_PROXY_VS_RIGIDBODY_DEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkpCharacterProxy;
class hkpCharacterRigidBody;
class hkpCharacterContext;
class hkCharacterRbForLadderListener;
class hkCharacterProxyForLadderListener;
class hkpShapePhantom;
class hkLoader;

struct hkpSurfaceInfo;


class CharacterProxyVsRigidBodyDemo : public hkDefaultPhysicsDemo
{
public:

	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	CharacterProxyVsRigidBodyDemo(hkDemoEnvironment* env);

	~CharacterProxyVsRigidBodyDemo();

	// Collision Layers used in this demo.
	// By enabling and disabling collisions between these layers, we can tweak the behaviour of the demo
	// Disable collision between CHARACTER_PROXY and CHARACTER_RIGIDBODY
	enum
	{
		LAYER_LANDSCAPE = 1,
		LAYER_CHARACTER_PROXY = 2,
		LAYER_CHARACTER_RIGIDBODY = 3			
	};

	hkDemo::Result stepDemo();

	// Do Character proxy
	void doCharacterProxy();
	// Do Character rigid body
	void doCharacterRigidBody();

	void swapPhantomShape( hkpShape* newShape );

	void cameraHandling();	

protected:

	hkLoader* m_loader;

	// Control inputs
	hkReal m_posX;
	hkReal m_posY;
	hkReal m_currentAngle;
	hkQuaternion m_currentOrient;

	hkUint8	m_controllerSwitch;

	// Stand and crouch capsule shapes
	hkpShape* m_standShape;
	hkpShape* m_crouchShape;
	// Shape loaded from assets file
	hkpShape* m_newShape;

	// Character proxy
	hkpShapePhantom* m_phantom;
	hkpCharacterProxy* m_characterProxy;
	hkpCharacterContext* m_characterProxyContext;
	hkCharacterProxyForLadderListener* m_listenerProxy;

	// Character rigid body
	hkpCharacterRigidBody* m_characterRigidBody;
	hkpCharacterContext* m_characterRigidBodyContext;
	hkCharacterRbForLadderListener* m_listenerRigidBody;

	// Ground data filtering 
	hkpSurfaceInfo*	m_previousGround;
	hkInt32 m_framesInAir;

	// Objects ids for camera handling and transparency
	hkArray<hkUlong> m_objectIds;

};

#endif // HK_CHARACTER_PROXY_VS_RIGIDBODY_DEMO_H

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
