/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

/// A demo, which shows how to use the hkpSurfaceVelocityUtil.
///
/// Cubes are dropped through a funnel onto a conveyer belt modelled with a static geometry.
/// The movement of the conveyor belt is simulated by adding surface velocity to the object.

#ifndef HK_CONVEYORBELTS_H
#define HK_CONVEYORBELTS_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkVector4;


class ConveyorBeltsDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		ConveyorBeltsDemo(hkDemoEnvironment* env);
		~ConveyorBeltsDemo();

		struct Options
		{
			HK_DECLARE_REFLECTION();
			Options()
				:	m_shapeType(BOX),
					m_shapeScale(.55f),
					m_numBodies(200)
			{
			}

			enum ShapeType
			{
				BOX,
				CONVEX_VERTICES,
			};
			hkEnum<ShapeType,hkInt8> m_shapeType; //+default(0)
			hkReal m_shapeScale; //+default(.55f) +absmin(.1f)
			int m_numBodies; //+default(200), +absmin(1)
		};

		DEMO_OPTIONS_DECLARE(Options);

	protected:

		struct ConveyorBeltBlueprint
		{
			hkVector4	m_startPos;
			hkVector4	m_endPos;

			hkReal		m_width;
			hkReal		m_height;
			hkReal		m_thickness;
			hkReal		m_sideAngle;	// 0 = vertical
			hkReal		m_speed;
			hkReal		m_friction;
		};

		void createConveyorBelt( const ConveyorBeltBlueprint& bp );

};
#endif // HK_CONVEYORBELTS_H

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
