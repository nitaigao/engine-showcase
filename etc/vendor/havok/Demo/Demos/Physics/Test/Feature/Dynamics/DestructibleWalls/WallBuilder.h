/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_WALLBUILDER
#define HK_WALLBUILDER

#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Dynamics/Entity/hkpRigidBodyCinfo.h>
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>
#include <Demos/Physics/Api/Constraints/ConstraintKit2/hkFixedConstraint.h>

//class hkpConstraintData;
class hkpConstraintInstance;
class hkpBreakableConstraintData;
class hkpBoxShape;

struct BrickwallBuilderDescriptor
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, BrickwallBuilderDescriptor);

	BrickwallBuilderDescriptor():
		m_wallID(0),
		m_brickShape(HK_NULL),
		m_brickMass(.0f),
		m_setCollisionFilter(true),
		m_height(0),
		m_width(0),
		m_transform(hkQuaternion::getIdentity(), hkVector4::getZero()),
		/*m_position(hkVector4::getZero()),
		m_orientation(0,0,0,1),*/
		m_rowShifting(.0f),
		m_gapWidth(0.f),
		m_attachToGround(false),
		m_theGround(NULL),
		m_useConstraints(true),
		m_isBreakable(true),
		m_strength(100.0f),
		m_lowerThreshold(100.0f),
		m_brickExtents(1.0f, 1.0f, 1.0f)
	{
	}

	int m_wallID;
	// shape of the bricks
	hkpBoxShape* m_brickShape;
	// the mass of a brick
	hkReal m_brickMass;
	// true if a collision filter must be set
	hkBool m_setCollisionFilter;
	// Wall dimensions and orientation params
	int m_height;
	int m_width;
	hkTransform m_transform;
	//// position of the center of the wall's basis
	//hkVector4 m_position;
	//// orientation in space of the wall
	//hkQuaternion m_orientation;
	// if>0 rows are shifted in an alternate manner of the quantity provided
	hkReal m_rowShifting;
	// Gap between bricks along width
	hkReal m_gapWidth;
	// true if the first row af the wall must be attached to the ground
	hkBool m_attachToGround;
	// the ground, used if attachToGround==true
	hkpRigidBody* m_theGround;
	// Constraints params
	// true if bricks are constrained  
	hkBool m_useConstraints;
	// true if the bricks constraints must be breakable
	hkBool m_isBreakable;
	// if constraints are breakable, sets the threshold
	hkReal m_strength;
	// if != strength the brick constraints have a strength inversely proportional to the height. 
	// the highest one has threshold == m_lowerThreshold
	hkReal m_lowerThreshold;	
	// extents of the brick shape
	hkVector4 m_brickExtents;
};


// Class used to build walls made of bricks
class BrickWallBuilder 
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, BrickWallBuilder);

		// at least shape and dimensions must be provided to constructor
		BrickWallBuilder();
		~BrickWallBuilder();

		static void buildBrickWall( const BrickwallBuilderDescriptor& bwDescriptor, hkArray<hkpRigidBody*>& bricksOut, hkArray<hkpConstraintInstance*>& constraintsOut );

		static void computeBrickInfo(const hkpShape* shape, const hkReal& mass, hkpRigidBodyCinfo& boxInfo);
		
		//
		static hkFixedConstraintData* makeContraintData( hkpRigidBody* body1, hkpRigidBody* body2 );
		// Builds a constraint instance between the bodies
		static inline hkpConstraintInstance* buildConstraintInstance(hkpRigidBody* body1, hkpRigidBody* body2);
		// Builds a breakable constraint instance between the bodies
		static inline hkpConstraintInstance* buildBreakableConstraintInstance(hkpRigidBody* body1, hkpRigidBody* body2, const hkReal& threshold);
		// make a breakable constraint from constraint
		static hkpBreakableConstraintData* makeConstraintBreakable(hkpConstraintData* old, const hkReal& threshold);


	private:
		static inline void computeBrickPropertiesShape(const hkpShape* shape, const hkReal& mass, hkpMassProperties& result);

		static inline void computeBrickPropertiesBox(const hkpBoxShape* shape, const hkReal& mass, hkpMassProperties& result);

		static void setProperties(const hkpMassProperties& massProperties, hkpRigidBodyCinfo& boxInfo);
};

class StiffWallBuilder
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, StiffWallBuilder);

		StiffWallBuilder(){}
		~StiffWallBuilder(){}

		static inline hkpBoxShape* createBoxShape(int width, int height, const hkVector4& brickHalfExtents);
};

#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallBuilder.inl>

#endif

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
