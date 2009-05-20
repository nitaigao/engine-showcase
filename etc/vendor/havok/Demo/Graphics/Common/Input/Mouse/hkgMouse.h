/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_MOUSE_H
#define HK_GRAPHICS_MOUSE_H

#include <Common/Base/Types/hkBaseTypes.h>
#include <Graphics/Common/Math/hkgMath.h>
#include <Graphics/Common/Input/Mouse/hkgMouseDefines.h>
#include <Graphics/Common/hkgObject.h>

/// This is a structure to hold the current state for a 3 button mouse. 
/// It is not intended to be a base class for a mouse implementation 
/// but just a data structure for the state info. Only a window can 
/// create a mouse structure.
class hkgMouse
{
		friend class hkgWindow;

	public:
		
			/// Is the mouse data valid? This will be false if no mouse
			/// data has been received.
		inline bool isValid() const;

			/// Get the bitfield that represents the button state. Use 
			/// bit & and bit | to test for individual button.
			/// Current buttons are:
			///   HKG_MOUSE_RIGHT_BUTTON
			///   HKG_MOUSE_MIDDLE_BUTTON
			///   HKG_MOUSE_LEFT_BUTTON
		inline HKG_MOUSE_BUTTON getButtonState() const;

			/// Set the bitfield that represents the button state.
		inline void setButtonState(HKG_MOUSE_BUTTON state);

			/// Get the X (left-right) position of the mouse.
		inline int getPosX() const;

			/// Get the Y (up-down) position of the mouse.
		inline int getPosY() const;

			/// Get the previous mouse button state, See getButtonState for 
			/// more information on how to use the return value.
		inline HKG_MOUSE_BUTTON getPrevButtonState() const;

			/// Get the previous X value.
		inline int getPrevPosX() const;
			
			/// Get the previous Y value.
		inline int getPrevPosY() const;

			/// Was the given button(s) just pressed (in other words, was
			/// the button(s) up last state check and down now).
		inline bool wasButtonPressed(HKG_MOUSE_BUTTON b) const;
	
			/// Was the given button(s) just release (in other words, was
			/// the button(s) down last state check and up now).
		inline bool wasButtonReleased(HKG_MOUSE_BUTTON b) const;

			/// Has the X value of the mouse changed? Was there any left-right movement. The tolerance
			/// is if you want to set the granularity of the check.
		inline bool hasXChanged(int tolerance = 0) const;

			/// Has the Y value of the mouse changed? Was there any up-down movement.The tolerance
			/// is if you want to set the granularity of the check.
		inline bool hasYChanged(int tolerance = 0) const;

		hkgMouse(); 

	protected:


		HKG_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( hkgMouse );

		inline void cycleState();

		HKG_MOUSE_BUTTON	m_curButtonState;
		HKG_MOUSE_BUTTON	m_prevButtonState;

		int m_curX;
		int m_curY;

		int m_prevX;
		int m_prevY;

		bool m_valid;
};

#include <Graphics/Common/Input/Mouse/hkgMouse.inl>

#endif //HK_GRAPHICS_MOUSE_H

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
