/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DEMOFRAMEWORK_DEMO_H
#define HK_DEMOFRAMEWORK_DEMO_H

#include <Common/Base/hkBase.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Graphics/Common/Input/Keyboard/hkgKeyboard.h>

#include <Common/Base/Thread/Job/ThreadPool/hkJobThreadPool.h>

struct hkDemoEntry;
class hkgWindow;
class hkgDisplayHandler;
class hkgSceneDataConverter;
class hkgDisplayWorld;
class hkgPad;
class hkPerformanceCounterUtility;
struct hkDemoFrameworkOptions;
class hkMonitorStreamAnalyzer;
struct hkTimerData;

#define DEMO_OPTIONS_DECLARE(OPTIONS) \
		virtual hkVariant* getOptions() { return &m_optionsVariant; } \
		static hkVariant m_optionsVariant; \
		static OPTIONS m_options

#define DEMO_OPTIONS_DEFINE(OWNER,OPTIONS) \
	extern const hkClass OWNER##OPTIONS##Class; \
	OWNER::OPTIONS OWNER::m_options; \
	hkVariant OWNER::m_optionsVariant = { &OWNER::m_options, &OWNER##OPTIONS##Class }


// Demo environment
class hkDemoEnvironment
{
	public:

		hkDemoEnvironment();

		// When mapping keys to gamepad buttons, the Ctrl key is used to determine which pad a key affects.
		// This function returns true if the Ctrl key needs to be down to affect m_gamePad (index 0)
		// or m_gamePadB (index 1).  This function is useful if a demo is handling additional
		// keys explicitly and wants to correlate them with the gamepads.  But it's better to
		// call wasKeyPressed or wasKeyReleased below which will automate the procedure.
		hkBool getDesiredKeyboardCtrlStateForGamePad( int padIndex=0 ) const;

		// You can find out if a key has been pressed or released when the ctrl key is in the state
		// associated with a particular gamepad.
		hkBool wasKeyPressed( int padIndex, HKG_KEYBOARD_VKEY key ) const;
		hkBool wasKeyReleased( int padIndex, HKG_KEYBOARD_VKEY key ) const;
		hkBool getKeyState( int padIndex, HKG_KEYBOARD_VKEY key ) const;

		// COM-305 : You can use reserve/release in your demos if you want to use the CTRL key (PC only) 
		//	         for something different than switching game pads
		void reserveControlKey();
		void releaseControlKey();

		// Environment
		hkgWindow*	m_window;
		hkgDisplayHandler* m_displayHandler;
		hkgSceneDataConverter* m_sceneConverter;
		hkgDisplayWorld* m_displayWorld;
		hkTextDisplay* m_textDisplay;
		hkgPad* m_gamePad;
		hkgPad* m_gamePadB;
		hkBool m_controlKeyReserved;

		hkBool m_mousePickingEnabled;

		hkDemoFrameworkOptions* m_options;
		
		// Current demo
		hkString m_menuPath;
		hkString m_demoPath;
		int m_variantId;
		int m_cpuMhz; // similar to x86 of this speed

		enum ReportingLevel
		{
			REPORT_OFF		= 0,
			REPORT_FATAL	= 1,
			REPORT_ERROR	= 2,
			REPORT_WARN		= 3,
			REPORT_INFO		= 4,
			REPORT_DEBUG	= 5,
			REPORT_ALL		= REPORT_DEBUG
		};

		ReportingLevel m_reportingLevel;	// Specifies the desired level of console output

		char m_exitCode;	// Exit Code to return from framework

};

// Base demo class
class hkDemo : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		hkDemo(hkDemoEnvironment* env) : m_bootstrapIterations(100)
		{
			env->m_mousePickingEnabled = true;
			m_env = env;
		} 

		// Step result
		enum Result
		{
			DEMO_OK,
			DEMO_PAUSED,
			DEMO_STOP,
			DEMO_RESTART,
			DEMO_ERROR
		};

		// called after demo's constructor has been executed
		virtual void postConstruct() { ; }

		// called right before the demo is deleted
		virtual void preDeleteDemo() { ; }

		// Run a single step of the demo
		virtual Result stepDemo() = 0;

		virtual void waitForStepCompletion() {}

		// Perform whatever extra operations independent of simulation being run/paused.
		virtual void preRenderDisplayWorld(class hkgViewport* view) {}
		virtual void postRenderDisplayWorld(class hkgViewport* view) {}

		// Run single step the VDB, if one
		virtual Result stepVisualDebugger() = 0;
		virtual bool visualDebuggerEnabled() = 0;

		virtual void getNumTimerStreams( int& numThreadStreams, int& numSpuStreams, int maxThreads = 0x7fffffff ) const;
		virtual void getTimerStreamInfo( hkArray<hkTimerData>& threadStreams, hkArray<hkTimerData>& spuStreams, int maxThreads = 0x7fffffff );

		virtual void resetTimerStreams();
		virtual void addTimersToVdb( const hkArray<hkTimerData>& threadStreams, const hkArray<hkTimerData>& spuStreams ) {}

		// What tweakable parameters does this demo have
		virtual hkVariant* getOptions() { return HK_NULL; }

		// Get any error text
		const hkString& getError() { return m_error; }

		virtual void mouseDown() {}	// Mouse has been pressed
		virtual void mouseUp() {}	// Mouse has been released
		virtual void mouseDrag() {}	// Mouse has been dragged

		virtual void windowResize(int w, int h) {} // window has resize. Handy to redo some 2d graphs etc

		void setDemoName(hkString& name);

			/// Recursively scan a directory for hkx files and create a demo for each found entry
		static void HK_CALL scanDirectoryAndCreateDemosForHkxFiles(hkString path, hkDemoEntry& thisEntry, hkArray<hkDemoEntry*>& entriesOut );

		// Called by the BootstrapDemo
		// Artificial input when running in bootstrap mode
		virtual void makeFakeInput() = 0;


		// Our current demo environment
		hkDemoEnvironment* m_env;

		// Number of times to the demo will be stepped by the bootstrapper
		int m_bootstrapIterations; // +default(100)

	protected:

		hkString m_error;
		hkString m_name;
};





// Error demo
class ErrorDemo : public hkDemo
{
	public:

		ErrorDemo(hkDemoEnvironment* env, const char* failed)
			: hkDemo(env), m_failed(failed)
		{
		}

		Result stepDemo()
		{
			hkString os;
			os.printf("Creation of '%s' demo failed. Was it registered?", m_failed);
			m_env->m_textDisplay->outputText(os, 20, 20);
			return DEMO_OK;			
		}

		Result stepVisualDebugger()
		{
			return DEMO_OK;
		}

		void makeFakeInput() {}

		bool visualDebuggerEnabled() { return false; }

		const char* m_failed;
};

#endif // HK_DEMOFRAMEWORK_DEMO_H

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
