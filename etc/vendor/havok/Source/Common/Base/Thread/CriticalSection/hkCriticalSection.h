/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_HK_CRITICAL_SECTION_H
#define HKBASE_HK_CRITICAL_SECTION_H

#include <Common/Base/Config/hkConfigThread.h>
#include <Common/Base/Monitor/hkMonitorStream.h>


#	include <Common/Base/Fwd/hkwindows.h>

#include <Common/Base/Thread/Thread/hkThreadLocalData.h>

	/// Set this define if you want a timer begin and timer end call 
	/// if a thread has to wait for a critical section
	/// You also have to call hkCriticalSection::setTimersEnabled()
#define HK_TIME_CRITICAL_SECTION_LOCKS

#if !defined (HK_TIME_CRITICAL_SECTION_LOCKS) && defined(HK_SIMULATE_SPU_DMA_ON_CPU)
#	define HK_TIME_CRITICAL_SECTION_LOCKS
#endif

#define HK_PS3_CRITICAL_SECTION_SYSTEM_WAIT 1

	/// Critical section wrapper. This can be used to guard access
	/// to shared data structures between threads.
	/// Note that critical sections are fast but have serious drawbacks.
	/// Check windows help for details. 
	/// Note that including this file means including a system file, such as windows.h, 
	/// which makes compile time significantly slower.
class hkCriticalSection
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE_CLASS, hkCriticalSection);

			/// Init a critical section with spin count. 
			/// Read MSDN help of InitializeCriticalSectionAndSpinCount for details.
			/// In short: positive spinCount value results in threads doing busy waiting and is good
			///           when you know that the critical section will only be locked for a short period of time.
			///           zero value causes the thread to immediately go back to the system when waiting.
		hkCriticalSection( int spinCount = 0, hkBool32 addToList = true );

			/// Quit a critical section
		inline ~hkCriticalSection();

			/// Lock a critical section
		inline void enter();

			/// Unlock a critical section
		inline void leave();


			/// Tell the critical section to time blocking locks. HK_TIME_CRITICAL_SECTION_LOCKS must be
			/// defined at compile time for this flag to have any effect
		static inline void HK_CALL setTimersEnabled();

			/// Stop timing blocking locks
		static inline void HK_CALL setTimersDisabled();

			/// adds a value to a variable in a thread safe way and returns the old value
		static HK_FORCE_INLINE hkUint32 HK_CALL atomicExchangeAdd(hkUint32* var, int value);


	public:

#if HK_CONFIG_THREAD == HK_CONFIG_MULTI_THREADED
		CRITICAL_SECTION  m_section;

		struct SectionList
		{
			SectionList( hkCriticalSection* section, int spinCount, hkBool32 addToList );
			~SectionList();
			static inline bool isEmpty();

				// The spin count value of created critical section.
			int m_spinCount;

				// Link to the previous critical section wrapper in the list.
			hkCriticalSection* m_prev;

				// Link to the next critical section wrapper in the list.
			hkCriticalSection* m_next;

				// Head of the critical section wrappers list.
			static hkCriticalSection s_listHead;
		};
		SectionList m_list;
#endif
};

// move to static member when compiler support
#if HK_CONFIG_THREAD == HK_CONFIG_MULTI_THREADED && defined(HK_TIME_CRITICAL_SECTION_LOCKS)
	extern HK_THREAD_LOCAL( int ) hkCriticalSection__m_timeLocks;
#endif



#if !defined(HK_PLATFORM_PS3SPU) && (HK_CONFIG_THREAD == HK_CONFIG_MULTI_THREADED)
	inline hkCriticalSection::SectionList::SectionList(hkCriticalSection* self, int spinCount, hkBool32 addToList)
		: m_spinCount(spinCount), m_prev(HK_NULL), m_next(HK_NULL)
	{
		if( addToList )
		{
			s_listHead.enter();
			// add to the list
			m_next = s_listHead.m_list.m_next;
			if( m_next )
			{
				m_next->m_list.m_prev = self;
			}
			m_prev = &s_listHead;
			s_listHead.m_list.m_next = self;
			s_listHead.leave();
		}
	}

	inline hkCriticalSection::SectionList::~SectionList()
	{
		if( m_prev )
		{
			s_listHead.enter();
				// If you get crash here:
				// It seems some critical sections were not destroyed and removed
				// from the list of the critical section wrappers, and links are
				// referring to the already invalid objects.
				// To resolve the issue make sure there are no memory leaks of the
				// critical section wrappers.
			m_prev->m_list.m_next = m_next;
			if( m_next )
			{
					// If you get crash here:
					// It seems some critical sections were not destroyed and removed
					// from the list of the critical section wrappers, and links are
					// referring to the already invalid objects.
					// To resolve the issue make sure there are no memory leaks of the
					// critical section wrappers.
				m_next->m_list.m_prev = m_prev;
			}
			s_listHead.leave();
		}
	}

	inline bool hkCriticalSection::SectionList::isEmpty()
	{
		bool ret;
		s_listHead.enter();
		ret = (s_listHead.m_list.m_next == HK_NULL);
		s_listHead.leave();
		return ret;
	}
#endif

// include the inl before the hkCriticalSectionLock def so 
// that gcc can inline the enter and leave properly
#if HK_CONFIG_THREAD != HK_CONFIG_MULTI_THREADED 
#		include <Common/Base/Thread/CriticalSection/Empty/hkEmptyCriticalSection.inl>
#else // HK_CONFIG_THREAD == HK_CONFIG_MULTI_THREADED 
#	include <Common/Base/Thread/Thread/hkThread.h>
#	define HK_INVALID_THREAD_ID (hkUint64(-1))

#		include <Common/Base/Thread/CriticalSection/Win32/hkWin32CriticalSection.inl>
#endif


	/// Helper class which locks a critical section as long is this object exists. 
class hkCriticalSectionLock
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE_CLASS, hkCriticalSectionLock);

			/// Create a lock by entering a critical section.
		inline hkCriticalSectionLock( hkCriticalSection* section )
		{
			m_section = section;
			section->enter();
		}

			/// Destructor leaves the critical section
		inline ~hkCriticalSectionLock()
		{
			m_section->leave();
		}

	protected:
		hkCriticalSection* m_section;
};

#endif // HKBASE_HK_CRITICAL_SECTION_H

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
