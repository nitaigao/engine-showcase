/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// A demo which shows how to use the Vehicle API to construct a very basic car with (untuned) 
// parameters, including an Action used to update the car, and a simple controller for driving it.
// The constructor creates a car and a landscape to drive on.
// The stepDemo() updates the car based on user input (steering, acceleration etc.), and updates the
// camera to follow the car.

#ifndef HK_RoundTrackDemo_H
#define HK_RoundTrackDemo_H


class hkpMoppCode;
class hkpMoppBvTreeShape;

#include <Common/Base/hkBase.h>


	
class RoundTrackDemo: public hkReferencedObject
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		RoundTrackDemo();
		~RoundTrackDemo();

		// This helper function facilitates construction of a "ground" shape on which to drive.
		hkpMoppBvTreeShape* createMoppShape();

	private:

		//
		//	Static data for the landscape
		//
		enum { SIDEA = 300 };
		enum { SIDEB = 8 };
		enum { NUM_VERTICES = SIDEA * SIDEB };
		enum { NUM_TRIANGLES = 2 * (SIDEA-1) * (SIDEB-1) };

		float		m_vertices[NUM_VERTICES * 3];
		hkUint16	m_indices[NUM_TRIANGLES * 3];

};

#endif //HK_RoundTrackDemo_H


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
