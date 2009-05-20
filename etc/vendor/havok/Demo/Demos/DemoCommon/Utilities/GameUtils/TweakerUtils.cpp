/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/GameUtils/TweakerUtils.h>

#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkClassMember.h>

#include <Graphics/Common/Font/hkgFont.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>

#include <Common/Serialize/Util/hkBuiltinTypeRegistry.h>
#include <Common/Base/Reflection/Registry/hkVtableClassRegistry.h>

#define NOT(A) (!(A))

template <typename T>
T& lookupMember(void* start)
{
	return *reinterpret_cast<T*>( start );
}

template <typename T>
const T& lookupMember(const void* start)
{
	return *reinterpret_cast<const T*>( start );
}

const hkClass* TweakerUtils::getInstanceClass( const hkClass* baseClass, const void* instance )
{
	if ( ( instance != 0 ) && baseClass->hasVtable() )
	{
		const hkClass* c = hkBuiltinTypeRegistry::getInstance().getVtableClassRegistry()->getClassFromVirtualInstance( instance );

		if ( c )
		{
			return c;
		}
	}

	return baseClass;
}

void TweakerUtils::displayMemberData(	const hkString& memberPath, const void* rootData, const class hkClass& rootKlass,
										class hkTextDisplay& disp, float x, float& y,
										HideMemberFunc hideMember, hkUint32 selectedColor )
{
	if (memberPath.getLength() == 0)
		return;

	hkString path  = memberPath;

	// Strip leading '/'
	while (path[0] == '/')
		path = path.substr(1);

	// Find next '/'
	int idx = path.indexOf('/');
	if (idx < 0) idx = path.getLength();

	int leftBracketIndex = path.indexOf('[');
	if ( leftBracketIndex < 0 ) leftBracketIndex = path.getLength();

	hkBool isArray = leftBracketIndex < idx;
	hkString memberName;
	int arrayIndex = -1;

	if ( isArray )
	{
		int rightBracketIndex = path.indexOf(']');
		hkString indexString = path.substr( leftBracketIndex+1, rightBracketIndex - leftBracketIndex - 1 );
		arrayIndex = hkString::atoi( indexString.cString() );
		memberName = path.substr(0, leftBracketIndex );
		path = path.substr(rightBracketIndex+1);
	}
	else
	{
		memberName = path.substr(0, idx);
		path = path.substr(idx); // now holds remaining path
	}

	//Show the class
	for (int memberIndex = 0; memberIndex < rootKlass.getNumMembers(); ++memberIndex )
	{
		const hkClassMember& member = rootKlass.getMember(memberIndex);

		if ( hideMember && hideMember(member) )
		{
			continue;
		}

		bool isSelected = hkString::strCmp(member.getName(), memberName.cString()) == 0;
		hkUint32 col = (isSelected) ? selectedColor : 0xffffffff;

		const void* memberData = static_cast<const void*>(static_cast<const char*>(rootData) + member.getOffset());

		hkString str;
		switch (member.getType())
		{
		case hkClassMember::TYPE_REAL :
			{
				hkReal value = lookupMember<hkReal>(memberData);
				str.printf("%s: %f\n", member.getName(), value);
			}
			break;
		case hkClassMember::TYPE_INT32 :
			{
				hkInt32 value = lookupMember<hkInt32>(memberData);
				str.printf("%s: %d\n", member.getName(), value);
			}
			break;
		case hkClassMember::TYPE_UINT32 :
			{
				hkUint32 value = lookupMember<hkUint32>(memberData);
				str.printf("%s: %u\n", member.getName(), value);
			}
			break;
		case hkClassMember::TYPE_INT16 :
			{
				hkInt16 value = lookupMember<hkInt16>(memberData);
				str.printf("%s: %d\n", member.getName(), value);
			}
			break;
		case hkClassMember::TYPE_UINT16 :
			{
				hkUint16 value = lookupMember<hkUint16>(memberData);
				str.printf("%s: %u\n", member.getName(), value);
			}
			break;
		case hkClassMember::TYPE_BOOL :
			{
				bool value = lookupMember<bool>(memberData);
				str.printf("%s: %s\n", member.getName(), (value) ? "True" : "False");
			}
			break;
		case hkClassMember::TYPE_VECTOR4:
		case hkClassMember::TYPE_QUATERNION:
			{
				const hkReal* r = reinterpret_cast<const hkReal*>(memberData);
				str.printf("(%f %f %f %f)", r[0], r[1], r[2], r[3] );
				break;
			}
			break;
		case hkClassMember::TYPE_ENUM :
			{
				const hkClassEnum& e = member.getEnumType();
				int value = member.getEnumValue(memberData);
				const char* valueName = HK_NULL;
				if( e.getNameOfValue( value, &valueName) == HK_SUCCESS )
				{
					str.printf("%s: %s\n", member.getName(), valueName);
				}
			}
			break;
		case hkClassMember::TYPE_STRUCT :
			{
				str.printf("%s >\n", member.getName());
			}
			break;
		case hkClassMember::TYPE_POINTER:
			{
				const void* ptrAddr = static_cast<const void*>(memberData);
				void* ptrTo = *static_cast<void*const*>(ptrAddr);

				if ( ptrTo == HK_NULL )
				{
					str.printf("%s -> null\n", member.getName() );
				}
				else
				{
					switch( member.getSubType() )
					{
						case hkClassMember::TYPE_CHAR:
						{
							str.printf("%s -> \"%s\"\n", member.getName(), ptrTo );

							break;
						}
						case hkClassMember::TYPE_STRUCT:
						{
							const hkClass& structClass = member.getStructClass();
							const char* className = getInstanceClass( &structClass, ptrTo )->getName();

							str.printf("%s -> <%s>", member.getName(), className );

							break;
						}
						default:
						{
							str.printf("%s -> ???\n", member.getName() );

							break;
						}
					}
				}
			}
			break;
		case hkClassMember::TYPE_ARRAY :
			{
				const void* ptrAddr = static_cast<const void*>(memberData);
				const hkArray<void*>* arrayPtr = static_cast<const hkArray<void*>*>( ptrAddr );
				int arraySz = arrayPtr->getSize();
				str.printf("%s[0..%d]\n", member.getName(), arraySz-1 );
			}
			break;
		case hkClassMember::TYPE_CSTRING:
			{
				const void* ptrAddr = static_cast<const void*>(memberData);
				void* ptrTo = *static_cast<void*const*>(ptrAddr);

				if ( ptrTo == HK_NULL )
				{
					str.printf("%s -> null\n", member.getName() );
				}
				else
				{
					str.printf("%s -> \"%s\"\n", member.getName(), ptrTo );
				}
			}
			break;
		case hkClassMember::TYPE_FLAGS:
			{
				str.printf("%s -> [", member.getName() );
				const hkClassEnum& cenum = member.getEnumType();
				hkArray<const char*> bits;
				int leftOvers;
				cenum.decomposeFlags( member.getEnumValue(memberData), bits, leftOvers );
				for( int i = bits.getSize() - 1; i >= 0; --i )
				{
					str += bits[i];
					if( i != 0 ) str += " | ";
				}
				if( leftOvers )
				{
					hkString tmp;
					tmp.printf("%s%s %x", str.cString(), str.getLength() ? " | " : "", leftOvers );
					str = tmp;
				}
				str += "]";
			}
			break;
		default :
			{
				str.printf("%s\n", member.getName());
			}
			break;
		}

		disp.outputText(str, x, y, col);
		y += disp.getFont()->getCharHeight() * 1.2f;

		// Chase structs
		if ((isSelected) && (member.getType() == hkClassMember::TYPE_STRUCT))
		{
			const void* ptrAddr = static_cast<const void*>(memberData);
			displayMemberData(path, ptrAddr, member.getStructClass(), disp, x+20, y, hideMember);
		}

		// Chase pointers
		if (isSelected && (member.getType() == hkClassMember::TYPE_POINTER) && (member.getSubType() == hkClassMember::TYPE_STRUCT))
		{
			const void* ptrAddr = static_cast<const void*>(memberData);
			void* ptrTo = *static_cast<void*const*>(ptrAddr);

			if ( ptrTo )
			{
				const hkClass* instanceClass = getInstanceClass( &member.getStructClass(), ptrTo );

				displayMemberData(path, ptrTo, *instanceClass, disp, x+20, y, hideMember);
			}
		}

		// Chase arrays
		if ( isArray && isSelected && (member.getType() == hkClassMember::TYPE_ARRAY))
		{
			switch( member.getSubType() )
			{
			case hkClassMember::TYPE_REAL :
				{
					const void* ptrAddr = static_cast<const void*>(memberData);
					const hkArray<hkReal>* arrayPtr = static_cast<const hkArray<hkReal>*>( ptrAddr );
					int arraySz = arrayPtr->getSize();

					for( int j = 0; j < arraySz; j++ )
					{
						hkReal value = (*arrayPtr)[j];

						hkString s;
						s.printf( "%s[%d]: %f", memberName.cString(), j, value );

						int color = ( j == arrayIndex ? selectedColor : 0xffffffff );
						disp.outputText(s, x+20, y, color);
						y += disp.getFont()->getCharHeight() * 1.2f;
					}

					break;
				}
			case hkClassMember::TYPE_INT32 :
				{
					const void* ptrAddr = static_cast<const void*>(memberData);
					const hkArray<hkInt32>* arrayPtr = static_cast<const hkArray<hkInt32>*>( ptrAddr );
					int arraySz = arrayPtr->getSize();

					for( int j = 0; j < arraySz; j++ )
					{
						hkInt32 value = (*arrayPtr)[j];

						hkString s;
						s.printf( "%s[%d]: %d", memberName.cString(), j, value );

						int color = ( j == arrayIndex ? selectedColor : 0xffffffff );
						disp.outputText(s, x+20, y, color);
						y += disp.getFont()->getCharHeight() * 1.2f;
					}

					break;
				}
			case hkClassMember::TYPE_POINTER:
				{
					const void* ptrAddr = static_cast<const void*>(memberData);
					const hkArray<void*>* arrayPtr = static_cast<const hkArray<void*>*>( ptrAddr );
					int arraySz = arrayPtr->getSize();

					for( int j = 0; j < arraySz; j++ )
					{
						void* ptrTo = (*arrayPtr)[j];
						const hkClass* instanceClass = getInstanceClass( &member.getStructClass(), ptrTo );

						const char* instanceClassName = ( ptrTo ? instanceClass->getName() : "null" );

						hkString s;
						s.printf( "%s[%d] -> <%s>", memberName.cString(), j, instanceClassName );

						if ( j == arrayIndex )
						{
							disp.outputText(s, x+20, y, selectedColor);
							y += disp.getFont()->getCharHeight() * 1.2f;

							if ( ptrTo )
							{
								displayMemberData(path, ptrTo, *instanceClass, disp, x+40, y, hideMember);
							}
						}
						else
						{
							disp.outputText(s, x+20, y, 0xffffffff);
							y += disp.getFont()->getCharHeight() * 1.2f;
						}
					}

					break;
				}
			case hkClassMember::TYPE_STRUCT:
				{
					const void* ptrAddr = static_cast<const void*>(memberData);
					const hkArray<char>* arrayPtr = static_cast<const hkArray<char>*>( ptrAddr );
					int arraySz = arrayPtr->getSize();
					int structSz = member.getArrayMemberSize();

					for( int j = 0; j < arraySz; j++ )
					{
						hkString s;
						s.printf( "%s[%d]", memberName.cString(), j );

						if ( j == arrayIndex )
						{
							disp.outputText(s, x+20, y, selectedColor);
							y += disp.getFont()->getCharHeight() * 1.2f;
							displayMemberData(path, arrayPtr->begin() + j*structSz, member.getStructClass(), disp, x+40, y, hideMember );
						}
						else
						{
							disp.outputText(s, x+20, y, 0xffffffff);
							y += disp.getFont()->getCharHeight() * 1.2f;
						}
					}

					break;
				}
			default:
				{
					break;
				}
			}
		}
	}
}

int TweakerUtils::convertDisplayYPosToMemberIndex(hkReal yPos, class hkTextDisplay& disp, hkReal floatYOffset)
{
	return (int)  ((yPos- floatYOffset + 0.001f) / (disp.getFont()->getCharHeight() * 1.2f)) ;
}


int TweakerUtils::getIndexOfMember(	const hkString& memberPath, const void* rootData, const class hkClass& rootKlass,
										class hkTextDisplay& disp, float x, float y,
										HideMemberFunc hideMember )
{
	hkReal yOut = y;
	getIndexOfMemberRecursive(memberPath, rootData, rootKlass,
										 disp, x, yOut,
										 hideMember );

	int	index = convertDisplayYPosToMemberIndex(yOut, disp, y); 

	return index;
}


// TODO: Replace with getMemberIdx calls (since current implementation below actually not recursive??).
void TweakerUtils::getIndexOfMemberRecursive(	const hkString& memberPath, const void* rootData, const class hkClass& rootKlass,
										class hkTextDisplay& disp, float x, float& y,
										HideMemberFunc hideMember )
{
	if (memberPath.getLength() == 0)
		return;

	hkString path  = memberPath;

	// Strip leading '/'
	while (path[0] == '/')
		path = path.substr(1);

	// Find next '/'
	int idx = path.indexOf('/');
	if (idx < 0) idx = path.getLength();

	int leftBracketIndex = path.indexOf('[');
	if ( leftBracketIndex < 0 ) leftBracketIndex = path.getLength();

	hkBool isArray = leftBracketIndex < idx;
	hkString memberName;
	int arrayIndex = -1;

	if ( isArray )
	{
		int rightBracketIndex = path.indexOf(']');
		hkString indexString = path.substr( leftBracketIndex+1, rightBracketIndex - leftBracketIndex - 1 );
		arrayIndex = hkString::atoi( indexString.cString() );
		memberName = path.substr(0, leftBracketIndex );
		path = path.substr(rightBracketIndex+1);
	}
	else
	{
		memberName = path.substr(0, idx);
		path = path.substr(idx); // now holds remaining path
	}

	//Show the class
	for (int memberIndex = 0; memberIndex < rootKlass.getNumMembers(); ++memberIndex )
	{
		const hkClassMember& member = rootKlass.getMember(memberIndex);

		if ( hideMember && hideMember(member) )
		{
			continue;
		}

		bool isSelected = hkString::strCmp(member.getName(), memberName.cString()) == 0;

		if(isSelected) 
		{
			return;
		}

		y += disp.getFont()->getCharHeight() * 1.2f;
	}
}

int getMemberIdx( const hkString& memberName, const hkClass& klass)
{
	int memberIdx = -1;
	for (int i=0; i < klass.getNumMembers(); i++)
	{
		const hkClassMember& member = klass.getMember(i);
		const hkString name(member.getName());
		if ( memberName == name )
		{
			memberIdx = i;
		}
	}
	return memberIdx;
}

hkString TweakerUtils::getClass(	const hkString& memberPath, const void* rootData, const class hkClass& rootKlass,
									void*& data, const hkClass*& klass,
									HideMemberFunc hideMember )
{
	hkString path = memberPath;

	// Strip leading '/'
	while (path[0] == '/')
		path = path.substr(1);

	// Empty string
	if (path.getLength() == 0)
	{
		data = const_cast<void*>(rootData);
		klass = &rootKlass;
		return hkString("/");
	}

	// Find next '/'
	int idx = path.indexOf('/');
	if (idx < 0) idx = path.getLength();

	// find next '['

	int leftBracketIndex = path.indexOf( '[' );
	if (leftBracketIndex < 0) leftBracketIndex = path.getLength();

	hkBool isArray = ( leftBracketIndex < idx );
	hkString memberName;
	int arrayIndex = -1;
	hkString memberNameAndIndex;

	if ( isArray )
	{
		int rightBracketIndex = path.indexOf(']');
		hkString indexString = path.substr( leftBracketIndex+1, rightBracketIndex - leftBracketIndex - 1 );
		arrayIndex = hkString::atoi( indexString.cString() );
		memberName = path.substr(0, leftBracketIndex );
		memberNameAndIndex = path.substr(0, rightBracketIndex+1);
		path = path.substr(rightBracketIndex+1);
	}
	else
	{
		memberName = path.substr(0, idx);
		path = path.substr(idx); // now holds remaining path
	}

	// Check the klass has this member
	int memberIdx = getMemberIdx( memberName, rootKlass);

	// Member not found so return
	if (memberIdx == -1)
	{
		data = const_cast<void*>(rootData);
		klass = &rootKlass;
		return hkString("");
	}

	const hkClassMember& member = rootKlass.getMember( memberIdx );

	// Chase pointers to structs / classes

	if (path.getLength() > 0)
	{
		if (member.getType() == hkClassMember::TYPE_STRUCT)
		{
			const void* ptrAddr = static_cast<const char*>(rootData) + member.getOffset();
			return hkString("/") + memberName +
				TweakerUtils::getClass(path, ptrAddr, member.getStructClass(), data, klass);
		}
		else if (( member.getType() ==  hkClassMember::TYPE_POINTER) && (member.getSubType() == hkClassMember::TYPE_STRUCT))
		{
			const void* ptrAddr = static_cast<const char*>(rootData) + member.getOffset();
			const void* ptrTo = *static_cast<const void*const*>(ptrAddr);
			const hkClass* instanceClass = getInstanceClass( &member.getStructClass(), ptrTo );

			return hkString("/") + memberName +
				TweakerUtils::getClass(path, ptrTo, *instanceClass, data, klass);
		}
		else if (( member.getType() == hkClassMember::TYPE_ARRAY ) && isArray )
		{
			if ( member.getSubType() == hkClassMember::TYPE_STRUCT )
			{
				const void* ptrAddr = static_cast<const char*>(rootData) + member.getOffset();
				const hkArray<char>* arrayPtr = static_cast<const hkArray<char>*>( ptrAddr );
				int sz = member.getArrayMemberSize();
				const void* memberData = arrayPtr->begin() + sz*arrayIndex;

				return hkString("/") + memberNameAndIndex +
					TweakerUtils::getClass(path, memberData, member.getStructClass(), data, klass);
			}
			else if ( member.getSubType() == hkClassMember::TYPE_POINTER )
			{
				const void* arrayData = static_cast<const void*>(static_cast<const char*>(rootData) + member.getOffset());
				const hkArray<void*>* arrayPtr = static_cast<const hkArray<void*>*>( arrayData );
				const void* ptrTo = *(arrayPtr->begin() + arrayIndex);
				const hkClass* instanceClass = getInstanceClass( &member.getStructClass(), ptrTo );

				return hkString("/") + memberNameAndIndex +
						TweakerUtils::getClass(path, ptrTo, *instanceClass, data, klass);
			}
			else
			{
				return memberPath;
			}
		}
	}

	// Return this class
	data = const_cast<void*>(rootData);
	klass = &rootKlass;
	return hkString("/");
}

hkString TweakerUtils::setReal( const hkString& memberPath, void* rootData, const hkClass& rootKlass, float newValue )
{
	if (memberPath.getLength() == 0)
		return hkString("/");

	const hkClass* klass;
	void* data;
	hkString classPath = TweakerUtils::getClass(memberPath, rootData, rootKlass, data, klass);

	hkString memberName = memberPath;
	if (classPath.getLength() > 0)
		memberName = memberName.replace(classPath, hkString(""));

	// deal with arrays

	int rightBracketIndex = memberName.indexOf( ']' );
	int arrayIndex = -1;

	if ( rightBracketIndex != -1 )
	{
		int leftBracketIndex = memberName.indexOf( '[' );
		hkString indexString = memberName.substr( leftBracketIndex + 1, rightBracketIndex - leftBracketIndex - 1 );
		arrayIndex = hkString::atoi( indexString.cString() );
		memberName = memberName.substr( 0, leftBracketIndex );
	}

	int memberIdx = getMemberIdx(memberName, *klass);
	if (memberIdx < 0)
		return classPath;

	const hkClassMember& member = klass->getMember(memberIdx);
	void* memberData = static_cast<void*>(static_cast<char*>(data) + member.getOffset());

	hkString str;
	switch (member.getType())
	{
	case hkClassMember::TYPE_REAL :
		{
			hkReal& value = lookupMember<hkReal>(memberData);

			value = newValue;
		}
		break;
	case hkClassMember::TYPE_INT32 :
		{
			hkInt32& value = lookupMember<hkInt32>(memberData);
			value = hkInt32(newValue);
		}
		break;
	case hkClassMember::TYPE_UINT32 :
		{
			hkUint32& value = lookupMember<hkUint32>(memberData);
			value = hkUint32(newValue);
		}
		break;
	case hkClassMember::TYPE_INT16 :
		{
			hkInt16& value = lookupMember<hkInt16>(memberData);
			value = hkInt16(newValue);
		}
		break;
	case hkClassMember::TYPE_UINT16 :
		{
			hkUint16& value = lookupMember<hkUint16>(memberData);
			value = hkUint16(newValue);
		}
		break;
	case hkClassMember::TYPE_BOOL :
		{
			bool& value = lookupMember<bool>(memberData);
			value = (newValue!=0.0f);
		}
		break;
	case hkClassMember::TYPE_ENUM :
		{
			// for backward compatibility we clear the data on zero, otherwise do nothing to enums
			if ( newValue == 0.0f )
			{
				const hkClassEnum& e = member.getEnumType();
				member.setEnumValue(memberData, e.getItem(0).getValue());
			}
		}
		break;
	case hkClassMember::TYPE_ARRAY:
		{
			switch( member.getSubType() )
			{
			case hkClassMember::TYPE_REAL :
				{
					void* ptrAddr = static_cast<void*>(memberData);
					hkArray<hkReal>* arrayPtr = static_cast<hkArray<hkReal>*>( ptrAddr );
					(*arrayPtr)[arrayIndex] = newValue;
				}
				break;
			case hkClassMember::TYPE_INT32 :
				{
					void* ptrAddr = static_cast<void*>(memberData);
					hkArray<hkInt32>* arrayPtr = static_cast<hkArray<hkInt32>*>( ptrAddr );
					(*arrayPtr)[arrayIndex] = static_cast<hkInt32>(newValue);
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}

	return memberPath;
}

static hkInt32 convertTweakerFloatToInt(float f)
{
	// If between -1 and 1, set to 1 or -1
	if ( hkMath::fabs(f) < 1.0f)
	{
		return (f > 0) ? 1 : -1;
	}

	// Else round towards zero
	return hkInt32(f);
}


hkString TweakerUtils::tweakData(const hkString& memberPath, void* rootData, const hkClass& rootKlass, float offset, const hkReal threshold, FloatTweakType floatTweakType)
{
	if (memberPath.getLength() == 0)
		return hkString("/");

	const hkClass* klass;
	void* data;
	hkString classPath = TweakerUtils::getClass(memberPath, rootData, rootKlass, data, klass);

	hkString memberName = memberPath;
	if (classPath.getLength() > 0)
		memberName = memberName.replace(classPath, hkString(""));

	// deal with arrays

	int rightBracketIndex = memberName.indexOf( ']' );
	int arrayIndex = -1;

	if ( rightBracketIndex != -1 )
	{
		int leftBracketIndex = memberName.indexOf( '[' );
		hkString indexString = memberName.substr( leftBracketIndex + 1, rightBracketIndex - leftBracketIndex - 1 );
		arrayIndex = hkString::atoi( indexString.cString() );
		memberName = memberName.substr( 0, leftBracketIndex );
	}

	int memberIdx = getMemberIdx(memberName, *klass);
	if (memberIdx < 0)
		return classPath;

	const hkClassMember& member = klass->getMember(memberIdx);
	void* memberData = static_cast<void*>(static_cast<char*>(data) + member.getOffset());

	hkString str;
	switch (member.getType())
	{
	case hkClassMember::TYPE_REAL :
		{

			switch(floatTweakType)
			{
				case MULTIPLICATIVE:
				{
					hkReal& value = lookupMember<hkReal>(memberData);
					value *= (value > 0) ? 1.0f + offset : 1.0f - offset;


					// On 0 snap to +ve or -ve
					if ( value == 0.0f )
					{
						value = (offset > 0) ? threshold : -threshold;
					}

					// Snap to 0 exactly
					if (hkMath::fabs(value) < threshold)
					{
						value = 0.0f;
					}
					break;
				}
				case ADDITIVE:
				{
					hkReal& value = lookupMember<hkReal>(memberData);
					value += offset;					
				}

			}
		}
		break;
	case hkClassMember::TYPE_INT32 :
		{
			hkInt32& value = lookupMember<hkInt32>(memberData);
			hkInt32 offsetAsInt = convertTweakerFloatToInt(offset);
			value += offsetAsInt;
		}
		break;
	case hkClassMember::TYPE_UINT32 :
		{
			hkUint32& value = lookupMember<hkUint32>(memberData);
			hkInt32 offsetAsInt = convertTweakerFloatToInt(offset);
			value += offsetAsInt;
		}
		break;
	case hkClassMember::TYPE_INT16 :
		{
			hkInt16& value = lookupMember<hkInt16>(memberData);
			hkInt32 offsetAsInt = convertTweakerFloatToInt(offset);
			value = hkInt16( value + offsetAsInt);
		}
		break;
	case hkClassMember::TYPE_UINT16 :
		{
			hkUint16& value = lookupMember<hkUint16>(memberData);
			hkInt32 offsetAsInt = convertTweakerFloatToInt(offset);
			value = hkUint16( value + offsetAsInt);
		}
		break;
	case hkClassMember::TYPE_BOOL :
		{
			bool& value = lookupMember<bool>(memberData);
			value = !value;
		}
		break;
	case hkClassMember::TYPE_ENUM :
		{
			const hkClassEnum& e = member.getEnumType();
			int value = member.getEnumValue(memberData);

			// Find item with current value
			int itemIdx = 0;
			while ( e.getItem(itemIdx).getValue() != value)
				itemIdx++;

			itemIdx += (offset > 0) ? 1 : e.getNumItems() -1;
			itemIdx = itemIdx % e.getNumItems();

			member.setEnumValue(memberData, e.getItem(itemIdx).getValue());
		}
		break;
	case hkClassMember::TYPE_ARRAY :
		{
			switch( member.getSubType() )
			{
				case hkClassMember::TYPE_INT32 :
				{
					void* ptrAddr = static_cast<void*>(memberData);
					hkArray<hkInt32>* arrayPtr = static_cast<hkArray<hkInt32>*>( ptrAddr );
					(*arrayPtr)[arrayIndex] += (offset > 0) ? 1 : -1;
					break;
				}
				default:
				{
					break;
				}
			}
		}
		break;
	default:
		break;
	}

	return memberPath;
}


hkString TweakerUtils::clearData(const hkString& memberPath, void* rootData, const hkClass& rootKlass)
{
	if (memberPath.getLength() == 0)
		return hkString("/");

	const hkClass* klass;
	void* data;
	hkString classPath = TweakerUtils::getClass(memberPath, rootData, rootKlass, data, klass);

	hkString memberName = memberPath;
	if (classPath.getLength() > 0)
		memberName = memberName.replace(classPath, hkString(""));

	int memberIdx = getMemberIdx(memberName, *klass);
	if (memberIdx < 0)
		return classPath;

	const hkClassMember& member = klass->getMember(memberIdx);
	void* memberData = static_cast<void*>(static_cast<char*>(data) + member.getOffset());

	hkString str;
	switch (member.getType())
	{
	case hkClassMember::TYPE_REAL :
		{
			hkReal& value = lookupMember<hkReal>(memberData);
			value = 0.0f;
		}
		break;
	case hkClassMember::TYPE_INT32 :
		{
			hkInt32& value = lookupMember<hkInt32>(memberData);
			value = 0;
		}
		break;
	case hkClassMember::TYPE_UINT32 :
		{
			hkUint32& value = lookupMember<hkUint32>(memberData);
			value = 0;
		}
		break;
	case hkClassMember::TYPE_BOOL :
		{
			bool& value = lookupMember<bool>(memberData);
			value = static_cast<bool>(0);
		}
		break;
	case hkClassMember::TYPE_ENUM :
		{
			const hkClassEnum& e = member.getEnumType();
			member.setEnumValue(memberData, e.getItem(0).getValue());
		}
		break;
	default:
		break;
	}

	return memberPath;
}

void HK_CALL TweakerUtils::displayData(	const hkString& memberPath, const void* rootData, const class hkClass& rootKlass,
								class hkTextDisplay& disp, float x, float y,
								HideMemberFunc hideMember, hkUint32 selectedColor )
{
	hkReal yCopy = y;
	displayMemberData(memberPath, rootData, rootKlass, disp, x, yCopy, hideMember, selectedColor);
}

hkString TweakerUtils::getNextSiblingPath(	const hkString& memberPath, const void* rootData, const class hkClass& rootKlass,
										HideMemberFunc hideMember )
{
	if (memberPath.getLength() == 0)
		return hkString("/");

	const hkClass* klass;
	void* data;
	hkString classPath = TweakerUtils::getClass(memberPath, rootData, rootKlass, data, klass);

	hkString memberName = memberPath;
	if (classPath.getLength() > 0)
		memberName = memberName.replace(classPath, hkString(""));

	int leftBracketIndex = memberName.indexOf('[');
	int arrayIndex = -1;

	if ( leftBracketIndex != -1 )
	{
		int rightBracketIndex = memberName.indexOf(']');
		hkString indexString = memberName.substr( leftBracketIndex+1, rightBracketIndex - leftBracketIndex - 1 );
		arrayIndex = hkString::atoi( indexString.cString() );
		memberName = memberName.substr( 0, leftBracketIndex );
	}

	int memberIdx = getMemberIdx(memberName, *klass);

	if ( (memberIdx != -1) && (arrayIndex != -1) )
	{
		const hkClassMember& member = klass->getMember(memberIdx);

		if ( member.getType() == hkClassMember::TYPE_ARRAY )
		{
			void* memberData = static_cast<void*>(static_cast<char*>(data) + member.getOffset());
			hkArray<char>* arrayPtr = static_cast<hkArray<char>*>(memberData);
			int sz = arrayPtr->getSize();

			int newIndex = (arrayIndex + 1) % sz;

			hkString s;
			s.printf( "%s%s[%d]", classPath.cString(), memberName.cString(), newIndex );
			return s;
		}
	}

	do
	{
		memberIdx = ( memberIdx + 1 ) % klass->getNumMembers();
	} while ( hideMember && hideMember(klass->getMember(memberIdx)) );

	return classPath + hkString( klass->getMember(memberIdx).getName());
}

hkString TweakerUtils::getPrevSiblingPath(	const hkString& memberPath, const void* rootData, const class hkClass& rootKlass,
										HideMemberFunc hideMember )
{
	if (memberPath.getLength() == 0)
		return hkString("/");

	const hkClass* klass;
	void* data;
	hkString classPath = TweakerUtils::getClass(memberPath, rootData, rootKlass, data, klass);

	hkString memberName = memberPath;
	if (classPath.getLength() > 0)
		memberName = memberName.replace(classPath, hkString(""));

	int leftBracketIndex = memberName.indexOf('[');
	int arrayIndex = -1;

	if ( leftBracketIndex != -1 )
	{
		int rightBracketIndex = memberName.indexOf(']');
		hkString indexString = memberName.substr( leftBracketIndex+1, rightBracketIndex - leftBracketIndex - 1 );
		arrayIndex = hkString::atoi( indexString.cString() );
		memberName = memberName.substr( 0, leftBracketIndex );
	}

	int memberIdx = getMemberIdx(memberName, *klass);

	if ( (memberIdx != -1) && (arrayIndex != -1) )
	{
		const hkClassMember& member = klass->getMember(memberIdx);

		if ( member.getType() == hkClassMember::TYPE_ARRAY )
		{
			void* memberData = static_cast<void*>(static_cast<char*>(data) + member.getOffset());
			hkArray<char>* arrayPtr = static_cast<hkArray<char>*>(memberData);
			int sz = arrayPtr->getSize();

			int newIndex = (arrayIndex + sz - 1) % sz;

			hkString s;
			s.printf( "%s%s[%d]", classPath.cString(), memberName.cString(), newIndex );
			return s;
		}
	}

	if ( memberIdx == -1 )
	{
		memberIdx = 0;
	}

	do
	{
		memberIdx = ( memberIdx - 1 + klass->getNumMembers()) % klass->getNumMembers();
	} while ( hideMember && hideMember(klass->getMember(memberIdx)) );

	return classPath + hkString( klass->getMember(memberIdx).getName());
}

hkString TweakerUtils::getChildPath(	const hkString& memberPath, const void* rootData, const class hkClass& rootKlass,
										HideMemberFunc hideMember )
{
	if (memberPath.getLength() == 0)
		return hkString("/");

	if ( memberPath.endsWith("]") )
	{
		// getClass only returns the class of the array element type if a slash is at the end
		hkString path;
		path = memberPath + hkString("/");

		const hkClass* klass = 0;
		void* data;
		hkString classPath = TweakerUtils::getClass(path, rootData, rootKlass, data, klass);

		if ( klass && ( klass->getNumMembers() > 0 ) )
		{
			for( int i = 0; i < klass->getNumMembers(); i++ )
			{
				const hkClassMember& member = klass->getMember(i);
				if( NOT( hideMember && hideMember(member) ) )
				{
					return classPath + member.getName();
				}
			}

			// all of the members filtered out so we just return the parent path that was passed in
			return memberPath;
		}

		return memberPath;
	}

	hkString path;
	path = memberPath;

	const hkClass* klass;
	void* data;
	hkString classPath = TweakerUtils::getClass(path, rootData, rootKlass, data, klass);

	int memberIdx;
	{
	hkString memberName = path;
	if (classPath.getLength() > 0)
		memberName = memberName.replace(classPath, hkString(""));

		memberIdx = getMemberIdx(memberName, *klass);
	}

	// Couldn't find the member
	if (memberIdx == -1)
	{
		if (rootKlass.getNumMembers() > 0)
		{
			for( int i = 0; i < rootKlass.getNumMembers(); i++ )
			{
				const hkClassMember& member = rootKlass.getMember(i);
				if( NOT( hideMember && hideMember(member) ) )
				{
					return classPath + member.getName();
				}
			}
		}
	}

	const hkClassMember& member = klass->getMember(memberIdx);

	if ( member.getType() ==  hkClassMember::TYPE_STRUCT)
	{
		const hkClass& childClass = member.getStructClass();
		if (childClass.getNumMembers() > 0)
		{
			for( int i = 0; i < childClass.getNumMembers(); i++ )
			{
				const hkClassMember& mem = childClass.getMember(i);
				if( NOT( hideMember && hideMember(mem) ) )
				{
					return memberPath + "/" + mem.getName();
				}
			}

			return memberPath;
		}
	}
	else if (( member.getType() ==  hkClassMember::TYPE_POINTER) && (member.getSubType() == hkClassMember::TYPE_STRUCT))
	{
		const void* ptrAddr = const_cast<const char*> (static_cast<char*>(data) + member.getOffset() );
		const void* ptrTo = *static_cast<const void*const*>(ptrAddr);

		const hkClass& childClass = *getInstanceClass( &member.getStructClass(), ptrTo );

		if ( ptrTo && ( childClass.getNumMembers() > 0 ) )
		{
			for( int i = 0; i < childClass.getNumMembers(); i++ )
			{
				const hkClassMember& mem = childClass.getMember(i);
				if( NOT( hideMember && hideMember(mem) ) )
				{
					return memberPath + "/" + mem.getName();
				}
			}

			return memberPath;
		}
	}
	else if ( member.getType() == hkClassMember::TYPE_ARRAY )
	{
		// check to see if it is an empty array or not

		const void* ptrAddr = const_cast<const char*>( static_cast<char*>(data) + member.getOffset() );
		const hkArray<char>* arrayPtr = static_cast<const hkArray<char>*>( ptrAddr );

		if ( arrayPtr->getSize() )
		{
			return memberPath + "[0]"; // first element
		}
	}

	return memberPath;
}

hkString TweakerUtils::getParentPath(const hkString& memberPath, const void* rootData, const class hkClass& rootKlass)
{
	if (memberPath.getLength() == 0)
		return hkString("/");

	if ( memberPath[memberPath.getLength()-1] == ']' )
	{
		int leftBracketIndex = memberPath.lastIndexOf('[');
		return memberPath.substr( 0, leftBracketIndex );
	}

	const hkClass* klass;
	void* data;
	hkString classPath = TweakerUtils::getClass(memberPath, rootData, rootKlass, data, klass);

	// Remove trailing /
	while (classPath.endsWith("/"))
		classPath = classPath.substr(0, classPath.getLength() -1);

	return classPath;
}


hkBool TweakerUtils::getTweakInput(hkDemoEnvironment* env, 
											const hkString& tweakName, void* tweakee, const class hkClass& klass,
											class hkTextDisplay& disp,	const int xOffset, const int xWidth, const int yOffset,
											hkBool mouseInput, 
											int &downOut, int &upOut, hkBool &leftOut, hkBool &rightOut)
{

	if( !mouseInput )
	{
		// keyboard
		downOut = env->m_gamePad->wasButtonPressed( HKG_PAD_DPAD_DOWN );
		upOut = env->m_gamePad->wasButtonPressed( HKG_PAD_DPAD_UP );
		leftOut = env->m_gamePad->wasButtonPressed( HKG_PAD_DPAD_LEFT );
		rightOut = env->m_gamePad->wasButtonPressed( HKG_PAD_DPAD_RIGHT );
		return true;
	}

	// else mouse
	if( env->m_window->getMouse().getPosX() < xOffset - 10)
	{
		// To the left of (allowable rectangle of) tweak display
		return false;
	}

	if( env->m_window->getMouse().getPosX() > xOffset + xWidth)
	{
		// To the right of (allowable rectangle of) tweak display
		return false;
	}

	int yPos = env->m_window->getHeight()- env->m_window->getMouse().getPosY();

	int	mouseIndex = TweakerUtils::convertDisplayYPosToMemberIndex( (hkReal) yPos, *env->m_textDisplay, (hkReal) yOffset); 
	if(mouseIndex < 0)
	{
		// Above tweak display
		return false;
	}
	int actualIndex = TweakerUtils::getIndexOfMember( tweakName, tweakee, klass, *env->m_textDisplay, 0.0f, (hkReal) yOffset, 0 );

	{
		// Determine how far we actually can go down before hitting end
		int toEnd = 0;
		hkString s = tweakName;
		while(1)
		{
			s = TweakerUtils::getNextSiblingPath( s, tweakee, klass ); 
			int newIndex = TweakerUtils::getIndexOfMember( s, tweakee, klass, *env->m_textDisplay, 0.0f, (hkReal) yOffset, 0 );

			if(newIndex == 0)
			{
				break;
			}
			toEnd++;
		}
		int end = toEnd + actualIndex;
		if(mouseIndex > end)
		{
			// Below tweak display
			return false;
		}

		downOut = mouseIndex - actualIndex;
		upOut = actualIndex - mouseIndex;
	}
	leftOut = env->m_window->getMouse().wasButtonPressed( HKG_MOUSE_LEFT_BUTTON );
	rightOut = env->m_window->getMouse().wasButtonPressed( HKG_MOUSE_RIGHT_BUTTON );
	return true;
}

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
