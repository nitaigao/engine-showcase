/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_CONSTRAINT_PIVOTS_UTIL_H
#define HK_DYNAMICS2_CONSTRAINT_PIVOTS_UTIL_H

class hkpConstraintData;

	/// Simple utility for cloning of constraint datas (which reference motors).
class hkpConstraintPivotsUtil
{
	public:

			/// Get the constraint's pivot for body A.
		static const hkVector4& HK_CALL getPivotA(const hkpConstraintData* data);

			/// Get the constraint's pivot for body B.
		static const hkVector4& HK_CALL getPivotB(const hkpConstraintData* data);

};

#endif // HK_DYNAMICS2_CONSTRAINT_PIVOTS_UTIL_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20080925)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2008
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
