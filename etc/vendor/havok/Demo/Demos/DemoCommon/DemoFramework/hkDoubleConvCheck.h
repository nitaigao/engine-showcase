/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


/*
 * This enables our double precision math check for PS2.
 * If double check is enabled then we raise an assert if dptofp or fptodp
 * is called. This allows us to only check for double precision conversions
 * at simulation time - we allow doubles at construction time.
 *
 * This code reimplements dptofp and fptodp for Codewarrior on PS2 only.
 * The reimplementation is a software reimplementation.
 * calling these methods on any other platform/compiler has no effect.
 * The implementations are only overridden with software version in debug.
 */

	// Push and set state
void HK_CALL pushDoubleConversionCheck(hkBool enable);

	// Pop and restore state	
void HK_CALL popDoubleConversionCheck();


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
