/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef DEMO_REPLAY_H
#define DEMO_REPLAY_H

#include <Common/Base/hkBase.h>
#include <Common/Base/Container/BitField/hkBitField.h>

/// A utility class to record/playback the user input for a demo.
/// You can record a movie with all the graphics setting set to low, and then playback the
/// demo with all the graphical settings set to max, and dump each frame to a bitmap file.
/// There are some caveats involved:
/// - When playing input back, the screen resolution must be the same screen resolution as
///   the input was recorded with.
/// - The mouse should not be moved when playing a demo back.
///   This will cause the window manager to send mouse move messages, which are not currently
///   intercepted by this class.
/// This class is intended for internal Havok use only.
class hkDemoReplayUtility
{
	public:

		HK_DECLARE_REFLECTION();

		hkDemoReplayUtility();
		~hkDemoReplayUtility();

		/// Start recording input.
		void startRecording(void);

		/// Stop recording, and save the recorded input to the given file.
		void endRecordingAndSave( const char* filename );

		/// Start playing back input from the given file
		void startPlayback(const char* filename, hkDemoEnvironment* env);

		/// Stop playing back input
		void endPlayback ( hkDemoEnvironment* env );

		/// Record the current state
		void recordFrame(hkDemoEnvironment* env);

		/// Playback a single frame
		void playbackFrame(hkDemoEnvironment* env);

		static bool HK_CALL DummyMouseMoveFunc(hkgWindow* w, int x, int y, int dx, int dy, void* userContext);
		static bool HK_CALL DummyMouseWheelFunc(hkgWindow* w, int delta, void* userContext);
		static bool HK_CALL DummyMouseButtonFunc(hkgWindow* w, HKG_MOUSE_BUTTON button, bool state, void* userContext);

private:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO_FRAMEWORK, hkDemoReplayUtility);

		/// Overrides window manager callbacks to do nothing, when playing back a recording.
		/// This prevents the window manager from sending mouse move events to the demo
		/// framework, which will mess up the playback.
		void hijackInputCallbacks  ( hkDemoEnvironment* env );

		/// Restores the callbacks which were modified in hijackInputCallbacks().
		void restoreInputCallbacks ( hkDemoEnvironment* env );

	public:

		/// Store the current camera matrix
		class Camera
		{
			public:
			HK_DECLARE_REFLECTION();
			float		m_from[3];
			float		m_to[3];
			float		m_up[3];
		};

		/// Store a frame of data
		class Frame
		{
			public:
			HK_DECLARE_REFLECTION();
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO_FRAMEWORK, hkDemoReplayUtility::Frame);

			/// Each bit represents a key which is down
			class hkBitField					m_keysDown;

			/// Store the current camera matrix
			class hkDemoReplayUtility::Camera	m_camera;

			/// Store the current mouse coordinates
			int									m_mouseX;
			int									m_mouseY;

			/// Store the current gamepad buttons
			hkUint32							m_padButtons;

			/// Store the current mouse buttons
			hkUint32							m_mouseButtons;

			/// Store the current state of the analgue sticks
			hkReal								m_sticks[4];

			Frame ( void );
			Frame ( hkFinishLoadedObjectFlag flag );
		};

		/// This is just a simple wrapper around an array of Frame objects.
		/// We wrap the array so that we can serialize it
		class ReplayData : public hkReferencedObject
		{
		  public:
			//+vtable(true)
			HK_DECLARE_REFLECTION();
			HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO_FRAMEWORK);

			ReplayData ( void );
			ReplayData ( hkFinishLoadedObjectFlag flag );
			~ReplayData();

			/// Was the frame data allocated by this class, or by the serialization system
			hkBool					m_framesWereAllocatedExternally;

			/// The input frames that will be stored
			hkArray<class Frame*>	m_frames;

			/// The inital mouse position.
			/// These are stored so that the inital state of the demo is recorded precisely.
			int						m_initialMouseX;
			int						m_initialMouseY;
		};

		class MouseCallbacks
		{
		public:
			MouseCallbacks()
				: m_mouseMoveFunc(HK_NULL)
				, m_mouseWheelFunc(HK_NULL)
				, m_mouseButtonFunc(HK_NULL)
			{
			}

			HK_DECLARE_REFLECTION();
			HKG_USER_FUNC_MOUSE_MOVE	m_mouseMoveFunc;	//+overridetype(int)
			HKG_USER_FUNC_MOUSE_WHEEL	m_mouseWheelFunc;	//+overridetype(int)
			HKG_USER_FUNC_MOUSE_BUTTON	m_mouseButtonFunc;	//+overridetype(int)
		};

		/// The data to save
		ReplayData*				m_replayData;

		/// The current frame, 0 -> # frames of data
		int						m_currentFrame;

		/// Any data loaded from the packfile
		class hkPackfileData*	m_allocatedData; //+nosave

		/// Store the old state of the window manager's callbacks.
		class MouseCallbacks	m_oldMouseCallbacks; //+serialized(false)
};

#endif

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
