/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/Tweaker/Tweaker.h>
#include <Demos/DemoCommon/Utilities/GameUtils/TweakerUtils.h>
#include <Demos/DemoCommon/Utilities/Menu/MenuDemo.h>
#include <Common/Base/Reflection/hkClassMember.h>

Tweaker::Tweaker(	void* data, 
					const hkClass& klass,
					HideMemberFunc hideFunc )
:	m_activateButton(HKG_PAD_SELECT),
	m_toNextSiblingButton(HKG_PAD_DPAD_DOWN),
	m_toPrevSiblingButton(HKG_PAD_DPAD_UP),
	m_toParentButton(HKG_PAD_DPAD_LEFT),
	m_toChildButton(HKG_PAD_DPAD_RIGHT),
	m_tweakUpButton(HKG_PAD_BUTTON_3),
	m_tweakDownButton(HKG_PAD_BUTTON_0),
	m_clearButton(HKG_PAD_BUTTON_2),
	m_activateKey('T'),
	m_tweakDownKey(HKG_VKEY_OEM_COMMA),
	m_tweakDownKey2(HKG_VKEY_SPACE),
	m_tweakUpKey(HKG_VKEY_OEM_PERIOD),
	m_decimalKey(HKG_VKEY_DECIMAL),
	m_minusKey(HKG_VKEY_SUBTRACT),
	m_zeroKey(HKG_VKEY_NUMPAD0),
	m_data(data),
	m_class(klass),
	m_hideMembers(hideFunc),
	m_mode(MODE_INACTIVE),
	m_selected("/"),
	m_inputReal(0.0f),
	m_inputDivisor(1.0f),
	m_inputDecimalSeen(false),
	m_inputMinus(false)
{
	m_gamepadHelp =	"Tweaking menu help (gamepad):\n"
					"-----------------------------\n\n"
					" Select: toggle tweaker mode\n"
					"         (off,help,on)\n\n"
					" Use pad to navigate hierarchy\n\n"
					" Tweaking simple data types:\n"
					"  \223: Tweak value up\n"
					"  \220: Tweak value down\n"
					"  \222: Tweak clear value\n\n"
					" Direct keyboard input:\n"
					"  Use keypad digits and decimal\n"
					"  Keypad minus changes sign\n";

	m_keyboardHelp =	"Tweaking menu help (keyboard):\n"
						"------------------------------\n\n"
						" 'T' : toggle tweaker mode\n"
						"       (off,help,on)\n\n"
						" Use arrow keys to navigate hierarchy\n\n"
						" Tweaking simple data types:\n"
						"  '>' : Tweak value up\n"
						"  '<' : Tweak value down\n\n"
						" Direct keypad input:\n"
						"  Use keypad digits and decimal\n"
						"  Keypad minus changes sign\n";
}

void Tweaker::update( hkDemoEnvironment* env )
{
	if (	env->m_gamePad->wasButtonPressed( m_activateButton ) ||
			env->m_window->getKeyboard().wasKeyPressed( m_activateKey ) )
	{
		// cycle the mode
		m_mode = (Mode)(((int)m_mode + 1) % (int)MODE_COUNT);

		resetInput();

		// if the tweaker gets activated, deactivate the help text in the menu demo
		if ( ( m_mode == MODE_HELP ) || ( m_mode == MODE_ACTIVE ) )
		{
			if ( MenuDemo::getMenuDemo() != HK_NULL )
			{
				MenuDemo::getMenuDemo()->turnOffHelp();
			}
		}
	}

	if ( m_mode == MODE_HELP )
	{
		displayHelp( env );
	}
	else if ( m_mode == MODE_ACTIVE )
	{
		// let tweaker handle input from gamepad and controller
		handleInput( env );

		// display the tweaker
		display( env );
		
		// Clean out the pad so the keypresses don't propagate (tweaker thwarts game input)
		hkString::memSet( env->m_gamePad, 0, sizeof(hkgPad));

		
	}
}

void Tweaker::toNextSibling()
{
	m_selected = TweakerUtils::getNextSiblingPath(m_selected, m_data, m_class, m_hideMembers);
	m_inputReal = 0.0f;
	m_inputDecimalSeen = false;
	m_inputMinus = false;
}

void Tweaker::toPrevSibling()
{
	m_selected = TweakerUtils::getPrevSiblingPath(m_selected, m_data, m_class, m_hideMembers);
	m_inputReal = 0.0f;
	m_inputDecimalSeen = false;
	m_inputMinus = false;
}

void Tweaker::toParent()
{
	m_selected = TweakerUtils::getParentPath( m_selected, m_data, m_class );
	m_inputReal = 0.0f;
	m_inputDecimalSeen = false;
	m_inputMinus = false;
}

void Tweaker::toChild()
{
	m_selected = TweakerUtils::getChildPath( m_selected, m_data, m_class, m_hideMembers);
	m_inputReal = 0.0f;
	m_inputDecimalSeen = false;
	m_inputMinus = false;
}

void Tweaker::toRoot()
{
	m_selected = "/";
}

void Tweaker::tweakData( hkReal fraction, const hkReal threshold)
{
	TweakerUtils::tweakData( m_selected, m_data, m_class, fraction, threshold );
}

void Tweaker::clearData()
{
	TweakerUtils::clearData( m_selected, m_data, m_class );
}

void Tweaker::handleDigit( int digit )
{
	if ( m_inputDecimalSeen )
	{
		m_inputReal += ((hkReal)digit)/m_inputDivisor;
		m_inputDivisor *= 10.0f;
	}
	else
	{
		m_inputReal *= 10.0;
		m_inputReal += digit;
	}

	hkReal value = m_inputMinus ? -m_inputReal : m_inputReal;
	TweakerUtils::setReal( m_selected, m_data, m_class, value );
}

void Tweaker::changeSign()
{
	m_inputMinus = !m_inputMinus;

	hkReal value = m_inputMinus ? -m_inputReal : m_inputReal;
	TweakerUtils::setReal( m_selected, m_data, m_class, value );
}

void Tweaker::handleDecimal()
{
	if ( m_inputDecimalSeen )
	{
		m_inputDecimalSeen = false;
		m_inputMinus = false;
		m_inputReal = 0.0f;
	}
	else
	{
		m_inputDecimalSeen = true;
		m_inputDivisor = 10.0;

		// in case the decimal comes before any other digit we need to send the number along here

		hkReal value = m_inputMinus ? -m_inputReal : m_inputReal;
		TweakerUtils::setReal( m_selected, m_data, m_class, value );
	}
}

void Tweaker::resetInput()
{
	m_inputReal = 0.0f;
	m_inputDecimalSeen = false;
	m_inputMinus = false;
}

void Tweaker::display( hkDemoEnvironment* env )
{
	TweakerUtils::displayData( m_selected, m_data, m_class, *env->m_textDisplay, 40, 40, m_hideMembers );

	env->m_textDisplay->outputText( hkString("Tweaking: ") + m_selected, 40, 15, 0xffffffff);
}

void Tweaker::displayHelp( hkDemoEnvironment* env )
{
	bool usingGamepad = env->m_window->hasGamePads() && !env->m_options->m_forceKeyboardGamepad;
	const char* str; 
	if (usingGamepad)
	{
		str =	m_gamepadHelp;
	}
	else // keyboard
	{
		str =	m_keyboardHelp;
	}

	env->m_textDisplay->outputText( str, 40, 15, 0xffffffff);
}

void Tweaker::handleInput( hkDemoEnvironment* env )
{
	const hkgPad& pad = *env->m_gamePad;
	const hkgKeyboard& keys = env->m_window->getKeyboard();
	bool usingGamepad = env->m_window->hasGamePads() && !env->m_options->m_forceKeyboardGamepad;

	// These buttons are mapped nicely to key presses so they handle both gamepad and keyboard input

	if ( pad.wasButtonPressed( m_toNextSiblingButton ) )
	{
		toNextSibling();
	}

	if ( pad.wasButtonPressed( m_toPrevSiblingButton ) )
	{
		toPrevSibling();
	}

	if ( pad.wasButtonPressed( m_toParentButton ) )
	{
		toParent();
	}

	if ( pad.wasButtonPressed( m_toChildButton ) )
	{
		toChild();
	}
	
	// These buttons map badly to the keyboard (for tweaking) so we separate the two cases.

	if ( usingGamepad )
	{
		if ( pad.wasButtonPressed( m_tweakDownButton ) )
		{
			tweakData( -0.1f );
		}

		if ( pad.wasButtonPressed(m_tweakUpButton) )
		{
			tweakData( 0.1f );
		}

		if ( pad.wasButtonPressed( m_clearButton ) )
		{
			clearData();
		}
	}
	else
	{
		if ( keys.wasKeyPressed( m_tweakDownKey ) || keys.wasKeyPressed( m_tweakDownKey2 ) )
		{
			tweakData( -0.1f );
		}

		if ( keys.wasKeyPressed( m_tweakUpKey ) )
		{
			tweakData( 0.1f );
		}
	}

	/////////////////////////////////////////////
	// On the keyboard you can do direct input.
	/////////////////////////////////////////////

	// Handle a period for when a number is being entered directly

	if ( keys.wasKeyPressed( m_decimalKey ) )
	{
		handleDecimal();
	}

	// Handle a minus sign

	if ( keys.wasKeyPressed( m_minusKey ) )
	{
		changeSign();
	}

	// Handle a digit for when a number is being entered directly

	for( char i = 0; i < 10; i++ )
	{
		if ( keys.wasKeyPressed( m_zeroKey + i ) )
		{
			handleDigit( i );

			break;
		}
	}
}

hkBool Tweaker::defaultHideMembers(const hkClassMember& m)
{
	const char* name = m.getName();
	return hkString::strCmp(name, "memSizeAndFlags" ) == 0
		|| hkString::strCmp(name, "referenceCount" ) == 0;
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
