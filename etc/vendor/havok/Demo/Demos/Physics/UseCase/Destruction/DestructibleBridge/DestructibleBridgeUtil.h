/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_DESTRUCTIBLE_BRIDGE_UTIL_H
#define HK_DESTRUCTIBLE_BRIDGE_UTIL_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
class hkpBreakOffPartsUtil;


	/// A utility class for the BridgesAndPlanes demo.
	/// Provides functions to build the scenery and objects.
class DestructibleBridgeUtil
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, DestructibleBridgeUtil);

		struct TerrainData
		{
			hkReal		m_maxHeight;
			int			m_numTiles;
			hkReal		m_canyonWidthRatio;
			hkBool		m_smoothBreak;
			hkVector4	m_scale;
			hkVector4	m_position;
			int			m_numBridges;
			hkReal		m_maxBreakOffImpulse;
		};

		struct AirplaneData
		{
			hkReal		m_wingSpan;
			hkVector4	m_position;
			hkVector4	m_destination;
			hkReal		m_velocity;
			hkReal		m_mass;
		};



	public:
			// create the landscape and flags all breakable objects breakable
			// terrainDataOut retutns a pointer to a referenced object holding all the terrain static mesh data
		static hkpRigidBody* createTerrainAndBridges( hkpBreakOffPartsUtil* breakOffUtil, const TerrainData& cInfo, hkReferencedObject*& terrainDataOut );

		static hkpRigidBody* createAirplane( const AirplaneData& info );

	private:

		DestructibleBridgeUtil();
		~DestructibleBridgeUtil();

	protected:
		static void         createBridge(hkArray<hkpConvexShape*>& shapes);

		static void createBridgeElementLevel0(const hkpConvexShape* beamShape, const hkpConvexShape* boardShape, const hkVector4& elementCenter, hkArray<hkpConvexShape*>& shapeArray);
		static void createBridgeElementLevel1(const hkpConvexShape* beamShape, const hkpConvexShape* boardShape, const hkVector4& elementCenter, hkArray<hkpConvexShape*>& shapeArray);
		static void createBridgeElementLevel2(const hkpConvexShape* beamShape, const hkpConvexShape* boardShape, const hkVector4& elementCenter, hkArray<hkpConvexShape*>& shapeArray);
		static void createSingleBoardOrBeam  (const hkpConvexShape* shape, const hkVector4& position, hkArray<hkpConvexShape*>& shapeArray);

		static hkpBoxShape* createBoxShape(hkReal width, hkReal height, hkReal length, hkReal scale);
		static void createConvexTranslateShapeAndAddToArray(hkpBoxShape* shape, hkReal posX, hkReal posY, hkReal posZ, hkReal scale, hkArray<hkpShape*>& shapes);
};



#endif // HK_DESTRUCTIBLE_BRIDGE_UTIL_H


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
