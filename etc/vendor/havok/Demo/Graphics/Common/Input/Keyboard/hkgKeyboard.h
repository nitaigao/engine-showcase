/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_KEYBOARD
#define HK_GRAPHICS_KEYBOARD

#include <Graphics/Common/Input/Keyboard/hkgKeyboardDefines.h>
#include <Graphics/Common/hkgObject.h>

/// A simple keyboard state wrapper. This is not intended to do more
/// than that, so is not meant to be a base class for the actual keyboard 
/// for instance. The #defines used are just a copy of the winuser.h VK_ 
/// codes in Windows but included here so that we can fake them if wanted 
/// on a console etc which may have a usb keyboard etc or just from a pad.
class hkgKeyboard
{
		friend class hkgWindow;

	public:

			/// Is this keyboard valid? In other words does it represent a proper keyboard.
		inline bool isValid() const;

			/// Get the current key state in an unfiltered form as a pure virtual key. Virtual keys
			/// a the un translated, raw key values from the keyboard and so have no case
			/// and no special language characteristics. A virtual key for funtion keys 
			/// and other OEM keys are present, along with the captial version of 'A'-'Z'
			/// and '0'-'9'. But such letters as small 'c' is actually 'C' and 'SHIFT' perhaps.
		inline bool getKeyState( HKG_KEYBOARD_VKEY key ) const; 

			/// Get the previous value of the given HKG_VKEY. See the getVirtualKeyState() method
			/// description for more info.
		inline bool getPrevKeyState( HKG_KEYBOARD_VKEY key ) const; // virtual key state as per above

			/// Check the transition state of the given HKG_VKEY. This method will return true
			/// if the key has gone from an unpressed to a pressed state in this state frame.
		inline bool wasKeyPressed(HKG_KEYBOARD_VKEY key ) const;
	
			/// Check the transition state of the given HKG_VKEY. This method will return true
			/// if the key has gone from a pressed to an unpressed state in this state frame.
		inline bool wasKeyReleased(HKG_KEYBOARD_VKEY key ) const;

			/// Handy for event timers 
		inline int numKeysPressed() const;

			/// Set the current state of a key
		inline void setCurrentKeyState ( HKG_KEYBOARD_VKEY key, bool state);

			/// Wipe the list of pressed keys from current state, usually due to loss of focus.
		inline void clear();

		/// Only a window can have a keyboard state so this is really a private constructor.
		inline hkgKeyboard(); 

	protected:

		inline void cycleState();

		HKG_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( hkgKeyboard );
		
		bool m_curState[HKG_KEYBOARD_NUM_VKEYS];
		bool m_prevState[HKG_KEYBOARD_NUM_VKEYS];

		bool m_valid;
};

#include <Graphics/Common/Input/Keyboard/hkgKeyboard.inl>

#endif //HK_GRAPHICS_KEYBOARD

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
