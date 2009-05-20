/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DEMO_GRAPH_H
#define HK_DEMO_GRAPH_H

class hkaDemoGraph
{

public:

	/// Draw a graph of real values
	static void draw( hkDemoEnvironment* env, hkReal bottom, hkReal left, hkReal top, hkReal right, hkUint32 color, const hkArray< hkReal >& ); 
	static void draw( hkDemoEnvironment* env, hkReal bottom, hkReal left, hkReal top, hkReal right, hkReal min, hkReal max, hkUint32 color, const hkArray< hkReal >& ); 

	// Draw a graph of boolean values
	static void draw( hkDemoEnvironment* env, hkReal bottom, hkReal left, hkReal top, hkReal right, hkUint32 color, const hkArray< hkBool >& , bool invert = false );

	static hkReal getMax( const hkArray< hkReal >& );
	static hkReal getMin( const hkArray< hkReal >& );

	/// \return (1.0-u)*a + a*b
	static hkReal linterp( hkReal a, hkReal b, hkReal u );

	/// \return (c-a)/(b-a)
	static hkReal invlinterp( hkReal a, hkReal b, hkReal c );

};

#endif // HK_DEMO_GRAPH_H

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
