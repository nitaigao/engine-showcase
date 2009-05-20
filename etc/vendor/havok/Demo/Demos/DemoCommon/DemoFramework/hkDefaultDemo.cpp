/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/DemoCommon/DemoFramework/hkDemo.h>
#include <Demos/demos.h>

#include <Demos/DemoCommon/DemoFramework/hkDemo.h>
#include <Demos/DemoCommon/DemoFramework/hkDefaultDemo.h>
#include <Demos/DemoCommon/DemoFramework/hkDemoFramework.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>

#include <Common/Base/System/Io/FileSystem/hkFileSystem.h>
#include <Common/Base/System/Io/Reader/hkStreamReader.h>
#include <Common/Base/System/Stopwatch/hkStopwatch.h>
#include <Common/Base/Reflection/hkClass.h>

#include <Graphics/Common/hkGraphics.h>
#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/Camera/hkgCamera.h>
#include <Graphics/Common/Texture/SkyBox/hkgSkyBox.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Common/DisplayWorld/hkgDisplayWorld.h>
#include <Graphics/Common/Light/hkgLightManager.h>
#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>

#include <Common/Visualize/hkProcess.h>
#include <Common/Visualize/hkProcessRegisterUtil.h>
#include <Common/Visualize/hkVisualDebugger.h>
#include <Common/Visualize/hkProcessFactory.h>
#include <Common/Visualize/Process/hkDebugDisplayProcess.h>

#include <Common/Base/Reflection/Registry/hkVtableClassRegistry.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Thread/Job/ThreadPool/Spu/hkSpuJobThreadPool.h>

#if defined USING_HAVOK_PHYSICS
	#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/SimpleDemoCharacter/SimpleDemoCharacter.h>
	#if defined USING_HAVOK_ANIMATION
	#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/AnimatedDemoCharacter/AnimatedDemoCharacter.h>
	#endif
#endif



#if defined HK_PLATFORM_PS3_PPU
#include <Common/Base/Memory/PlattformUtils/Spu/hkSpuMemoryInternal.h>
#define	SPURS_THREAD_GROUP_PRIORITY 250
#define SPURS_HANDLER_THREAD_PRIORITY 1
#define MAX_SPU_THREADS 6
#endif




hkDefaultDemo::hkDefaultDemo(hkDemoEnvironment* env, bool isMenuDemo )
	:	hkDemo(env),
		m_timestep(0.016f),
		// m_error
		m_menuPath(env->m_menuPath),
		m_variantId( env->m_variantId),
		// lastProgress
		// progressname
		m_lastProgress( HK_NULL ),
		m_mouseActive(false),
		m_mousePosDepth(0),
		m_vdb(HK_NULL),
		m_vdbClassReg(HK_NULL),
		m_allowZeroActiveSpus(true),
		m_allowChangingNumThreads(true)
{

	m_jobQueue = HK_NULL;
	m_jobThreadPool = HK_NULL;
	m_spuUtil = HK_NULL;

	m_forcedShadowsOff = false;
	m_forcedShadowsOn = false;
	
	if (!isMenuDemo)
	{

	#if HK_CONFIG_THREAD == HK_CONFIG_MULTI_THREADED

			hkCpuJobThreadPoolCinfo info;
			info.m_numThreads = m_env->m_options->m_numThreads - 1;
			//if ( info.m_numThreads < 1 ) info.m_numThreads = 1;
			info.m_timerBufferPerThreadAllocation = 2000000;
			m_jobThreadPool = new hkCpuJobThreadPool(info);

		// Create multi-threading classes
		hkJobQueueCinfo jobQueueInfo;
		jobQueueInfo.m_maxNumJobTypes = HK_JOB_TYPE_MAX; // Allow user jobs

		m_jobQueue = new hkJobQueue( jobQueueInfo );

	#endif
	}

	//HK_ASSERT(0, ((hkUlong)m_jobQueue & 0x3F) == 0); // check 64 byte aligned




	// Set default viewport state
	if ( env->m_window )
	{
			hkProcessRegisterUtil::registerAllCommonProcesses(); // all common ones (stats, debug display, etc)

			env->m_window->getCurrentViewport()->setDesiredState(
				HKG_ENABLED_LIGHTING | HKG_ENABLED_TEXTURE2D |
				HKG_ENABLED_ZREAD | HKG_ENABLED_ZWRITE | HKG_ENABLED_CULLFACE
			);

			// Set up anisotropic filtering
			if (env->m_sceneConverter && env->m_options)
			{
				env->m_sceneConverter->setAllowTextureAnisotropicFilter(env->m_options->m_anisotropicFiltering);
			}
	}

#if defined (HK_USE_CHARACTER_FACTORY)
	m_characterFactory = HK_NULL;
#endif
}

#if defined (HK_USE_CHARACTER_FACTORY)
CharacterFactory* hkDefaultDemo::getCharacterFactory( )
{
	if (!m_characterFactory)
	{
#if !defined USING_HAVOK_PHYSICS
		m_characterFactory = HK_NULL; // No version currently implemented that do not use a character proxy from phsyics
#else
#	if defined USING_HAVOK_ANIMATION
		//#if 0
		m_characterFactory = new AnimatedCharacterFactory( );
#	else 
		m_characterFactory = new SimpleCharacterFactory();
#	endif
#endif
	}
	return m_characterFactory;
}
#endif // defined (HK_USE_CHARACTER_FACTORY)

void hkDefaultDemo::shutdownVDB()
{
	for (int dv=0; dv<m_debugProcesses.getSize(); ++dv)
	{
		delete m_debugProcesses[dv];
	}
	m_debugProcesses.setSize(0);
	m_debugViewerNames.setSize(0);

	if (m_vdb)
	{
		m_vdb->shutdown();
		m_vdb->removeReference();
		m_vdb = HK_NULL;
	}

	if (m_vdbClassReg)
	{
		m_vdbClassReg->removeReference();
		m_vdbClassReg = HK_NULL;
	}
}

hkDefaultDemo::~hkDefaultDemo()
{

#if HK_CONFIG_THREAD == HK_CONFIG_MULTI_THREADED

	if (m_jobThreadPool)
	{
		m_jobThreadPool->removeReferenceLockUnchecked();
	}

	delete m_jobQueue;

	if (m_spuUtil)
	{

		#if defined (HK_PLATFORM_PS3_PPU)
			// free resources if necessary
			int ret = cellSpursFinalize ( hkGetSpursInstance() );
			if (ret)
			{
				HK_ERROR(0x73e432b3, "cellSpursFinalize failed :" << ret);
			}
			void* spurs = hkGetSpursInstance();

			hkAlignedDeallocate<char>( (char*)spurs );
		#endif
	}
#endif

	for ( int i = 0; i < m_steppers.getSize(); ++i )
	{
		m_steppers[i]->removeReference();
	}

	if(m_mouseActive)
	{
		mouseUp();
	}
	if ( m_lastProgress )
	{
		delete m_lastProgress;
	}

	cleanupGraphics();

	for( int i = 0; i < m_delayedCleanup.getSize(); ++i )
	{
		m_delayedCleanup[i]->removeReference();
	}

	if (m_forcedShadowsOn)
	{
		m_env->m_options->m_enableShadows = false; // reset
	}
	else if (m_forcedShadowsOff)
	{
		m_env->m_options->m_enableShadows = true;// reset
	}
	
#if defined (HK_USE_CHARACTER_FACTORY)
 	if (m_characterFactory)
 	{
 		delete m_characterFactory;
 	}
#endif
}


void hkDefaultDemo::setupContexts(hkArray<hkProcessContext*>& contexts)
{
	// m_debugViewerNames.pushBack( hkStatisticsProcess::getName()  );
	m_debugViewerNames.pushBack( hkDebugDisplayProcess::getName() );
}

hkDemo::Result hkDefaultDemo::stepVisualDebugger()
{
	if(m_vdb)
	{
		m_vdb->step(m_timestep * 1000); // VDB frame time in ms, m_timestep is in secs
	}
	return DEMO_OK;
}

void hkDefaultDemo::cleanupGraphics()
{
	shutdownVDB();

	m_env->m_window->getContext()->lock();

	if (m_env->m_sceneConverter)
	{
		m_env->m_sceneConverter->purgeHeldMappings();
		m_env->m_sceneConverter->removeCreatedObjectsFromWorld();

		// reset some options
		m_env->m_sceneConverter->revertToDefaultSettings();
	}

	// Should not need to remove 'extra' objects from the world if the demo behaves itself, but if the need be, turn this back on
	if (m_env->m_displayWorld)
	{
		m_env->m_displayWorld->lock();
		// See if anything is left to remove, as the world should be empty always at this stage
		// but only if only running one demo at once (see multithread tests(
		while (m_env->m_displayWorld->getNumDisplayObjects() > 0)
		{
			hkgDisplayObject* o = m_env->m_displayWorld->safeRemoveDisplayObject(m_env->m_displayWorld->getNumDisplayObjects()-1);
			o->removeReference();
		}
		m_env->m_displayWorld->unlock();
	}

	m_env->m_displayHandler->clearDisplay();
	m_env->m_displayWorld->unsort();

	if (m_env->m_window)
	{
		m_env->m_window->getViewport(0)->setSkyBox(HK_NULL);
		m_env->m_window->setShadowMapMode( HKG_SHADOWMAP_MODE_DEFAULT, HK_NULL );
	}

	m_env->m_window->getContext()->unlock();
}

void hkDefaultDemo::setupGraphics()
{
	if (!m_env->m_options)
	{
		return;
	}

	// do we need a class reg. If so make it now so that the demo specifics can set it up in setupContexts
	if(m_env->m_options->m_debugger)
	{
		if (m_vdbClassReg) m_vdbClassReg->removeReference();
		m_vdbClassReg = new hkVtableClassRegistry;
	}


	setupContexts(m_contexts);

	for (int dvi=0; dvi < m_debugViewerNames.getSize(); ++dvi)
	{
		hkProcess* p = hkProcessFactory::getInstance().createProcess( m_debugViewerNames[dvi].cString(), m_contexts );
		if (p)
			{
				p->m_inStream = HK_NULL; // no streams
				p->m_outStream = HK_NULL;		
				p->m_displayHandler = m_env->m_displayHandler;
				p->m_processHandler = HK_NULL; // no process handler
				p->init();
				m_debugProcesses.pushBack(p); // so we can delete them
			}
		}

	setupVisualDebugger(m_contexts, m_env->m_options->m_debugger, HK_NULL);
}


void hkDefaultDemo::setGraphicsState(HKG_ENABLED_STATE state, hkBool on)
{
	hkgViewport* v = m_env->m_window->getCurrentViewport();
	HKG_ENABLED_STATE currentState = v->getDesiredState();
	if (on)
		currentState |= state; // turn on
	else
		currentState &= ~state; // turn off
	v->setDesiredState(currentState);
}

void hkDefaultDemo::forceShadowState(bool shadowsOn)
{
	m_forcedShadowsOn = false;
	m_forcedShadowsOff = false;
	if (!m_env->m_options->m_forceNoShadows)
	{
		if (m_env->m_options->m_enableShadows != shadowsOn)
		{
			if (shadowsOn && !m_forcedShadowsOff)
			{
				m_forcedShadowsOn = true;
			}
			else if (!shadowsOn && !m_forcedShadowsOn)
			{
				m_forcedShadowsOff = true;
			}
		}
		m_env->m_options->m_enableShadows = shadowsOn;
	}
}


void hkDefaultDemo::setupDefaultCameras( hkDemoEnvironment* env, const hkVector4& from, const hkVector4& to, const hkVector4& up, const hkReal nearPlane, const hkReal farPlane, bool rightHanded) const
{
	hkgWindow* w = env->m_window;
	if ( !w )
	{
		return;
	}

	m_env->m_window->getContext()->lock();
	for(int i = 0; i < w->getNumViewports(); ++i)
	{
		hkgCamera* c = w->getViewport(i)->getCamera();

		float upN[3];
		hkgVec3Copy(upN, &up(0));
		hkgVec3Normalize(upN);
		// set up camera
		c->setFrom(&from(0));
		c->setTo(&to(0));
		c->setUp(upN);
		c->setFar(farPlane);
		c->setNear(nearPlane);
		c->orthogonalize();
		c->computeModelView();
		c->computeProjection();
		c->setHandednessMode(HKG_CAMERA_HANDEDNESS_MODE( rightHanded ? HKG_CAMERA_HANDEDNESS_RIGHT : HKG_CAMERA_HANDEDNESS_LEFT) );

		w->getViewport(i)->setFlyModeUp( upN );
	}
	m_env->m_window->getContext()->unlock();
}

void hkDefaultDemo::setupDefaultCameras( hkEnum<CameraAxis,int> upAxis, hkReal fromX, hkReal fromY, hkReal fromZ ) const
{
	hkVector4 from; from.set(fromX, fromY, fromZ);
	hkVector4 to; to.setZero4();
	hkVector4 up;
	switch(upAxis)
	{
		case CAMERA_AXIS_X: 
			up = hkQuadReal1000; break;
		case CAMERA_AXIS_Y: 
			up = hkQuadReal0100; break;
		case CAMERA_AXIS_Z:
		default:
			up = hkQuadReal0010; break;
	}
	setupDefaultCameras( m_env, from, to, up );
}


void HK_CALL hkDefaultDemo::setupSkyBox(hkDemoEnvironment* env, const char* skyBoxFileName)
{
	if (!skyBoxFileName)
{
		skyBoxFileName = "Resources/Common/Graphics/defaultskybox";
	}

	hkString rootName (skyBoxFileName);
	hkString upName = rootName + "_UP.png";
	hkString downName = rootName + "_DN.png";
	hkString leftName = rootName + "_LF.png";
	hkString rightName = rootName + "_RT.png";
	hkString frontName = rootName + "_FR.png";
	hkString backName = rootName + "_BK.png";

	// see which way is 'up'
	hkgCamera* cam = env->m_window->getViewport(0)->getCamera();

	float worldUp[3];
	cam->getBestAlignedUpVector(worldUp);

	hkgSkyBox* sky = hkgSkyBox::create();
	sky->setUpDirection( worldUp );

	const char* names[] = { upName.cString(), downName.cString(), leftName.cString(), rightName.cString(), frontName.cString(), backName.cString() };
	for (int i=0; i < 6; ++i)
	{
		hkIstream s(names[i]);
		if (!s.isOk())
			continue;

		hkgTexture* t = hkgTexture::create(env->m_window->getContext());
		t->loadFromPNG(s);
		t->realize();
		t->setTextureWrapModeU(HKG_TEXTURE_CLAMP);
		t->setTextureWrapModeV(HKG_TEXTURE_CLAMP);
		sky->setTexture((HKG_SKYBOX_FACE)i, t, 0xffffffff );
		t->release();
	}

	env->m_window->getViewport(0)->setSkyBox(sky);
	sky->release();
}

void HK_CALL hkDefaultDemo::loadingScreen(hkDemoEnvironment* env, const char* screenFile)
{
	hkgWindow* window = env->m_window;
	hkgDisplayContext* ctx = window->getContext();

	ctx->lock();

	hkgTexture* t = hkgTexture::create(ctx);

	const char* defaultScreen = "./Resources/Common/Graphics/loading_screen.png";
	if (screenFile == HK_NULL)
		screenFile = defaultScreen;

	t->loadFromFile(screenFile);
	t->realize();
	hkgViewport* orthoView = window->getWindowOrthoView();
	orthoView->setAsCurrent(ctx);

	float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	ctx->setCurrentSoleTexture( t, HKG_TEXTURE_MODULATE );
	t->removeReference();

	ctx->setTexture2DState( true ); // turn on textures for this
	ctx->setBlendState( false );

	ctx->setCurrentColor4( white );

	float p[3];
	float uv[2];
	p[2] = -0.01f; //depth

	float tl[2];
	float lr[2];
	tl[0] = (float)0;
	tl[1] = (float)window->getHeight();

	lr[0] = (float)window->getWidth();
	lr[1] = (float)0;

	ctx->beginGroup( HKG_IMM_TRIANGLE_LIST );

		p[0] = tl[0]; p[1] = tl[1];
		uv[0] = 0.0f; uv[1] = 1.0f;
		ctx->setCurrentTextureCoord( uv );
		ctx->setCurrentPosition( p );

		p[0] = tl[0]; p[1] = lr[1];
		uv[0] = 0.0f; uv[1] = 0.0f;
		ctx->setCurrentTextureCoord( uv );
		ctx->setCurrentPosition( p );

		p[0] = lr[0]; p[1] = tl[1];
		uv[0] = 1.0f; uv[1] = 1.0f;
		ctx->setCurrentTextureCoord( uv );
		ctx->setCurrentPosition( p );

		p[0] = tl[0]; p[1] = lr[1];
		uv[0] = 0.0f; uv[1] = 0.0f;
		ctx->setCurrentTextureCoord( uv );
		ctx->setCurrentPosition( p );

		p[0] = lr[0]; p[1] = tl[1];
		uv[0] = 1.0f; uv[1] = 1.0f;
		ctx->setCurrentTextureCoord( uv );
		ctx->setCurrentPosition( p );

		p[0] = lr[0]; p[1] = lr[1];
		uv[0] = 1.0f; uv[1] = 0.0f;
		ctx->setCurrentTextureCoord( uv );
		ctx->setCurrentPosition( p );

	ctx->endGroup();

	bool logoOn = window->getWantDrawHavokLogo();
	window->setWantDrawHavokLogo(false);
	window->swapBuffers(); // display
	window->clearBuffers(); // clear for demo step
	window->setWantDrawHavokLogo(logoOn);
	ctx->unlock();

}

class ProgressStreamReader : public hkStreamReader
{
	public:

		ProgressStreamReader( hkStreamReader* r, hkDefaultDemo* d, const char* fname)
			: m_child(r), m_demo(d)
		{
			//child already has 1 ref // m_child->addReference();
			m_demo->progressBegin(fname);
			m_fileSize = 4096;//m_child->getFileSize();
			m_bytesRead = 0;
		}

		~ProgressStreamReader()
		{
			m_demo->progressEnd();
			m_child->removeReference();
		}

		virtual int read(void* buf, int nbytes)
		{
			m_bytesRead += nbytes;
			m_demo->progressSet(hkReal(m_bytesRead) / m_fileSize);
			return m_child->read(buf, nbytes);
		}

		int m_fileSize;
		int m_bytesRead;
		hkStreamReader* m_child;
		hkDefaultDemo* m_demo;
};

void hkDefaultDemo::progressBegin(const char* name)
{
	if ( ! m_lastProgress )
	{
		m_lastProgress = new hkStopwatch();
	}
	m_lastProgress->start();
	m_progressName = name;
}

void hkDefaultDemo::progressEnd()
{
	m_lastProgress->stop();
	delete m_lastProgress;
	m_lastProgress = HK_NULL;
}

void hkDefaultDemo::progressSet(hkReal percent)
{
	if( m_lastProgress->getElapsedSeconds() >= (1.0f / 60.f) )
	{
		hkString bar = m_progressName;
		{
			const int NDOTS = 50;
			char spaces[NDOTS+4];
			spaces[0] = '\n';
			spaces[1] = '[';
			hkString::memSet(spaces+2, ' ', NDOTS);
			spaces[NDOTS+2] = ']';
			spaces[NDOTS+3] = 0;
			int end = (percent < 1) ? int(percent*NDOTS) : NDOTS;
			for(int i = 0; i < end; ++i)
			{
				spaces[2+i] = '#';
			}
			bar += spaces;
		}

		hkgWindow* window = m_env->m_window;
		window->clearBuffers();

		for(int i = 0; i < window->getNumViewports(); ++i )
		{
			window->getViewport(i)->setAsCurrent( window->getContext() );
			m_env->m_textDisplay->outputText(bar, 20, 20);
			m_env->m_textDisplay->displayText(window);
		}
		window->swapBuffers();

		m_lastProgress->reset();
		m_lastProgress->start();
	}
}

void hkDefaultDemo::mouseDown()
{
	HK_ASSERT(0x499454e1, m_mouseActive == false);

	// see if the user is picking - only if
	//   there is a display world!
	//   and we're not already picking
	hkgDisplayWorld* dw = m_env->m_displayWorld;
	if( m_env->m_mousePickingEnabled && dw )
	{
		hkgWindow* w = m_env->m_window;
		hkgViewport* v = w->getCurrentViewport();
		hkgCamera* c = v->getCamera();
		int vx, vy;
		v->getLowerLeftCoord(vx, vy);

		hkgViewportPickData pd;

		const int x = w->getMouse().getPosX() - vx;
		const int y = w->getMouse().getPosY() - vy;

		if (v->pick( x, y, dw, pd ))
		{
   			const int objectNum = pd.m_objectIndex;
			hkVector4 mousePosWorldSpace; mousePosWorldSpace.set( pd.m_worldPos[0], pd.m_worldPos[1], pd.m_worldPos[2]);

			float result[3];
			c->project( pd.m_worldPos[0], pd.m_worldPos[1], pd.m_worldPos[2], v->getWidth(), v->getHeight(), result );
			m_mousePosDepth = result[2];	// remember the z value

			//hkprintf( "GM: Try pick at [%d %d (%f)] ", x, y, m_mousePosDepth );
			//hkprintf( "World mouse pos: [%f %f %f]\n", pd.m_worldPos[0], pd.m_worldPos[1], pd.m_worldPos[2]);

			// find out if there is a (the first one) demo that has a rigid body attached
			// to this display object
			const hkgDisplayObject* dobject = dw->getDisplayObject( objectNum );
			m_mouseActive = objectPicked( dobject, mousePosWorldSpace, pd.m_objectPickData.m_geomIndex  );
		}
		else
		{
			m_mouseActive = objectPicked( HK_NULL, hkVector4::getZero(), 0  );
		}
	}
}

void hkDefaultDemo::mouseUp()
{
	if(m_mouseActive)
	{
		objectReleased();
		m_mouseActive = false;
	}
}

void hkDefaultDemo::mouseDrag()
{
	if(!m_mouseActive)
		return;

	// find the new world space point
	hkgWindow* w = m_env->m_window;
	hkgViewport* v = w->getCurrentViewport();
	hkgCamera* c = v->getCamera();

	int vx, vy;
	v->getLowerLeftCoord(vx, vy);

	const int x = w->getMouse().getPosX() - vx;
	const int y = w->getMouse().getPosY() - vy;

	hkVector4 mousePosWorldSpace;	mousePosWorldSpace.setZero4();


	c->unProject( (x - vx), (y - vy), m_mousePosDepth, v->getWidth(), v->getHeight(), &mousePosWorldSpace(0) );

	objectDragged( mousePosWorldSpace );
}

//
// Visual Debugger Utility Functions
//

// Call this function to setup the visualdebugger
// The default is to run a network server

void hkDefaultDemo::setupVisualDebugger(const hkArray<hkProcessContext*>& contexts, hkBool runServer, const char* captureFile)
{
	// Create it
	if (m_vdb)
		m_vdb->removeReference();

	m_vdb = HK_NULL;

	if(runServer || captureFile)
	{
		m_vdb = new hkVisualDebugger(contexts, m_vdbClassReg );

		// Add/Remove viewers from the default list
		//vdb->addDefaultProcess("Islands");
		//vdb->addDefaultProcess("Broadphase");
		//vdb->addDefaultProcess("Statistics");
		//vdb->addDefaultProcess("Tensor");
		//vdb->addDefaultProcess("CentreOfMass");
		//vdb->addDefaultProcess("ContactPoints");

		// Start the server if it has been requested
		if(runServer)
		{
			m_vdb->serve();
		}

		// Capture the simulation information to a file
		if(captureFile)
		{
			m_vdb->capture(captureFile);
		}
	}
}

hkProcess* hkDefaultDemo::getLocalViewerByName( const char* name )
{
	int tag = hkProcessFactory::getInstance().getProcessId( name );
	for (int pi = 0; pi < m_debugProcesses.getSize(); ++pi)
	{
		if (m_debugProcesses[pi]->getProcessTag() == tag)
		{
			return m_debugProcesses[pi];
		}
	}
	return HK_NULL;
}

void hkDefaultDemo::getVDBViewersByName( const char* name, hkArray<hkProcess*>& processes )
{
	if (m_vdb)
	{
		int tag = hkProcessFactory::getInstance().getProcessId( name );
		m_vdb->getCurrentProcesses( processes );

		int np = processes.getSize();
		for (int pi = np -1; pi >= 0; --pi)
		{
			if (processes[pi]->getProcessTag() != tag)
				processes.removeAt(pi);
		}
	}
}


void HK_CALL hkDefaultDemo::removeLights(hkDemoEnvironment* env)
{
	// make some default lights
	hkgLightManager* lm = env->m_displayWorld->getLightManager();
	if (lm)
	{
		lm->lock();
		// clear out the lights currently in the world.
		while( lm->getNumLights() > 0 )
		{
			hkgLight* l = lm->removeLight(0); // gives back reference
			l->release();
		}
		lm->unlock();
	}
}

// handy for simple demos
void HK_CALL hkDefaultDemo::setSoleDirectionLight(hkDemoEnvironment* env, float dir[3], hkUint32 color)
{
	// make some default lights
	hkgLightManager* lm = env->m_displayWorld->getLightManager();

	if (!lm)
	{
		lm = hkgLightManager::create();
		env->m_displayWorld->setLightManager( lm );
		lm->release();
		lm->lock();
	}
	else
	{
		lm->lock();
		// clear out the lights currently in the world.
		while( lm->getNumLights() > 0 )
		{
			hkgLight* l = lm->removeLight(0); // gives back reference
			l->release();
		}
	}

	// Background color
	float bg[4] = { 0.53f, 0.55f, 0.61f, 1 };
	env->m_window->setClearColor( bg );

	float v[4]; v[3] = 255;
	hkgLight* light;

	// the sun (direction downwards)
	{
		light = hkgLight::create();
		light->setType( HKG_LIGHT_DIRECTIONAL );
		float rgba[4];
		hkgColor4UnpackToVec4(rgba, color);
		light->setDiffuse( rgba );
		light->setSpecular( rgba );
		float ndir[3];
		hkgVec3Copy(ndir, dir);
		hkgVec3Normalize( ndir );
		light->setDirection( ndir );
		light->setPosition( HKG_VEC3_ZERO ); // not used as directional
		light->setDesiredEnabledState( true );
		lm->addLight( light );

		// float shadowPlane[] = { 0,1,0,-0.01f };
		// light->addShadowPlane( shadowPlane );
		light->release();
	}

	lm->computeActiveSet( HKG_VEC3_ZERO );
	lm->unlock();
}

void HK_CALL hkDefaultDemo::setupLights(hkDemoEnvironment* env)
{
	// make some default lights
	hkgLightManager* lm = env->m_displayWorld->getLightManager();

	if (!lm)
	{
		lm = hkgLightManager::create();
		env->m_displayWorld->setLightManager( lm );
		lm->release();
		lm->lock();
	}
	else
	{
		lm->lock();
		// clear out the lights currently in the world.
		while( lm->getNumLights() > 0 )
		{
			hkgLight* l = lm->removeLight(0); // gives back reference
			l->release();
		}
	}

	// Background color
	float bg[4] = { 0.53f, 0.55f, 0.61f, 1 };
	env->m_window->setClearColor( bg );

	float v[4]; v[3] = 255;
	hkgLight* light;


	// the sun (direction downwards)
	{
		light = hkgLight::create();
		light->setType( HKG_LIGHT_DIRECTIONAL );
		v[0] = 256;
		v[1] = 256;
		v[2] = 256;
		v[0] /= 255; v[1] /= 255; v[2] /= 255;
		light->setDiffuse( v );
		light->setSpecular( v );
		v[0] = 0;
		v[1] = -1;
		v[2] = -0.5f;
		light->setDirection( v );
		v[0] = 0;
		v[1] = 1000;
		v[2] = 0;
		light->setPosition( v );
		light->setDesiredEnabledState( true );
		lm->addLight( light );

		// float shadowPlane[] = { 0,1,0,-0.01f };
		// light->addShadowPlane( shadowPlane );
		light->release();
	}

	hkgAabb areaOfInterest;
	areaOfInterest.m_max[0] = 10;
	areaOfInterest.m_max[1] = 10;
	areaOfInterest.m_max[2] = 10;
	areaOfInterest.m_min[0] = -10;
	areaOfInterest.m_min[1] = -10;
	areaOfInterest.m_min[2] = -10;
	setupFixedShadowFrustum( env, *light, areaOfInterest );

	// if se have shadow maps we only support on light by default (or else it looks dodge)
	if (!env->m_options->m_enableShadows || (env->m_window->getShadowMapSupport() == HKG_SHADOWMAP_NOSUPPORT) )
	{
		// fill 1 - blue
		{
			light = hkgLight::create();
			light->setType( HKG_LIGHT_DIRECTIONAL );
			v[0] = 200;
			v[1] = 200;
			v[2] = 240;
			v[0] /= 255; v[1] /= 255; v[2] /= 255;
			light->setDiffuse( v );
			v[0] = 1;
			v[1] = 1;
			v[2] = 1;
			light->setDirection( v );
			v[0] = -1000;
			v[1] = -1000;
			v[2] = -1000;
			light->setPosition( v );
			light->setDesiredEnabledState( true );
			lm->addLight( light );
			light->release();
		}

		// fill 2 - yellow
		{
			light = hkgLight::create();
			light->setType( HKG_LIGHT_DIRECTIONAL );
			v[0] = 240;
			v[1] = 240;
			v[2] = 200;
			v[0] /= 255; v[1] /= 255; v[2] /= 255;
			light->setDiffuse( v );
			v[0] = -1;
			v[1] = 1;
			v[2] = -1;
			light->setDirection( v );
			v[0] = 1000;
			v[1] = -1000;
			v[2] = 1000;
			light->setPosition( v );
			light->setDesiredEnabledState( true );
			lm->addLight( light );
			light->release();
		}
	}

	lm->computeActiveSet( HKG_VEC3_ZERO );
	lm->unlock();
}

void HK_CALL hkDefaultDemo::setupFixedShadowFrustum( hkDemoEnvironment* env, hkgLight& light, const hkgAabb& areaOfInterest, float extraNear, float extraFar )
{
	hkgCamera* lightCam = hkgCamera::createFixedShadowFrustumCamera( light, areaOfInterest, true, extraNear, extraFar  );
	env->m_window->setShadowMapMode(HKG_SHADOWMAP_MODE_FIXED, lightCam);
	lightCam->removeReference();
}

void hkDefaultDemo::setLightAndFixedShadow(float* lightDir, float* shadowAabbMin, float* shadowAabbMax, float extraNear, float extraFar )
{
	setSoleDirectionLight(m_env, lightDir, 0xffffffff );
	hkgAabb aabb( shadowAabbMin, shadowAabbMax );
	setupFixedShadowFrustum(m_env, *(m_env->m_displayWorld->getLightManager()->getLight(0)), aabb, extraNear, extraFar );
}


int hkDefaultDemo::getNumSpus()
{
	#ifdef HK_PLATFORM_PS3_PPU
		return m_env->m_options->m_numSpus;
	#else
		return 1;
	#endif
}

void hkDefaultDemo::addStepper( DemoStepper* stepper )
{
	m_steppers.pushBack(stepper);
}

hkDemo::Result hkDefaultDemo::stepDemo()
{
	hkDemo::Result result = DEMO_OK;
	for ( int i = 0; i < m_steppers.getSize(); ++i )
	{
		result = m_steppers[i]->stepDemo( this );
		if (result != DEMO_OK)
		{
			return result;
		}
	}

	if ( m_allowChangingNumThreads )
	{
	addOrRemoveThreads();
	}
	return result;
}

void hkDefaultDemo::addOrRemoveThreads()
{
	//
	// Add/remove threads
	//
	if (m_env->m_window)
	{
		if (m_jobThreadPool)
		{
			if (m_env->m_window->getKeyboard().wasKeyPressed('T') ||
				m_env->m_window->getGamePad(0).wasButtonPressed(HKG_PAD_BUTTON_R1) )
			{
				int num = m_jobThreadPool->getNumThreads();

				hkHardwareInfo info;
				hkGetHardwareInfo(info);
				if (num < info.m_numThreads - 1) num++;
				m_jobThreadPool->setNumThreads( num );
			}
			if ( m_env->m_window->getKeyboard().wasKeyPressed('G') ||
				m_env->m_window->getGamePad(0).wasButtonPressed(HKG_PAD_BUTTON_R2) )
			{
				int num = m_jobThreadPool->getNumThreads();
				if (num > 0) num--;
				m_jobThreadPool->setNumThreads( num );
			}
		}
	}
}


void hkDefaultDemo::getTimerStreamInfo( hkArray<hkTimerData>& threadStreams, hkArray<hkTimerData>& spuStreams, int maxThreads )
{
	hkTimerData info;
	info.m_streamBegin = hkMonitorStream::getInstance().getStart();
	info.m_streamEnd = hkMonitorStream::getInstance().getEnd();
	threadStreams.pushBack(info);

#if defined HK_PLATFORM_PS3_PPU

	if ( m_jobThreadPool != HK_NULL )
	{
		m_jobThreadPool->appendTimerData( spuStreams );
	}
#else
	if ( m_jobThreadPool != HK_NULL )
	{
		m_jobThreadPool->appendTimerData( threadStreams );
	}

#endif
}

void hkDefaultDemo::getNumTimerStreams( int& numThreadStreams, int& numSpuStreams, int maxThreads ) const
{
#if defined HK_PLATFORM_PS3_PPU
	numThreadStreams = 1;
	numSpuStreams = m_jobThreadPool == HK_NULL ? 0 : m_jobThreadPool->getNumThreads();
#else
	numThreadStreams = m_jobThreadPool == HK_NULL ? 1 : m_jobThreadPool->getNumThreads() + 1;
	numSpuStreams = 0;
#endif
}

void hkDefaultDemo::resetTimerStreams()
{
	hkMonitorStream::getInstance().reset();
	if ( m_jobThreadPool != HK_NULL )
	{
		m_jobThreadPool->clearTimerData();
	}

	if ( m_jobThreadPool != HK_NULL )
	{
		m_jobThreadPool->clearTimerData();
	}

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
