/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

//
// String Internal Representation
//

//
// private methods
//

// inline methods used by other inlines come first


HK_FORCE_INLINE int hkString::getLength() const
{
	// the length of the string is the size -1 (the null termination)
	return m_string.getSize() - 1;
}

HK_FORCE_INLINE const char* hkString::cString() const
{
	return m_string.begin();
}

HK_FORCE_INLINE void hkString::setLength( int length )
{
	m_string.setSize( length + 1 );
}

//
// constructors
//

HK_FORCE_INLINE hkString::hkString()
:m_string(1, 0) // "" string, size==1 => length==0
{
}

HK_FORCE_INLINE hkString::hkString(const hkString& other)
{
	setLength( other.getLength() );
	memCpy( m_string.begin(), other.cString(), other.m_string.getSize() );
}

HK_FORCE_INLINE hkString::hkString(const char* s)
{
	if( s != HK_NULL )
	{
		setLength( strLen(s) );
		hkString::memCpy(m_string.begin(), s, m_string.getSize() ); // copy null too
	}
	else
	{
		// empty string
		setLength(0);
		m_string[0] = 0;
	}	
}

HK_FORCE_INLINE hkString::hkString(char* ptr, int size, int capacity)
{
	m_string.setOwnedData( ptr, size, capacity );
}


HK_FORCE_INLINE hkString::hkString(const char* buf, int len)
{
	HK_ASSERT(0x1b0d49f3, len >= 0);
	setLength( len );
	hkString::memCpy( m_string.begin(), buf, len );
	m_string[len] = 0; // null terminate
}

HK_FORCE_INLINE hkString& hkString::operator= (const hkString& other)
{
	setLength( other.getLength() );
	memCpy( m_string.begin(), other.cString(), m_string.getSize() );
	return *this;
}
		
HK_FORCE_INLINE hkString& hkString::operator= (const char* s)
{
	if(s != HK_NULL && *s != 0)
	{
		int slen = strLen(s);
		setLength( slen );
		hkString::memCpy(m_string.begin(), s, slen+1); // copy null too
	}
	else
	{
		setLength(0);
		m_string[0] = 0;
	}
	return *this;
}

HK_FORCE_INLINE hkString::~hkString()
{
}

HK_FORCE_INLINE char hkString::operator[] (int index) const
{
	return m_string[index];
}

HK_FORCE_INLINE int hkString::compareTo(const hkString& other) const
{
	return strCmp( cString(), other.cString() );
}
HK_FORCE_INLINE int hkString::compareTo(const char* other) const
{
	return strCmp( cString(), other );
}

HK_FORCE_INLINE int hkString::compareToIgnoreCase(const hkString& other) const
{
	return strCasecmp( cString(), other.cString() );
}

HK_FORCE_INLINE int hkString::compareToIgnoreCase(const char* other) const
{
	return strCasecmp( cString(), other );
}

HK_FORCE_INLINE hkBool hkString::operator< (const hkString& other) const
{
	return compareTo(other) < 0;
}

HK_FORCE_INLINE hkBool hkString::operator== (const hkString& other) const
{
	return compareTo(other) == 0;
}
HK_FORCE_INLINE hkBool hkString::operator!= (const hkString& other) const
{
	return compareTo(other) != 0;
}
HK_FORCE_INLINE hkBool hkString::operator== (const char* other) const
{
	return compareTo(other) == 0;
}
HK_FORCE_INLINE hkBool hkString::operator!= (const char* other) const
{
	return compareTo(other) != 0;
}

HK_FORCE_INLINE void hkString::setCapacity(int capacity) 
{
	m_string.reserve( capacity+1 );
}



HK_FORCE_INLINE int hkString::getCapacity() const
{
	return m_string.getCapacity();
}

HK_FORCE_INLINE hkString hkString::substr(int index, int maxChars) const
{
	if(maxChars > getLength() - index)
	{
		maxChars = getLength() - index;
	}
	return hkString(m_string.begin()+index, maxChars);
}

HK_FORCE_INLINE void hkString::setAsSubstr(int index, int maxChars)
{
	if(maxChars > getLength() - index)
	{
		maxChars = getLength() - index;
	}
	hkString::memMove( m_string.begin(), m_string.begin()+index, maxChars+1 );
	m_string[ maxChars ] = 0;
	setLength( maxChars );
}

HK_FORCE_INLINE hkBool hkString::beginsWith (const hkString& other) const
{
	return this->beginsWith( other.cString() );
}

void HK_CALL hkString::memCpy4( void* dst, const void* src, int numWords)
{
	const hkUint32* src32 = reinterpret_cast<const hkUint32*>(src);
	hkUint32* dst32       = reinterpret_cast<      hkUint32*>(dst);
	{
		for (int i = 0; i < numWords; i++)
		{
			*(dst32++) = *(src32++);
		}
	}
}

void HK_CALL hkString::memCpy16( void* dst, const void* src, int numQuads)
{
	HK_ASSERT2( 0xf021d445, (hkUlong(dst) & 0xf) == 0, "Unaligned address" );
	HK_ASSERT2( 0xf021d446, (hkUlong(src) & 0xf) == 0, "Unaligned address" );
	const hkUint32* src32 = reinterpret_cast<const hkUint32*>(src);
	hkUint32* dst32 = reinterpret_cast<      hkUint32*>(dst);
	{
		for (int i = 0; i < numQuads; i++)
		{
			dst32[0] = src32[0];
			dst32[1] = src32[1];
			dst32[2] = src32[2];
			dst32[3] = src32[3];
			dst32+= 4;
			src32+= 4;
		}
	}
}

void HK_CALL hkString::memCpy16NonEmpty( void* dst, const void* src, int numQuads)
{
	HK_ASSERT2( 0xf022d444, numQuads > 0, "Size 0 not allowed" );
	HK_ASSERT2( 0xf022d445, (hkUlong(dst) & 0xf) == 0, "Unaligned address" );
	HK_ASSERT2( 0xf022d446, (hkUlong(src) & 0xf) == 0, "Unaligned address" );
	const hkUint32* src32 = reinterpret_cast<const hkUint32*>(src);
	hkUint32* dst32 = reinterpret_cast<      hkUint32*>(dst);
	{
		do
		{
			dst32[0] = src32[0];
			dst32[1] = src32[1];
			dst32[2] = src32[2];
			dst32[3] = src32[3];
			dst32+= 4;
			src32+= 4;
		}
		while ( --numQuads > 0 );
	}
}

template<int size>
void hkString::memCpy16(void* dst, const void* src)
{
	HK_ASSERT( 0xf0dedf34, ((size & 0xf) == 0) && (size <= 128) && (size > 0));

	hkString::memCpy16NonEmpty(dst, src, size/16);
}

void hkString::memCpy256(void* dst, const void* src)
{
	hkString::memCpy16NonEmpty(dst, src, 16);
}

void hkString::memSet4(void* dst, const int value, int numWords)
{
	hkUint32* dst32 = reinterpret_cast<      hkUint32*>(dst);
	for (int i = numWords-1; i>=0; i--)
	{
		*dst32 = value;
		dst32++;
	}
}

void HK_CALL hkString::memClear16(void* dst, int numQuads)
{
	HK_ASSERT2( 0xf021d445, (hkUlong(dst)   & 0xf) == 0, "Unaligned address" );
	hkUint32* dst32 = reinterpret_cast<      hkUint32*>(dst);
	{
		for (int i = 0; i < numQuads; i++)
		{
			dst32[0] = 0;
			dst32[1] = 0;
			dst32[2] = 0;
			dst32[3] = 0;
			dst32+= 4;
		}
	}
}

void HK_CALL hkString::memSet16(void* dst, const void* value, int numQuads)
{
	HK_ASSERT2( 0xf021d445, (hkUlong(dst) & 0xf) == 0, "Unaligned address" );
	HK_ASSERT2( 0xf021d446, (hkUlong(value) & 0xf) == 0, "Unaligned address" );
	const hkUint32* value32 = reinterpret_cast<const hkUint32*>(value);
	hkUint32* dst32 = reinterpret_cast<      hkUint32*>(dst);
	{
		for (int i = 0; i < numQuads; i++)
		{
			dst32[0] = value32[0];
			dst32[1] = value32[1];
			dst32[2] = value32[2];
			dst32[3] = value32[3];
			dst32+= 4;
		}
	}
}

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
