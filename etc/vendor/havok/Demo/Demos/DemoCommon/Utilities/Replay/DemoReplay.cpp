/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/Replay/DemoReplay.h>

#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Base/System/Io/OStream/hkOStream.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.h>
#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileReader.h>
#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileWriter.h>

extern const hkClass		hkDemoReplayUtilityCameraClass;
extern const hkTypeInfo		hkDemoReplayUtilityCameraTypeInfo;
extern const hkClass		hkDemoReplayUtilityFrameClass;
extern const hkTypeInfo		hkDemoReplayUtilityFrameTypeInfo;
extern const hkClass		hkDemoReplayUtilityReplayDataClass;
extern const hkTypeInfo		hkDemoReplayUtilityReplayDataTypeInfo;
extern const hkClass		hkDemoReplayUtilityMouseCallbacksClass;
extern const hkTypeInfo		hkDemoReplayUtilityMouseCallbacksTypeInfo;
extern const hkClass		hkDemoReplayUtilityClass;
extern const hkTypeInfo		hkDemoReplayUtilityTypeInfo;

hkDemoReplayUtility::hkDemoReplayUtility( void )
: m_replayData(HK_NULL)
, m_currentFrame(0)
, m_allocatedData(HK_NULL)
{
	/// Register this class with the serialization system.
	hkBuiltinTypeRegistry::getInstance().addType(&hkDemoReplayUtilityCameraTypeInfo, &hkDemoReplayUtilityCameraClass);
	hkBuiltinTypeRegistry::getInstance().addType(&hkDemoReplayUtilityFrameTypeInfo, &hkDemoReplayUtilityFrameClass);
	hkBuiltinTypeRegistry::getInstance().addType(&hkDemoReplayUtilityReplayDataTypeInfo, &hkDemoReplayUtilityReplayDataClass);
	hkBuiltinTypeRegistry::getInstance().addType(&hkDemoReplayUtilityMouseCallbacksTypeInfo, &hkDemoReplayUtilityMouseCallbacksClass);
	hkBuiltinTypeRegistry::getInstance().addType(&hkDemoReplayUtilityTypeInfo, &hkDemoReplayUtilityClass);
}

hkDemoReplayUtility::~hkDemoReplayUtility ( void )
{
	if (m_allocatedData) m_allocatedData->removeReference();
	if (m_replayData) m_replayData->removeReference();
}

bool HK_CALL hkDemoReplayUtility::DummyMouseMoveFunc(hkgWindow* w, int x, int y, int dx, int dy, void* userContext)
{
	return true;
}

bool HK_CALL hkDemoReplayUtility::DummyMouseWheelFunc(hkgWindow* w, int delta, void* userContext)
{
	return true;
}

bool HK_CALL hkDemoReplayUtility::DummyMouseButtonFunc(hkgWindow* w, HKG_MOUSE_BUTTON button, bool state, void* userContext)
{
	return true;
}

void hkDemoReplayUtility::hijackInputCallbacks( hkDemoEnvironment* env )
{
	//HK_ASSERT(0xdbc27b22, !m_oldMouseCallbacks.m_mouseMoveFunc);
	//HK_ASSERT(0xdbc27b22, !m_oldMouseCallbacks.m_mouseWheelFunc);
	//HK_ASSERT(0xdbc27b22, !m_oldMouseCallbacks.m_mouseButtonFunc);

	//m_oldMouseCallbacks.m_mouseMoveFunc	= env->m_window->m_mouseMoveFunc;
	//m_oldMouseCallbacks.m_mouseWheelFunc	= env->m_window->m_mouseWheelFunc;
	//m_oldMouseCallbacks.m_mouseButtonFunc	= env->m_window->m_mouseButtonFunc;

	//env->m_window->setMouseMoveFunction(hkDemoReplayUtility::DummyMouseMoveFunc);
	//env->m_window->setMouseWheelFunction(hkDemoReplayUtility::DummyMouseWheelFunc);
	//env->m_window->setMouseButtonFunction(hkDemoReplayUtility::DummyMouseButtonFunc);
}

void hkDemoReplayUtility::restoreInputCallbacks ( hkDemoEnvironment* env )
{
	//HK_ASSERT(0xdbc27b21, m_oldMouseCallbacks.m_mouseMoveFunc);
	//HK_ASSERT(0xdbc27b21, m_oldMouseCallbacks.m_mouseWheelFunc);
	//HK_ASSERT(0xdbc27b21, m_oldMouseCallbacks.m_mouseButtonFunc);

	//env->m_window->setMouseMoveFunction(m_oldMouseCallbacks.m_mouseMoveFunc);
	//env->m_window->setMouseWheelFunction(m_oldMouseCallbacks.m_mouseWheelFunc);
	//env->m_window->setMouseButtonFunction(m_oldMouseCallbacks.m_mouseButtonFunc);

	//m_oldMouseCallbacks.m_mouseMoveFunc	= HK_NULL;
	//m_oldMouseCallbacks.m_mouseWheelFunc	= HK_NULL;
	//m_oldMouseCallbacks.m_mouseButtonFunc	= HK_NULL;
}

void hkDemoReplayUtility::startPlayback(const char* filename, hkDemoEnvironment* env)
{
	hijackInputCallbacks(env);

	delete m_replayData;
	m_replayData	= HK_NULL;
	m_currentFrame	= 0;

	hkIstream infile(filename);

	HK_ASSERT2(0xdbc3dba1, infile.isOk(), "Cannot open file");

	hkBinaryPackfileReader reader;

	reader.loadEntireFile(infile.getStreamReader());
	m_allocatedData = reader.getAllocatedData();
	m_allocatedData->addReference();

	m_replayData = reinterpret_cast<ReplayData*>(reader.getContents(hkDemoReplayUtilityReplayDataClass.getName()));
	HK_ASSERT2(0xdbc3dba1, m_replayData, "No replay data found in file");
}

void hkDemoReplayUtility::startRecording(void)
{
	HK_ASSERT2(0xdbc182bd, m_replayData == HK_NULL, "Replay data is not null. Have you stopped the last recording?");
	m_replayData = new ReplayData();
	m_currentFrame = 0;
}

void hkDemoReplayUtility::endRecordingAndSave( const char* filename )
{
	hkReferencedObject::lockAll();

	hkOstream ostream( filename );
	if ( !ostream.isOk() )
	{
		hkReferencedObject::unlockAll();
		return;
	}

	hkBinaryPackfileWriter writer;
	writer.setContents(m_replayData, hkDemoReplayUtilityReplayDataClass);
	hkBinaryPackfileWriter::Options options;
	options.m_writeMetaInfo = false;
	options.m_userTag = m_replayData->m_frames.getSize();
	writer.save(ostream.getStreamWriter(), options);
	m_currentFrame = 0;
	
	if (m_replayData)
	{
		m_replayData->removeReference();
		m_replayData = HK_NULL;
	}
	hkReferencedObject::unlockAll();
}

void hkDemoReplayUtility::endPlayback ( hkDemoEnvironment* env )
{
	restoreInputCallbacks(env);
}

hkDemoReplayUtility::ReplayData::ReplayData ( void )
: m_framesWereAllocatedExternally(false)
{
}

hkDemoReplayUtility::ReplayData::ReplayData ( hkFinishLoadedObjectFlag flag )
: hkReferencedObject(flag),
  m_frames(flag)
{
	if( flag.m_finishing )
	{
		m_framesWereAllocatedExternally = true;
	}
}

hkDemoReplayUtility::ReplayData::~ReplayData()
{
	// Internal allocation, must be freed manually
	if (!m_framesWereAllocatedExternally)
	{
		for (int i=0;  i<m_frames.getSize(); i++)
		{
			delete m_frames[i];
		}
	}
}

hkDemoReplayUtility::Frame::Frame ( void )
: m_keysDown(HKG_KEYBOARD_NUM_VKEYS, 0)
{
}

hkDemoReplayUtility::Frame::Frame ( hkFinishLoadedObjectFlag flag )
: m_keysDown(flag)
{
}


void hkDemoReplayUtility::recordFrame( hkDemoEnvironment* env )
{
	if (m_currentFrame == 0)
	{
		m_replayData->m_initialMouseX = env->m_window->getMouse().getPosX();
		m_replayData->m_initialMouseY = env->m_window->getMouse().getPosY();
	}

	Frame* f = new Frame();

	for (HKG_KEYBOARD_VKEY k=0; k<HKG_KEYBOARD_NUM_VKEYS; k++)
	{
		f->m_keysDown.assign(k, env->m_window->getKeyboard().getKeyState(k));
	}

	f->m_padButtons = env->m_gamePad->getButtonState();

	f->m_sticks[0] = env->m_gamePad->getStickPosX(0);
	f->m_sticks[1] = env->m_gamePad->getStickPosY(0);
	f->m_sticks[2] = env->m_gamePad->getStickPosX(1);
	f->m_sticks[3] = env->m_gamePad->getStickPosY(1);

	hkgCamera* cam = env->m_window->getViewport(0)->getCamera();
	hkgVec3Copy( f->m_camera.m_from, cam->getFromPtr() );
	hkgVec3Copy( f->m_camera.m_to, cam->getToPtr() );
	hkgVec3Copy( f->m_camera.m_up, cam->getUpPtr() );

	{
		const hkgMouse& m = env->m_window->getMouse();
		f->m_mouseX = m.getPosX();
		f->m_mouseY = m.getPosY();
		f->m_mouseButtons = m.getButtonState();
	}

	env->m_textDisplay->outputText("RECORDING", 10,10,0x7F7F0000, 1);
	m_replayData->m_frames.pushBack(f);
	m_currentFrame++;
}


void hkDemoReplayUtility::playbackFrame( hkDemoEnvironment* env )
{
	if (m_currentFrame >= m_replayData->m_frames.getSize())
	{
		// No more data
		env->m_textDisplay->outputText("END OF REPLAY", 10,10,0x7F7F0000, 1);
		return;
	}

	if (m_currentFrame == 0)
	{
		env->m_window->setMousePosition(m_replayData->m_initialMouseX, m_replayData->m_initialMouseY);
	}

	Frame* f = m_replayData->m_frames[m_currentFrame];
	m_currentFrame++;

	for (HKG_KEYBOARD_VKEY i=0; i<HKG_KEYBOARD_NUM_VKEYS; i++)
	{
		const_cast<hkgKeyboard&>(env->m_window->getKeyboard()).setCurrentKeyState(i, !!f->m_keysDown.get(i));
	}

	env->m_gamePad->setButtonState(f->m_padButtons);

	hkgPad::Stick& stick1 = const_cast<hkgPad::Stick&>(env->m_gamePad->getStickState(0));
	hkgPad::Stick& stick2 = const_cast<hkgPad::Stick&>(env->m_gamePad->getStickState(1));
	stick1.m_cur[0] = f->m_sticks[0];
	stick1.m_cur[1] = f->m_sticks[1];
	stick2.m_cur[0] = f->m_sticks[2];
	stick2.m_cur[1] = f->m_sticks[3];

	// overwrite camera
	hkgCamera* cam = env->m_window->getViewport(0)->getCamera();
	cam->setFrom(f->m_camera.m_from);
	cam->setTo(f->m_camera.m_to);
	cam->setUp(f->m_camera.m_up);
	cam->computeModelView();
	cam->computeProjection();


	hkgMouse& mouse = const_cast<hkgMouse&>(env->m_window->getMouse());
	env->m_window->processMouseMove(f->m_mouseX, f->m_mouseY, true);
	mouse.setButtonState(f->m_mouseButtons);

	env->m_textDisplay->outputText("REPLAY", 10,10,0x7F7F0000, 1);
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
