/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_PAD
#define HK_GRAPHICS_PAD

#include <Common/Base/Types/hkBaseTypes.h>
#include <Graphics/Common/Math/hkgMath.h>	
#include <Graphics/Common/Input/Pad/hkgPadDefines.h>
#include <Graphics/Common/hkgObject.h>

/// This is a structure used to represent the current and previous state of a generic 
/// controller. It should be noted that not all controllers have the full set of 16
/// buttons and that some have more than just a boolean state, which is not
/// catered for yet in this class. This class is designed to be just a data holder
/// and not the base class for an actual implementation.
class hkgPad
{
		friend class hkgWindow;

	public:

		struct Stick
		{
			Stick() 
			{
				m_cur[0] = m_cur[1] = 0;
				m_prev[0] = m_prev[1] = 0;
			}
			float m_cur[2];
			float m_prev[2];
		};

	public:
		HKG_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( hkgPad );

			/// Only window should really make these, but handy to have for replays
		hkgPad(); 

			/// Is the pad valid? In other words, has any data been entered in this
			/// pad yet / is it plugged in?
		inline bool isValid() const;

			/// Set the bitfield of all 16 button states. Use bit & and bit | and getButtonState
			/// to turn individual buttons on or off.
		inline void setButtonState(HKG_PAD_BUTTON newState);

			/// Get the bitfield of all 16 button states. Use bit & and bit | to find individual 
			/// button state from the return value. All unused buttons will just be always off.
		inline HKG_PAD_BUTTON getButtonState() const;

			/// Get the bitfield of all 16 button states from the previous snapshot. Use bit & and 
			/// bit | to find individual button state from the return value. All unused buttons 
			/// will just be always off.
		inline HKG_PAD_BUTTON getPrevButtonState() const;

			/// Get the normalized (-1.0, 1.0) stick X position for the given stick number. There is a max of
			/// 2 sticks per pad and any not present will stay at (0,0).
		inline float getStickPosX(int stick) const;
	
			/// Get the normalized (-1.0, 1.0) stick Y position for the given stick number. There is a max of
			/// 2 sticks per pad and any not present will stay at (0,0).
		inline float getStickPosY(int stick) const;

			/// Get the previous stick X position for the given stick number. See getStickPosX().
		inline float getPrevStickPosX(int stick) const;
			
			/// Get the previous stick Y position for the given stick number. See getStickPosY().
		inline float getPrevStickPosY(int stick) const;

			/// Is the button currently pressed?
		inline bool isButtonPressed(HKG_PAD_BUTTON b) const;

			/// Was the give button(s) pressed? Was the state of the previous != current and is the
			/// current state down.
		inline bool wasButtonPressed(HKG_PAD_BUTTON b) const;

			/// Was the give button(s) released? Was the state of the previous != current and is the
			/// current state up.
		inline bool wasButtonReleased(HKG_PAD_BUTTON b) const;

			/// Force the button to be considered pressed.
			/// Useful for faking input in a bootstrapped demo.
		inline void forceButtonPressed(HKG_PAD_BUTTON b);

			/// Has the X position of the given stick on the pad changed (with tolerance)
			/// from the last state?
		inline bool hasStickXChanged(int stick, float tolerance = 0.05f) const;

			/// Has the Y position of the given stick on the pad changed (with tolerance)
			/// from the last state?
		inline bool hasStickYChanged(int stick, float tolerance = 0.05f) const;
		
		const Stick& getStickState(int stick) const;

	protected:

		inline void cycleState(); 

		HKG_PAD_BUTTON	m_curButtonState;
		HKG_PAD_BUTTON	m_prevButtonState;
		
		Stick m_stick[2];

		bool m_valid;

};
	
#include<Graphics/Common/Input/Pad/hkgPad.inl>
	
#endif // HK_GRAPHICS_PAD

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
