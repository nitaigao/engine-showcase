/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DEMO_TWEAKER_H
#define HK_DEMO_TWEAKER_H

#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Graphics/Common/Input/Keyboard/hkgKeyboard.h>

class hkDemoEnvironment;
class hkClassMember;

//////////////////////////////////////////////////////////////////////////////////////////////
// This class encapuslates the tweaker menu, including handling gamepad and keyboard events.
// All you need to do is create one and call update() to process input and display.
// Or you can override the button and key mappings and/or call some of the other methods
// to customize the behavior.
//////////////////////////////////////////////////////////////////////////////////////////////

class Tweaker
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, Tweaker);

		typedef hkBool (HK_CALL *HideMemberFunc)(const hkClassMember& s);
		
		// the default member filter
		static hkBool HK_CALL defaultHideMembers(const hkClassMember& s);
		
		Tweaker(	void* data, 
					const hkClass& klass,
					HideMemberFunc hide = &Tweaker::defaultHideMembers );

		const void* getData() const { return m_data; }

		// full update
		void update( hkDemoEnvironment* env );

		// navigation
		void toNextSibling();
		void toPrevSibling();
		void toParent();
		void toChild();
		void toRoot();

		// tweaking
		void tweakData( hkReal fraction, const hkReal threshold = 0.01f );
		void clearData();

		// direct input
		void handleDigit( int digit );
		void changeSign();
		void handleDecimal();
		void resetInput();

		// display
		void display( hkDemoEnvironment* env );
		void displayHelp( hkDemoEnvironment* env );

		// input
		void handleInput( hkDemoEnvironment* env );

		enum Mode
		{
			MODE_INACTIVE = 0,
			MODE_HELP,
			MODE_ACTIVE,
			MODE_COUNT
		};

		// query
		Mode getMode() { return m_mode; }
		void setMode(Mode m) { m_mode = m; }

		// set these if you don't like the default button mapping
		HKG_PAD_BUTTON m_activateButton;
		HKG_PAD_BUTTON m_toNextSiblingButton;
		HKG_PAD_BUTTON m_toPrevSiblingButton;
		HKG_PAD_BUTTON m_toParentButton;
		HKG_PAD_BUTTON m_toChildButton;
		HKG_PAD_BUTTON m_tweakUpButton;
		HKG_PAD_BUTTON m_tweakDownButton;
		HKG_PAD_BUTTON m_clearButton;

		// set these if you don't like the default key mapping
		HKG_KEYBOARD_VKEY m_activateKey;
		HKG_KEYBOARD_VKEY m_tweakDownKey;
		HKG_KEYBOARD_VKEY m_tweakDownKey2;
		HKG_KEYBOARD_VKEY m_tweakUpKey;
		HKG_KEYBOARD_VKEY m_decimalKey;
		HKG_KEYBOARD_VKEY m_minusKey;
		HKG_KEYBOARD_VKEY m_zeroKey;

		// Change these strings if you change the button and key mappings:
		const char* m_gamepadHelp;
		const char* m_keyboardHelp;



	private:

		// The data that is being tweaked.
		void* m_data;

		// The class of the above data.
		const hkClass& m_class;

		// A filter that determines which members to tweak (by name).
		HideMemberFunc m_hideMembers;

		// The current mode.
		Mode m_mode;

		// The path in the data hierarchy of the current selection.
		hkString m_selected;

		// The current number being directly input using the keyboard.
		hkReal m_inputReal;

		// The current divisor (for digits typed after the decimal).
		hkReal m_inputDivisor;

		// Whether a decimal has been typed.
		hkBool m_inputDecimalSeen;

		// Whether the number being typed is negative.
		hkBool m_inputMinus;

};

#endif // HK_DEMO_TWEAKER_H

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
