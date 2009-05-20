/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DEFAULTDEMO_H
#define HK_DEFAULTDEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDemo.h>
#include <Common/Base/Container/Array/hkObjectArray.h>

// For access to display context
#include <Graphics/Common/Window/hkgWindow.h>

class hkgDisplayObject;
class hkgDisplayHandler;
class hkgCamera;
class hkgWindow;

class hkpWorld;
class hkDemoEnvironment;
class hkpMouseSpringAction;
class hkStopwatch;
class hkClassRegistry;
class hkStreamReader;
class hkProcess;
class hkpPhysicsContext;
class hkProcessContext;

class hkVisualDebugger;
class hkVtableClassRegistry;
struct hkTimerData;

class hkDefaultDemo;
class DemoStepper : public hkReferencedObject
{
	public:

		virtual hkDemo::Result stepDemo( hkDefaultDemo* demo ) = 0;
};

#define HK_USE_CHARACTER_FACTORY

class hkDefaultDemo : public hkDemo
{
	public:

		// The constructor
		hkDefaultDemo(hkDemoEnvironment* env, bool isMenuDemo = false );

		// The destructor
		virtual ~hkDefaultDemo();

		virtual void postConstruct() { ; }

		virtual void preDeleteDemo() { ; }

		virtual Result stepDemo();


		void addStepper( DemoStepper* stepper );

		virtual Result stepVisualDebugger();
		virtual bool visualDebuggerEnabled() { return m_vdb != HK_NULL; }

		// This implementation will setup the VDB and the local debug viewers
		void setupGraphics();
		void shutdownVDB();

		// Helper to temporarily switch on/off graphics states
		void setGraphicsState(HKG_ENABLED_STATE state, hkBool on);

		void forceShadowState(bool shadowsOn);

		inline void startGraphicsCodeSection() const { m_env->m_window->getContext()->lock(); }
		inline void endGraphicsCodeSection() const { m_env->m_window->getContext()->unlock(); }

		// Cleanup the viewers
		void cleanupGraphics();

		// set a default camera
		void setupDefaultCameras( hkDemoEnvironment* env, const hkVector4& from, const hkVector4& to, const hkVector4& up, const hkReal nearPlane = 1.0f, const hkReal farPlane = 500.0f, bool rightHanded = true) const;

		enum CameraAxis
		{
			CAMERA_AXIS_X,
			CAMERA_AXIS_Y,
			CAMERA_AXIS_Z,
		};
		// Set up a camera looking at the origin with up axis.
		void setupDefaultCameras( hkEnum<CameraAxis,int> upAxis, hkReal fromX, hkReal fromY, hkReal fromZ ) const;

		// Gets the current path of the demo
		const hkString& getMenuPath() const { return m_menuPath; }

		int getDemoVariant() const { return m_variantId;	}

		// Start a little progress slider. E.g. good for loading or processing big files
		void progressBegin(const char* what);

		// Update the progress slider with a value between 0 and 1.
		void progressSet(hkReal fraction);

		// Remove progress slider.
		void progressEnd();

		// Unprojects and calls objectPicked.
		virtual void mouseDown();

		// Calls objectReleased.
		virtual void mouseUp();

		// Unprojects and calls objectDragged.
		virtual void mouseDrag();

		static void HK_CALL setupLights(hkDemoEnvironment* env);
		static void HK_CALL setupSkyBox(hkDemoEnvironment* env, const char* skyBoxFileName = HK_NULL);
		static void HK_CALL setSoleDirectionLight(hkDemoEnvironment* env, float dir[3], hkUint32 color);
		static void HK_CALL setupFixedShadowFrustum(hkDemoEnvironment* env, hkgLight& light, const hkgAabb& areaOfInterest, float extraNear = 0, float extraFar = 0 );
		static void HK_CALL loadingScreen(hkDemoEnvironment* env, const char* screenFile = HK_NULL);

		// handy simple version of above (does a set sole dir then a fixed shadow frustum)
		virtual void setLightAndFixedShadow(float* lightDir, float* shadowAabbMin, float* shadowAabbMax, float extraNear = 0, float extraFar = 0);

		static void HK_CALL removeLights(hkDemoEnvironment* env);

		// Called by setupgraphics	if options request it
		void setupVisualDebugger(const hkArray<hkProcessContext*>& contexts, hkBool runServer = true, const char* captureFile = HK_NULL);

		// Setup the VDB and Local graphics pipes.
		// Called by setupgraphics
		virtual void setupContexts(hkArray<hkProcessContext*>& contexts);

			/// get the local (on screen) viewer, for instance the hhShapeDisplayViewer, by name (eg "Shapes", or hkpShapeDisplayViewer::getName() );
		hkProcess* getLocalViewerByName( const char* name );

			/// get the viewers used by VDB clients, for instance the hhShapeDisplayViewer, by name (eg "Shapes", or hkpShapeDisplayViewer::getName() );
		void getVDBViewersByName( const char* name, hkArray<hkProcess*>& processes );


		int getNumSpus();

		// Called by the BootstrapDemo before it steps the demo
		// Use this to fake any gamepad input to make the demo interesting
		void makeFakeInput() {}
	//
	// Internal
	//
	public:

		// Called by mouseDown, should return true if it picks an object
		virtual hkBool objectPicked( const hkgDisplayObject* displayObject, const hkVector4& worldPosition, int geomIndex ) { return false; }

		// Called by mouseUp
		virtual void objectReleased() { }

		// Called by mouseDrag
		virtual void objectDragged( const hkVector4& newWorldSpacePoint ) { }

		hkArray<DemoStepper*> m_steppers;

		//
		hkReal m_timestep;

		// The path in the demo menu
		hkString m_menuPath;

		// The path of the demo (not necessarily the resources)
		hkString m_demoPath;

		int		m_variantId;

		// Some helper to print progress bars while loading
		hkStopwatch* m_lastProgress;
		hkString m_progressName;

		// If true objectDragged is called, else objectPicked
		hkBool m_mouseActive;
		// The z-coordinate of the object picked
		hkReal m_mousePosDepth;

		// We remove reference as last step in destructor - handy for storing hkPackfileData
		hkArray<hkReferencedObject*> m_delayedCleanup;

#if defined (HK_USE_CHARACTER_FACTORY)
		// A utilitiy class for creating a character in a demo - will do different things depending on what products are present
		// Don't use directly - call getCharacterFactory()
		class CharacterFactory* m_characterFactory;
	
		// Initializes m_characterFactory if it isn't already
		class CharacterFactory* getCharacterFactory( );
#endif

	public:

		// Some debug viewers (debuglines, states, shapes, etc)
		hkObjectArray<hkString> m_debugViewerNames;
		hkArray<hkProcess*> m_debugProcesses;
		hkVisualDebugger* m_vdb;
		hkVtableClassRegistry* m_vdbClassReg;

		hkArray<hkProcessContext*> m_contexts;





	public:
			// Handles timer display for multithreaded demos
		virtual void addOrRemoveThreads( );
		virtual void getNumTimerStreams( int& numThreadStreams, int& numSpuStreams, int maxThreads = 0x7fffffff ) const;
		virtual void getTimerStreamInfo( hkArray<hkTimerData>& timerStreams, hkArray<hkTimerData>& spuStreams, int maxThreads = 0x7fffffff );
		virtual void resetTimerStreams();


	public:

		//
		// Multi-threading data
		//
		class hkJobQueue* m_jobQueue;
		class hkSpuUtil* m_spuUtil;
		class hkJobThreadPool* m_jobThreadPool;

			// If set to false, the user cannot reduce the number of active SPUs to zero using the demo framework.
			// This is necessary for e.g. the collision query demos to avoid a situation where an SPU job remains on
			// the job queue if no active SPU is available and the PPU is not allowed to take it.
		hkBool m_allowZeroActiveSpus;


		hkBool m_allowChangingNumThreads;


		hkBool m_forcedShadowsOff;
		hkBool m_forcedShadowsOn;

};

#endif // HK_DEFAULTDEMO_H


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
