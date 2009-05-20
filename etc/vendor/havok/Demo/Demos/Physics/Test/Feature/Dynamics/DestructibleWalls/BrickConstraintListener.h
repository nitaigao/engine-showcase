/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_BRICKCONSTRAINTLISTENER_H
#define HK_BRICKCONSTRAINTLISTENER_H


#include <Physics/Dynamics/Constraint/hkpConstraintListener.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallSimulation.h>

class BrickConstraintListener : public hkReferencedObject, public hkpConstraintListener
{
public:

	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	virtual void constraintRemovedCallback( hkpConstraintInstance* constraint ) {}
	virtual ~BrickConstraintListener() {}
};

class BrickConstraintListenerEntities : public BrickConstraintListener
{
public:
	BrickConstraintListenerEntities();
	virtual ~BrickConstraintListenerEntities();

	//// Overloading base class

	///Called when a constraint is removed from the world.
	virtual void constraintRemovedCallback( hkpConstraintInstance* constraint );
};


class BrickConstraintListenerNotify : public BrickConstraintListener
{
public:
	BrickConstraintListenerNotify(WallSimulation* simulation);
	virtual ~BrickConstraintListenerNotify(){}

	//// Overloading base class

	///Called when a constraint is removed from the world.
	virtual void constraintRemovedCallback( hkpConstraintInstance* constraint );
private:
	WallSimulation* m_simulation;
};


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
