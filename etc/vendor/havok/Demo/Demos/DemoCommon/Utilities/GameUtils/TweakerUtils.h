/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DEMO2_TWEAKER_UTILS_H
#define HK_DEMO2_TWEAKER_UTILS_H

class hkClassMember;

// Displays and tweaks the values in a class
// Right now it supports the tweaking of ints, floats, bools and enums.
// Its generally used for game options.
// The member path leads to a specific part of the data .
// It is usally something like /myClassInstance/myPtrToClass/myEmbeddedClass/memberName
// Later we should extend this to understand specific types e.g.
// /myClassInstance/myPtrToClass/myEmbeddedClass/myArray/4
// /myClassInstance/myPtrToClass/myEmbeddedClass/myVector4/x
// /myClassInstance/myPtrToClass/myEmbeddedClass/myMatrix/1/2

class TweakerUtils
{
public:

	enum FloatTweakType
	{
		ADDITIVE,
		MULTIPLICATIVE
	};

	typedef hkBool (HK_CALL *HideMemberFunc)(const hkClassMember& m);

		// get the hkClass representing an instance, given the base class.
	static const hkClass* HK_CALL getInstanceClass( const hkClass* baseClass, const void* instance );

		// Traversal
	static hkString HK_CALL getNextSiblingPath(	const hkString& memberPath, const void* rootData, const class hkClass& rootKlass,
												HideMemberFunc hideMember=0 );

	static hkString HK_CALL getPrevSiblingPath(	const hkString& memberPath, const void* rootData, const class hkClass& rootKlass,
												HideMemberFunc hideMember=0 );

	static hkString HK_CALL getChildPath(	const hkString& memberPath, const void* rootData, const class hkClass& rootKlass,
												HideMemberFunc hideMember=0 );

	static hkString HK_CALL getParentPath(const hkString& memberPath, const void* rootData, const class hkClass& rootKlass);

		// Returns access to the class containing this member
	static hkString HK_CALL getClass(	const hkString& memberPath, const void* rootData, const class hkClass& rootKlass, 
										void*& data, const hkClass*& klass,
										HideMemberFunc hideMember=0 );

		// Manipulation
	static hkString HK_CALL tweakData(const hkString& memberPath, void* rootData, const hkClass& rootKlass, float delta = 0.0f, const hkReal threshold = 0.01f, FloatTweakType floatTweakType = MULTIPLICATIVE);

		// Manipulation
	static hkString HK_CALL clearData(const hkString& memberPath, void* rootData, const hkClass& rootKlass);

	static hkString HK_CALL setReal( const hkString& memberPath, void* rootData, const hkClass& rootKlass, float newValue );

		// Display
	static void HK_CALL displayData(	const hkString& memberPath, const void* rootData, const class hkClass& rootKlass, 
										class hkTextDisplay& disp, float x, float y,
										HideMemberFunc hideMember=0, hkUint32 selectedColor = 0xffffff00  );

	static void HK_CALL displayMemberData(	const hkString& memberPath, const void* rootData, const class hkClass& rootKlass, 
											class hkTextDisplay& disp, float x, float& y,
											HideMemberFunc hideMember=0, hkUint32 selectedColor = 0xffffff00 );

	static int HK_CALL getIndexOfMember( const hkString& memberPath, const void* rootData, const class hkClass& rootKlass, 
											class hkTextDisplay& disp, float x, float y,
											HideMemberFunc hideMember=0 );

	static int HK_CALL convertDisplayYPosToMemberIndex(hkReal yPos, class hkTextDisplay& disp, hkReal floatYOffset);

	static hkBool HK_CALL getTweakInput(hkDemoEnvironment* env, 
											const hkString& tweakName, void* tweakee, const class hkClass& klass,
											class hkTextDisplay& disp, const int xOffset, const int xWidth, const int yOffset,
											hkBool mouseInput, 
											int &downOut, int &upOut, hkBool &leftOut, hkBool &rightOut);
											
private:
	static void HK_CALL getIndexOfMemberRecursive( const hkString& memberPath, const void* rootData, const class hkClass& rootKlass, 
											class hkTextDisplay& disp, float x, float& y,
											HideMemberFunc hideMember=0 );

};

#endif //HK_DEMO2_TWEAKER_UTILS_H

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
