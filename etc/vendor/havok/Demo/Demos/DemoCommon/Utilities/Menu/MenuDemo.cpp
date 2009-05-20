/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/Menu/MenuDemo.h>
#include <Demos/DemoCommon/Utilities/Tweaker/Tweaker.h>
#include <Common/Base/Memory/Memory/hkMemory.h>
#include <Common/Base/System/Stopwatch/hkStopwatch.h>
#include <Common/Base/Container/String/hkString.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Base/DebugUtil/StatisticsCollector/Simple/hkSimpleStatisticsCollector.h>
#include <Common/Base/Config/hkConfigVersion.h>
#include <Common/Base/Algorithm/Sort/hkSort.h>
#include <Common/Serialize/Version/hkVersionUtil.h>

#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/Texture/hkgTexture.h>

#include <Demos/DemoCommon/DemoFramework/hkPerformanceCounterUtility.h>

#include <Common/Serialize/Util/hkBuiltinTypeRegistry.h>

#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>
#include <Graphics/Bridge/StatGraph/hkgStatGraph.h>
#include <Graphics/Bridge/System/hkgSystem.h>
#include <Graphics/Common/Font/hkgFont.h>
#include <Common/Visualize/hkProcessFactory.h>

#define HK_UNIT_TESTS

#if defined(HK_UNIT_TESTS)
void HK_CALL registerUnitTests( hkArray<hkString*>& extraNames, hkArray<hkDemoEntryRegister*>& allocedDemos);
#endif


extern const unsigned int MenuImageDataSize;
extern const unsigned char MenuImageData[];
extern const unsigned int MenuImageLargeDataSize;
extern const unsigned char MenuImageLargeData[];

// colours for the menu text, normal and current active item.
const hkUint32 TEXT_COLOR_NORMAL = 0xffffffff;
const hkUint32 TEXT_COLOR_DIRECTORY = 0x7fffffff;
const hkUint32 TEXT_COLOR_CURRENT = 0xffc8c8eb;

const float MENU_BACKGROUND_COLOR[] = {  30.0f/255.0f,  30.0f/255.0f, 100.0f/255.0f };
const float MENU_BACKGROUND_CHANGE[] = {  0.0005f,  -0.0005f, 0.0005f };
const float DEMO_BACKGROUND_COLOR[] = { 100.0f/255.0f, 100.0f/255.0f, 150.0f/255.0f };

// last demo
const char* LASTDEMO_FILENAME = "lastdemo.txt";
const char* LASTDEMO_FILENAME_BOOTSTRAP = "lastdemoBootstrap.txt";

MenuDemo* MenuDemo::m_menuDemo = HK_NULL;

MenuDemo* MenuDemo::getMenuDemo()
{
	return m_menuDemo;
}

bool MenuDemo::isHelpOn()
{
	return ( m_helpTimeLeft > 0.0f );
}

void MenuDemo::turnOffHelp()
{
	m_helpTimeLeft = 0.0f;
}

bool MenuDemo::areMonitorsOn()
{
	return m_wantMonitors;
}

void hkSetLastDemo(const char* namein)
{
	hkOfstream out(LASTDEMO_FILENAME);
	if(out.isOk())
	{
		out << namein;
	}
// 	hkString name = namein;
// 	int lastSlash = name.lastIndexOf('/');
// 	if(lastSlash >= 0)
// 	{
// 		// Never save bootstrap demos as last game
// 		if ( hkString::strStr(name.asLowerCase().cString(), "bootstrap") == HK_NULL )
// 		{
// 			hkString::strNcpy(s_lastDemoName, name.cString(), sizeof(s_lastDemoName));
// 			hkOfstream out(LASTDEMO_FILENAME);
// 			if(out.isOk())
// 			{
// 				out << s_lastDemoName;
// 			}
// 		}
// 	}
// 	return s_lastDemoName;
}

extern void hkSetLastDemoBootstrap(const char* namein);

// Insert spaces between consecutive lower and upper case letters
static hkString insertSpaces(hkString str)
{
	for(int j=1; j< str.getLength(); j++)
	{
		char c1 = str[j-1];
		char c2 = str[j];
		if( (c1>='a'&&c1<='z') && ( (c2>='A'&&c2<='Z') || (c2>='0'&&c2<='9') ) )
		{
			str = str.substr(0, j) + " " + str.substr(j);
		}
	}
	return str;
}

static hkString canonicalise( hkString s )
{
	hkArray<char> buf;
	buf.reserve( s.getLength()+1 );
	for( const char* p = s.cString(); *p != 0; ++p )
	{
		char c = *p;
		if( (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || c == '/')
		{
			buf.pushBackUnchecked( c );
		}
		else if( c >= 'A' && c <= 'Z' )
		{
			buf.pushBackUnchecked( c + ('a' - 'A') );
		}
	}
	buf.pushBackUnchecked(0);
	return hkString(buf.begin());
}



void HK_CALL hkDrawMenuImage( hkgTexture* t, hkDemoEnvironment* env)
{
	hkgWindow* w = env->m_window;
	hkgDisplayContext* c = w->getContext();
	c->lock();

	hkgViewport* cv = w->getCurrentViewport();
	hkgViewport* v = w->getWindowOrthoView();
	v->setAsCurrent(c);

	float white[4] = { 1.0f, 1.0f, 1.0f, 0.70f}; // change alpha to fade logo

	c->setCurrentSoleTexture( t, HKG_TEXTURE_MODULATE );

	c->setTexture2DState( true ); // turn on textures for this
	c->setBlendMode(HKG_BLEND_MODULATE); // should be the default anyway
	c->setBlendState( true ); // modulate with background color
	c->setCurrentColor4( white );

	float p[3];
	float uv[2];
	p[2] = -0.01f; //depth

	float tl[3];
	float lr[3];


	unsigned int wh = w->getHeight() / 2;
	unsigned int ww = w->getWidth() / 2;

	unsigned int logoH = wh * 2;
	unsigned int logoW = ww * 2;

	// keep square
	if (logoH < logoW)
		logoW = logoH;
	if (logoW < logoH)
		logoH = logoW;

	// adjust if widescreen and not in a widescreen res
	if (w->isWidescreen() && !w->hasWidescreenAspect())
	{
		logoW = (int)( ((float)logoW) * 3.0f/4.0f ); // squash in the logo by its adjusted aspect
	}

	tl[0] = (float)(ww - logoW/2);
	tl[1] = (float)(wh + logoH/2 );

	lr[0] = (float)(ww + logoW/2);
	lr[1] = (float)(wh - logoH/2);

	c->beginGroup( HKG_IMM_TRIANGLE_LIST );

	p[0] = tl[0]; p[1] = tl[1];
	uv[0] = 0.0f; uv[1] = 1.0f;
	c->setCurrentTextureCoord( uv );
	c->setCurrentPosition( p );

	p[0] = tl[0]; p[1] = lr[1];
	uv[0] = 0.0f; uv[1] = 0.0f;
	c->setCurrentTextureCoord( uv );
	c->setCurrentPosition( p );

	p[0] = lr[0]; p[1] = tl[1];
	uv[0] = 1.0f; uv[1] = 1.0f;
	c->setCurrentTextureCoord( uv );
	c->setCurrentPosition( p );

	p[0] = tl[0]; p[1] = lr[1];
	uv[0] = 0.0f; uv[1] = 0.0f;
	c->setCurrentTextureCoord( uv );
	c->setCurrentPosition( p );

	p[0] = lr[0]; p[1] = tl[1];
	uv[0] = 1.0f; uv[1] = 1.0f;
	c->setCurrentTextureCoord( uv );
	c->setCurrentPosition( p );

	p[0] = lr[0]; p[1] = lr[1];
	uv[0] = 1.0f; uv[1] = 0.0f;
	c->setCurrentTextureCoord( uv );
	c->setCurrentPosition( p );

	c->endGroup();

	// restore
	cv->setAsCurrent(c);

	c->unlock();
}



//
// Menu Constructor
//
MenuDemo::MenuDemo(hkDemoEnvironment* environment)
	:	hkDefaultDemo(environment, true),
		m_defaultDemo(environment->m_options->m_defaultDemo),
		m_currentPath(""),
		m_currentDemo(HK_NULL),
		m_helpTimeMax(15.0f),
		m_helpTimeLeft(15.0f),
		m_paused(false),
		m_wantMonitors(false),
		m_wantTweak(false),
		m_wantCameraData(false),
		m_activeNode(HK_NULL),
		m_statsTree(HK_NULL),
		m_mousePadTrackedButton(-1),
		m_mouseInMenuOverlay(false),
		m_mouseStatZoomOriginX(0),
		m_mouseStatZoomOriginY(0),
		m_viewportMouseEventBackup(true),
		m_searchIndex(0),
		m_tweaker(HK_NULL),
		m_statGraph(HK_NULL)
{
	hkgVec3Copy(m_menuImageBackground, MENU_BACKGROUND_COLOR );
	hkgVec3Copy(m_menuImageBackgroundDirection, MENU_BACKGROUND_CHANGE );
	environment->m_window->setClearColor( m_menuImageBackground );
	environment->m_window->setWantDrawHavokLogo(false);

	m_newTimersGathered = false;

	int size = 2000000;
	
	m_savedStreamBegin = hkAllocate<char>( size, HK_MEMORY_CLASS_DEMO );
	m_savedStreamCurrent = m_savedStreamBegin;
	m_savedStreamEnd = m_savedStreamBegin + size;
	HK_ASSERT(0, hkMonitorStream::getInstance().isBufferAllocatedOnTheHeap() = true );
	m_simTimersEnd = hkMonitorStream::getInstance().getEnd();

	// bring up the logo
	{
		// lower end systems can't handle the big texture
		hkIstream s( (const char*)MenuImageLargeData, MenuImageLargeDataSize);
		m_menuImage = hkgTexture::create(environment->m_window->getContext());
		m_menuImage->loadFromPNG( s );
		m_menuImage->realize();
	}

	// 20 meg for timer info - this is needed for on screen stats and detailed stats files
	hkMonitorStream& stream = hkMonitorStream::getInstance();
	stream.resize( 20000000 );
	stream.reset();

	m_performanceCounterUtility = new hkPerformanceCounterUtility( );
	if (environment->m_options->m_enablePerformanceCounter)
	{
		m_performanceCounterUtility->enable((hkPerformanceCounterUtility::CounterSelect)environment->m_options->m_perfomanceCounterOption);
	}

#if defined(HK_UNIT_TESTS)
	registerUnitTests(m_extraNames, m_extraDemos);
#endif

	//	Rebuild the demo database
	hkDemoDatabase::getInstance().rebuildDatabase();


	if( m_defaultDemo != HK_NULL )
	{
		// Attempt to create default demo
		startCurrentDemo();
	}
	else
	{
		// Attempt to retrieve last demo path
		hkIstream is(LASTDEMO_FILENAME);
		if( is.isOk() )
		{
			char buffer[512];
			is.getline( buffer, 510 );
			m_currentPath = hkString( buffer );
		}
	}

	// set the "singleton" menu demo to me
	m_menuDemo = this;
}

MenuDemo::~MenuDemo()
{
//	writeSimpleMemoryStatistics();
	if (m_currentDemo)
	{
		stopCurrentDemo(); // so that stats can be written if required for it
	}

	delete m_tweaker;

	if (m_statGraph)
	{
		m_statGraph->removeReference();
		m_statGraph = HK_NULL;
	}

	for (int i=0; i< m_extraDemos.getSize(); i++)
	{
		delete m_extraDemos[i];
	}

	for (int n=0; n < m_extraNames.getSize(); n++)
	{
		delete m_extraNames[n];
	}

	if (m_menuImage)
	{
		m_env->m_window->getContext()->lock();
		m_menuImage->release();
		m_env->m_window->getContext()->unlock();
	}

	delete m_performanceCounterUtility;

	hkDeallocate<char>( m_savedStreamBegin );

	// clear the "singleton" menu demo if it is me
	if ( m_menuDemo == this )
	{
		m_menuDemo = HK_NULL;
	}
}

hkDemo::Result MenuDemo::stepVisualDebugger()
{
	if(!m_paused && m_currentDemo != HK_NULL) // run our current demo if present.
	{
		return m_currentDemo->stepVisualDebugger();
	}

	return hkDemo::DEMO_OK;
}

//
// stepDemo
//
hkDemo::Result MenuDemo::stepDemo()
{
	// run our current demo if present
	if ( m_currentDemo != HK_NULL )
	{
		return stepCurrentDemo();
	}

	// if we had a default demo then exit
	else if ( m_defaultDemo != HK_NULL )
	{
		return DEMO_STOP;
	}

	// otherwise just run the menu
	return stepMenuDemo();
}

void MenuDemo::preRenderDisplayWorld(hkgViewport* v)
{
	if(m_currentDemo != HK_NULL) // run our current game if present.
	{
		m_currentDemo->preRenderDisplayWorld(v);
	}
}

void MenuDemo::postRenderDisplayWorld(hkgViewport* v)
{
	if(m_currentDemo != HK_NULL) // run our current game if present.
	{
		m_currentDemo->postRenderDisplayWorld(v);
	}
	else if (m_menuImage)
	{
		hkDrawMenuImage(m_menuImage, m_env);
	}

	if (m_statGraph)
	{
		m_statGraph->display( m_env->m_options->m_statBarX, m_env->m_options->m_statBarY);

		if (m_mouseInMenuOverlay)
		{
			int mouseX = m_env->m_window->getMouse().getPosX();
			int mouseY = m_env->m_window->getMouse().getPosY();
			if ( (mouseX >= m_env->m_options->m_statBarX) && (mouseX < (m_env->m_options->m_statBarX + m_statGraph->getDisplayWidth())) &&
				(mouseY >= m_env->m_options->m_statBarY) && (mouseY < (m_env->m_options->m_statBarY + m_statGraph->getDisplayHeight())) )
			{
				m_statGraph->displayZoomExtents( m_mouseStatZoomOriginX, m_mouseStatZoomOriginY, mouseX, mouseY);
			}
		}
	}
}

void MenuDemo::windowResize(int w, int h)
{
	if (m_statGraph && (w > 0) && (h > 5) )
	{
		m_statGraph->init( m_statGraph->getType(), m_env->m_window->getContext(), w, h/m_env->m_options->m_statBarScreenFraction, 0x0 );
	}
}

hkDemo::Result MenuDemo::showPausedMenu( bool handleKeysOnly )
{
	if (!handleKeysOnly)
	{
		int dH = m_env->m_window->getTVDeadZoneH();
		int dV = m_env->m_window->getTVDeadZoneV();

		hkString oss;
		oss.printf("[ Paused ]\n\n%c Resume Demo\n%c Restart Demo\n\n%c Toggle Text Statistics\n%c Toggle Text Statistics\n%c Toggle Camera Info\n%c Settings\n%c Toggle Help\n%c Single Step\n\n%c Quit",
			// resume,         restart        stats          settings        camera info,      help            step           quit
			TEXT_BUTTON_START, TEXT_BUTTON_3, TEXT_BUTTON_1, TEXT_BUTTON_R2, TEXT_BUTTON_DOWN, TEXT_BUTTON_R1, TEXT_BUTTON_L1, TEXT_BUTTON_0, TEXT_BUTTON_2);

		m_env->m_textDisplay->outputText(oss, 20 + dH, 20 + dV, 0xffffffff);
	}

	hkgPad* gamePad = m_env->m_gamePad;
	const hkgMouse& mouse = m_env->m_window->getMouse();

	int px = mouse.getPosX();
	int py = mouse.getPosY();
	bool inStatGraphBounds = m_statGraph && (px > m_env->m_options->m_statBarX) && (px < (m_env->m_options->m_statBarX + m_statGraph->getDisplayWidth())) &&
		(py > m_env->m_options->m_statBarY) && (py < (m_env->m_options->m_statBarY + m_statGraph->getDisplayHeight()));


	if( gamePad->wasButtonPressed(HKG_PAD_BUTTON_L1) == true) // help
	{
		// Don't pass button press on to demo
		gamePad->setButtonState( gamePad->getButtonState() ^ HKG_PAD_BUTTON_L1 );
		m_helpTimeLeft = (m_helpTimeLeft <= 0.0f) ? m_helpTimeMax : 0.0f;
		m_paused = false;
	}
	else if( gamePad->wasButtonPressed(HKG_PAD_BUTTON_R1) == true) // settings
	{
		// Don't pass button press on to demo
		gamePad->setButtonState( gamePad->getButtonState() ^ HKG_PAD_BUTTON_R1 );
		m_wantTweak = true;
		m_paused = false;
	}
// 	else if( gamePad->wasButtonPressed(HKG_PAD_DPAD_DOWN) == true) // display camera position & target
// 	{
// 		// Don't pass button press on to demo
// 		gamePad->setButtonState( gamePad->getButtonState() ^ HKG_PAD_DPAD_DOWN );
// 		m_paused = false;
// 		if ( m_wantCameraData )
// 		{
// 			m_wantCameraData = false;
// 		}
// 		else
// 		{
// 			m_wantCameraData = true;
// 		}
// 	}
	else if( gamePad->wasButtonPressed(HKG_PAD_BUTTON_3) == true) // restart
	{
		// Don't pass button press on to demo
		gamePad->setButtonState( gamePad->getButtonState() ^ HKG_PAD_BUTTON_1 );
		stopCurrentDemo();
		startCurrentDemo();
		m_paused = false;
		return DEMO_OK;
	}
	else if( gamePad->wasButtonPressed(HKG_PAD_BUTTON_1) == true) // toggle stat text
	{
		m_wantMonitors = !m_wantMonitors;
		m_helpTimeLeft = 0.0f;
		m_paused = true;
		return DEMO_PAUSED;
	}
	else if( gamePad->wasButtonPressed(HKG_PAD_BUTTON_R2) == true) // toggle stat graph
	{
		if (!m_statGraph)
		{
			m_statGraph = new hkgStatGraph();
			m_statGraph->init( HKG_STAT_GRAPH_BAR, m_env->m_window->getContext(), m_env->m_window->getWidth(), m_env->m_window->getHeight()/m_env->m_options->m_statBarScreenFraction, 0x0);

			hkMonitorStreamAnalyzer::g_lastFrameTime = m_env->m_options->m_statBarStartTime * 1000.0f;

			m_env->m_window->setWantDrawHavokLogo(false);
		}
		else
		{
			m_statGraph->removeReference();
			m_statGraph = HK_NULL;

			m_env->m_window->setWantDrawHavokLogo(true);
		}
		m_helpTimeLeft = 0.0f;
		m_paused = true;
		return DEMO_PAUSED;
	}
	else if( gamePad->wasButtonPressed(HKG_PAD_BUTTON_2) == true) // quit
	{
		gamePad->setButtonState( gamePad->getButtonState() ^ HKG_PAD_BUTTON_2 );
		stopCurrentDemo();
		return DEMO_OK;
	}
	else if ( ( gamePad->wasButtonPressed(HKG_PAD_BUTTON_0) == true) || // single step
			(inStatGraphBounds && mouse.wasButtonPressed(HKG_MOUSE_LEFT_BUTTON)) )
	{
		if (inStatGraphBounds)
		{
			if (m_statGraph->userAction( px, py) == hkgStatGraph::HKG_STAT_GRAPH_ACTION_NONE)
			{
				m_mouseStatZoomOriginX = px;
				m_mouseStatZoomOriginY = py;
				m_mouseInMenuOverlay = true;

				// disable the hkg camera mode while we zoom
				if (mouse.wasButtonPressed(HKG_MOUSE_LEFT_BUTTON))
				{
					m_viewportMouseEventBackup = m_env->m_window->getCurrentViewport()->getAcceptsMouseEvents();
					m_env->m_window->getCurrentViewport()->setAcceptsMouseEvents(false);
				}
			}
			else
			{
				m_mouseInMenuOverlay = false; // icon pick
			}
			return DEMO_PAUSED;
		}

		// Don't pass button press on to demo
		// but fall out and do a single step if not doing a select using space etc in the stat bar
		gamePad->setButtonState( gamePad->getButtonState() ^ HKG_PAD_BUTTON_0 );
	}
	else if ((gamePad->wasButtonReleased(HKG_PAD_BUTTON_0)) ||
			(m_mouseInMenuOverlay && mouse.wasButtonReleased(HKG_MOUSE_LEFT_BUTTON)) )
	{
		if (m_mouseInMenuOverlay && mouse.wasButtonReleased(HKG_MOUSE_LEFT_BUTTON))
		{
			m_env->m_window->getCurrentViewport()->setAcceptsMouseEvents(m_viewportMouseEventBackup);
		}

		if (inStatGraphBounds && m_mouseInMenuOverlay)
		{
			m_statGraph->zoom( m_mouseStatZoomOriginX, m_mouseStatZoomOriginY, px, py);
		}
		m_mouseInMenuOverlay = false;

		return DEMO_PAUSED;
	}
	else
	{
		return DEMO_PAUSED;
	}
	return DEMO_ERROR;
}

hkDemo::Result MenuDemo::showTweakMenu()
{
	int dH = m_env->m_window->getTVDeadZoneH();
	int dV = m_env->m_window->getTVDeadZoneV();

	hkVariant* options = m_currentDemo->getOptions();
	if( options )
	{
		const hkgWindow* window = m_env->m_window;
		{
			hkString oss;
			oss.printf("[ Tweaking ]\n\n%c Tweak up\n%c Tweak down\n%c Restart with new parameters\n\n%c Stop tweaking\n%c %s",
				TEXT_BUTTON_3, TEXT_BUTTON_2, TEXT_BUTTON_1, TEXT_BUTTON_START, TEXT_BUTTON_0, m_paused ? "Resume" : "Pause" );
			hkReal lineHeight = m_env->m_textDisplay->getFont()->getCharHeight();
			m_env->m_textDisplay->outputText(oss, 20 + dH, int(window->getHeight()) - int(lineHeight*9) - dV, TEXT_COLOR_NORMAL);
		}
		HK_ASSERT(0x12cdf4de, options->m_object);
		HK_ASSERT(0x12cdf4df, options->m_class);
		if( m_tweaker && m_tweaker->getData() != options->m_object )
		{
			delete m_tweaker;
			m_tweaker = HK_NULL;
		}
		if( m_tweaker==HK_NULL )
		{
			m_tweaker = new Tweaker( options->m_object, *options->m_class );
			extern const hkClass MenuDemoEmptyClass;
			hkVersionUtil::copyDefaults( options->m_object, MenuDemoEmptyClass, *options->m_class );
			m_tweaker->m_activateButton = 0;
			m_tweaker->m_activateKey = 0;
			m_tweaker->m_clearButton = 0;
			m_tweaker->m_tweakUpButton = HKG_PAD_BUTTON_2; // ^
			m_tweaker->m_tweakDownButton = HKG_PAD_BUTTON_3; // O
			m_tweaker->m_tweakDownKey = '2';
			m_tweaker->m_tweakDownKey2 = 0;
			m_tweaker->m_tweakUpKey = '3';
			m_tweaker->m_clearButton = 0;
			m_tweaker->setMode( Tweaker::MODE_ACTIVE );
			m_tweaker->toNextSibling();
		}

		if( m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_0) ) // X
		{
			m_paused = !m_paused;
		}
		else if( m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_1) ) // []
		{
			stopCurrentDemo();
			startCurrentDemo();
			return DEMO_OK;
		}

		m_tweaker->update(m_env);
	}
	else
	{
		hkString oss;
		oss.printf("[ Tweaking ]\n\n%c Resume Demo\n%c Previous Menu\n\nNo tweakable parameters", TEXT_BUTTON_START, TEXT_BUTTON_0);
		m_env->m_textDisplay->outputText(oss, 20 + dH, 20 + dV, TEXT_COLOR_NORMAL);

		hkgPad* gamePad = m_env->m_gamePad;
		if( gamePad->wasButtonPressed(HKG_PAD_BUTTON_0) )
		{
			m_wantTweak = false;
		}
	}

	if(m_paused)
	{
		return DEMO_PAUSED;
	}
	return DEMO_ERROR;
}


void MenuDemo::showMonitorGraph( hkArray<hkTimerData>& threadStreamInfos, hkArray<hkTimerData>& spuStreamInfos )
{
	hkMonitorStreamFrameInfo frameInfo;
	hkArray<hkMonitorStreamAnalyzer::Node*> threadTrees;

	frameInfo.m_indexOfTimer0 = 0;
	frameInfo.m_indexOfTimer1 = -1;
	//the ms marker lines assume this (us, not ms):
	frameInfo.m_timerFactor0 = 1e6f / float(hkStopwatch::getTicksPerSecond());
	frameInfo.m_timerFactor1 = 1;
	frameInfo.m_absoluteTimeCounter = hkMonitorStreamFrameInfo::ABSOLUTE_TIME_TIMER_0;

	for (int destId = 0; destId < threadStreamInfos.getSize(); ++destId)
	{
		hkMonitorStreamAnalyzer::Node* currentTree = hkMonitorStreamAnalyzer::makeStatisticsTreeForSingleFrame(threadStreamInfos[destId].m_streamBegin, threadStreamInfos[destId].m_streamEnd, frameInfo, "/", false );

		hkMonitorStreamAnalyzer::Node* perThreadPerFrame = new hkMonitorStreamAnalyzer::Node(HK_NULL, "/", hkMonitorStreamAnalyzer::Node::NODE_TYPE_DIRECTORY);
		perThreadPerFrame->m_children.pushBack( currentTree ); // 1 thread->1 Frame->Stats
		threadTrees.pushBack( perThreadPerFrame );
	}
	for (int destId = 0; destId < spuStreamInfos.getSize(); ++destId)
	{
		hkMonitorStreamAnalyzer::Node* currentTree = hkMonitorStreamAnalyzer::makeStatisticsTreeForSingleFrame(spuStreamInfos[destId].m_streamBegin, spuStreamInfos[destId].m_streamEnd, frameInfo, "/", false);

		hkMonitorStreamAnalyzer::Node* perThreadPerFrame = new hkMonitorStreamAnalyzer::Node(HK_NULL, "/", hkMonitorStreamAnalyzer::Node::NODE_TYPE_DIRECTORY);
		perThreadPerFrame->m_children.pushBack( currentTree ); // 1 thread->1 Frame->Stats
		threadTrees.pushBack( perThreadPerFrame );
	}


	if ( threadTrees.getSize() > 0 )
	{
		int numThreads = threadStreamInfos.getSize();
		int numSpus = spuStreamInfos.getSize() ;

		// update may alter the node tree given to disable timers
		m_statGraph->update( threadTrees, frameInfo, numThreads, numSpus );

		if (!m_mouseInMenuOverlay)
		{
			int mouseX = m_env->m_window->getMouse().getPosX();
			int mouseY = m_env->m_window->getMouse().getPosY();

			hkMonitorStreamAnalyzer::Node* timerNodeUnderMouse = m_statGraph->findTimerNodeAtSample(threadTrees, frameInfo, numThreads, numSpus, mouseX, mouseY );
			if (timerNodeUnderMouse)
			{
				hkString nodeInfo;
				nodeInfo.printf("%s/%s:%.3fms", timerNodeUnderMouse->m_parent? timerNodeUnderMouse->m_parent->m_name : "", timerNodeUnderMouse->m_name, timerNodeUnderMouse->m_value[frameInfo.m_indexOfTimer0] * 0.001f );
				m_env->m_textDisplay->outputText( nodeInfo, mouseX, (int)( m_env->m_window->getHeight() - mouseY - 16));
			}
		}

		for (int tt=0; tt < threadTrees.getSize(); ++tt)
		{
			delete threadTrees[tt];
		}
	}
}


void MenuDemo::showMonitorAnalysis(int xOffset, hkArray<hkTimerData>& threadStreamInfos, hkArray<hkTimerData>& spuStreamInfos )
{
	hkMonitorStreamFrameInfo frameInfo;

#	ifdef HK_PS2
	frameInfo.m_indexOfTimer0 = 0;
	frameInfo.m_indexOfTimer1 = 1; // main timer
	frameInfo.m_timerFactor1 = 1e3f / 300e6f; //ms
	frameInfo.m_timerFactor0 = 1e3f / 300e6f; //ms
	frameInfo.m_absoluteTimeCounter = hkMonitorStreamFrameInfo::ABSOLUTE_TIME_TIMER_1;
#	else
	frameInfo.m_indexOfTimer0 = 0;
	frameInfo.m_indexOfTimer1 = -1;
	frameInfo.m_timerFactor0 = 1e3f / float(hkStopwatch::getTicksPerSecond());
	frameInfo.m_timerFactor1 = 1;
	frameInfo.m_absoluteTimeCounter = hkMonitorStreamFrameInfo::ABSOLUTE_TIME_TIMER_0;
#	endif


	if (m_statsTree == HK_NULL)
	{
		m_statsTree = hkMonitorStreamAnalyzer::makeStatisticsTreeForSingleFrame(threadStreamInfos[0].m_streamBegin, threadStreamInfos[0].m_streamEnd, frameInfo, "/", true );
		m_activeNode = HK_NULL;
	}
	else
	{
		//
		// Take the monitor streams from the last frame and blend them into the results for smooth display. If paused, just show the actual
		// timings from last frame.
		//

		hkReal blendFromLastFrame = m_paused ? 0 : .8f;

			for (int destId = 0; destId < threadStreamInfos.getSize(); ++destId)
			{
				hkMonitorStreamAnalyzer::Node* currentTree = hkMonitorStreamAnalyzer::makeStatisticsTreeForSingleFrame(threadStreamInfos[destId].m_streamBegin, threadStreamInfos[destId].m_streamEnd, frameInfo, "/", true );
				hkMonitorStreamAnalyzer::mergeTreesForCombinedThreadSummary(m_statsTree, currentTree, destId, 0, blendFromLastFrame );
				delete currentTree;
			}
			for (int destId = 0; destId < spuStreamInfos.getSize(); ++destId)
			{
				hkMonitorStreamAnalyzer::Node* currentTree = hkMonitorStreamAnalyzer::makeStatisticsTreeForSingleFrame(spuStreamInfos[destId].m_streamBegin, spuStreamInfos[destId].m_streamEnd, frameInfo, "/", true );
				hkMonitorStreamAnalyzer::mergeTreesForCombinedThreadSummary(m_statsTree, currentTree, destId + threadStreamInfos.getSize(), 0, blendFromLastFrame );
				delete currentTree;
			}
	}

	{
		hkgPad& pad = *m_env->m_gamePad;

		// do we have enough stats to init the active node.
		// As the sims can start off with no active node (no children in stats, esp multithread demos)
		if (!m_activeNode && m_statsTree && (m_statsTree->m_children.getSize() > 0))
		{
			m_statsTree->m_userFlags |= 1;
			m_activeNode = m_statsTree->m_children[0];
		}

		// Navigation
		if (m_paused || pad.getButtonState() & HKG_PAD_BUTTON_0)
		{
			hkMonitorStreamAnalyzer::CursorKeys keys;
			keys.m_upPressed = pad.wasButtonPressed(HKG_PAD_DPAD_UP);
			keys.m_downPressed = pad.wasButtonPressed(HKG_PAD_DPAD_DOWN);
			keys.m_leftPressed = pad.wasButtonPressed(HKG_PAD_DPAD_LEFT);
			keys.m_rightPressed = pad.wasButtonPressed(HKG_PAD_DPAD_RIGHT);
			m_activeNode = hkMonitorStreamAnalyzer::navigateMonitors( keys, m_activeNode );
		}

		hkArray<char> osBuf;
		hkOstream os(osBuf);

		os  << "Hold " << TEXT_BUTTON_0 << " and use "
			<< TEXT_BUTTON_UP << TEXT_BUTTON_DOWN << TEXT_BUTTON_LEFT << TEXT_BUTTON_RIGHT
			<< " to navigate\n\n";

		{
			hkMonitorStreamAnalyzer::CombinedThreadSummaryOptions options;
			options.m_activeNode = m_activeNode;
			options.m_displayPartialTree = true;
			options.m_downArrowChar = TEXT_BUTTON_DOWN;
			options.m_rightArrowChar = TEXT_BUTTON_RIGHT;
			options.m_indentationToFirstTimerValue = -1;
			options.m_timerColumnWidth = 13;
			options.m_tabSpacingForTimerNames = 1;
			options.m_tabSpacingForTimerValues = 1;

			hkMonitorStreamAnalyzer::showCombinedThreadSummaryForSingleFrame(m_statsTree, threadStreamInfos.getSize(), spuStreamInfos.getSize(), os, options );
		}

		int dH = m_env->m_window->getTVDeadZoneH();
		int dV = m_env->m_window->getTVDeadZoneV();

		m_env->m_textDisplay->outputText( osBuf.begin(), 10 + dH, 10 + dV);
	}
}

hkDemo::Result MenuDemo::stepCurrentDemo()
{
	hkgPad* gamePad = m_env->m_gamePad;

	m_currentDemo->waitForStepCompletion();

	switch (m_env->m_options->m_replayType)
	{
		case hkDemoFrameworkOptions::REPLAY_RECORD:
			m_replay.recordFrame( m_env );
			break;

		case hkDemoFrameworkOptions::REPLAY_PLAYBACK:
			m_replay.playbackFrame( m_env );
			break;
	}


	if( gamePad->wasButtonPressed(HKG_PAD_START) == true
		|| m_env->m_window->getKeyboard().wasKeyPressed(HKG_VKEY_F1) ) // pause pressed?
	{
		if( m_wantTweak )
		{
			m_wantTweak = false;
			m_paused = false;
		}
		else
		{
			m_paused = !m_paused;
		}
	}

	if ( m_paused && m_newTimersGathered )
	{
		// Switch current monitor stream for this thread to continue to capture graphics timers
		hkAlgorithm::swap<hkPadSpu<char*> >(m_savedStreamBegin, hkMonitorStream::getInstance().m_start);
		hkAlgorithm::swap(m_savedStreamCurrent, hkMonitorStream::getInstance().m_end);
		hkAlgorithm::swap(m_savedStreamEnd, hkMonitorStream::getInstance().m_capacity);
		hkMonitorStream::getInstance().m_capacityMinus16 = hkMonitorStream::getInstance().m_capacity - 32;
		hkMonitorStream::getInstance().reset();
		m_pausedSimTimers = m_simTimersEnd;
		m_newTimersGathered = false;
	}

	if( m_env->m_window->getKeyboard().wasKeyPressed(HKG_VKEY_TAB) )
	{
		m_wantTweak = !m_wantTweak;
	}

	if( m_wantTweak ) // show paused context menu
	{
		hkDemo::Result res = showTweakMenu();
		if (res != DEMO_ERROR )
		{
			return res;
		}
	}
	else if( m_paused ) // show paused context menu
	{
		if( (m_wantMonitors || m_statGraph) && m_currentDemo )
		{
			hkArray<hkTimerData> threadStreams;
			hkArray<hkTimerData> spuStreams;

			m_currentDemo->getTimerStreamInfo( threadStreams, spuStreams, 6 );

			// Append saved simulation timers to this threads timers (now have graphics in them)
			hkUlong simTimerSize = m_pausedSimTimers - m_savedStreamBegin;
			hkUlong availableSpace = hkMonitorStream::getInstance().m_capacity - hkMonitorStream::getInstance().m_end;
			if ( availableSpace > simTimerSize )
			{
				hkString::memCpy( hkMonitorStream::getInstance().getEnd(), m_savedStreamBegin, (int)simTimerSize );
				threadStreams[0].m_streamEnd = hkMonitorStream::getInstance().m_end + simTimerSize;
			}

			if (m_wantMonitors)
				showMonitorAnalysis(0, threadStreams, spuStreams );

			if (m_statGraph)
			{
				// For this thread, just use the saved physics timers, otherwise the time range will keep getting bigger
				hkArray<hkTimerData> threadStreamsNoGraphics;
				threadStreamsNoGraphics = threadStreams;
				threadStreamsNoGraphics[0].m_streamBegin = m_savedStreamBegin;
				threadStreamsNoGraphics[0].m_streamEnd = m_pausedSimTimers;
				showMonitorGraph(threadStreamsNoGraphics, spuStreams);
			}

			// Reset just this thread (used for graphics)
	
			hkMonitorStream::getInstance().reset();

		}
		hkDemo::Result res = showPausedMenu( m_wantMonitors );

		if (res != DEMO_ERROR )
		{
			return res;
		}
	}

	if(m_currentDemo->getError().getLength() != 0)
	{
		hkString error = (m_currentDemo->getError().getLength() > 0)
			? m_currentDemo->getError()
			: "Unspecified Error";

		m_env->m_textDisplay->outputText(error, 20, 20, TEXT_COLOR_CURRENT);
		return DEMO_OK;
	}
	else if(0)
	{
		hkString oss;
		oss.printf("Menu : %c", TEXT_BUTTON_START);
		m_env->m_textDisplay->outputText(oss, 20, 20, 0x00777777);
	}

	//
	// Display camera position & target
	//
	if ( m_wantCameraData )
	{
		int windowHeight = m_env->m_window->getHeight();
		int dH           = m_env->m_window->getTVDeadZoneH();
		int dV           = m_env->m_window->getTVDeadZoneV();

		float* cameraFrom = m_env->m_window->getViewport(0)->getCamera()->getFromPtr();
		float* cameraTo   = m_env->m_window->getViewport(0)->getCamera()->getToPtr();
		hkString oss;
		oss.printf("Camera position : %.2f %.2f %.2f\nCamera target   : %.2f %.2f %.2f", cameraFrom[0], cameraFrom[1], cameraFrom[2], cameraTo[0], cameraTo[1], cameraTo[2]);
		m_env->m_textDisplay->outputText(oss, 20 + dH, windowHeight - dV - 50, 0xffffffff);
	}

	if(m_helpTimeLeft > 0.0f && !m_wantTweak )
	{
		m_helpTimeLeft -= m_timestep;
		int overallindex = hkDemoDatabase::getInstance().findDemo( m_currentPath.cString());
		if( overallindex >= 0 )
		{
			const hkDemoEntry& demo = hkDemoDatabase::getInstance().getDemos()[overallindex];
			const int winWidth = m_env->m_window->getWidth();
			const char* help = demo.m_details;
			if ( !help ){ help = "Unknown Help"; }
			hkString help2;
			help2.setCapacity( hkString::strLen(help) + 256 );
			if( m_currentDemo->getOptions() )
			{
				help2.printf("%s\nPress <Tab> or %c %c to tweak this demo", help, TEXT_BUTTON_START, TEXT_BUTTON_R1);
				help = help2.cString();
			}
			int dH = m_env->m_window->getTVDeadZoneH();
			int dV = m_env->m_window->getTVDeadZoneV();

			m_env->m_textDisplay->outputTextWithWrapping(help, 20 + dH, 20 + dV, winWidth-20-dH, TEXT_COLOR_NORMAL);
		}
	}

	if (!m_childDemoJustCreated)
	{
		m_env->m_displayHandler->clear(); // clear debug display before a potential world step
	}
	m_childDemoJustCreated = false;


	// Only gather stats if you have to (so if showing graphs, gathering stats, or if VDB is enabled)
	//bool vdbEnbabledAndRunning = (stepResult != hkDemo::DEMO_PAUSED) && m_currentDemo->visualDebuggerEnabled();
	if ( m_currentDemo->visualDebuggerEnabled()  || m_wantMonitors || m_statGraph || m_performanceCounterUtility->isEnabled() )
	{
		hkArray<hkTimerData> threadStreams;
		hkArray<hkTimerData> spuStreams;

		m_currentDemo->getTimerStreamInfo( threadStreams, spuStreams, 6 );

		if( m_currentDemo->visualDebuggerEnabled() )
		{

			m_currentDemo->addTimersToVdb( threadStreams, spuStreams );

			stepVisualDebugger();

		}

		if (!m_paused)
		{

			// On screen monitors
			if( m_wantMonitors )
			{
				showMonitorAnalysis(0, threadStreams, spuStreams );
			}

			if (m_statGraph)
			{
				showMonitorGraph(threadStreams, spuStreams);
			}
		}
		for ( int i = 0; i < threadStreams.getSize(); ++i )
		{
			m_performanceCounterUtility->captureExternalFrame( threadStreams[i].m_streamBegin, threadStreams[i].m_streamEnd, i );
		}
		for ( int i = 0; i < spuStreams.getSize(); ++i )
		{
			m_performanceCounterUtility->captureExternalFrame( spuStreams[i].m_streamBegin, spuStreams[i].m_streamEnd, i + threadStreams.getSize() );
		}
	}

	if (m_env->m_mousePickingEnabled)
	{

		// assumes graph is shown from 0,0
		const hkgMouse& mouse = m_env->m_window->getMouse();
		int px = mouse.getPosX();
		int py = mouse.getPosY();
		bool inStatBarRegion = m_statGraph && (px > m_env->m_options->m_statBarX) && (px < (m_env->m_options->m_statBarX + m_statGraph->getDisplayWidth())  ) &&
			                                  (py > m_env->m_options->m_statBarY) && (py < (m_env->m_options->m_statBarY + m_statGraph->getDisplayHeight()) );

		if( !m_mouseActive &&
			(   m_env->m_window->getKeyboard().wasKeyPressed(HKG_VKEY_SPACE) ||
				( inStatBarRegion && mouse.wasButtonPressed(HKG_MOUSE_LEFT_BUTTON)) ||
			    gamePad->wasButtonPressed(HKG_PAD_BUTTON_0) // == Space in pseudo pad on PC, so on PC button 0 is used up anyway..
			  ) )
		{
			// do any other 2D Menu picking here
			if (inStatBarRegion)
			{
				// assumes graph is shown from 0,0
				if (m_statGraph->userAction( px, py) == hkgStatGraph::HKG_STAT_GRAPH_ACTION_NONE)
				{
					m_mouseStatZoomOriginX = px;
					m_mouseStatZoomOriginY = py;
					m_mouseInMenuOverlay = true;

					// disable the hkg camera mode while we zoom
					if (mouse.wasButtonPressed(HKG_MOUSE_LEFT_BUTTON))
					{
						m_viewportMouseEventBackup = m_env->m_window->getCurrentViewport()->getAcceptsMouseEvents();
						m_env->m_window->getCurrentViewport()->setAcceptsMouseEvents(false);
					}
				}
				else
				{
					m_mouseInMenuOverlay = false; // actually should be called 'in stat zoom mode'
				}
			}
			else
			{
				m_currentDemo->mouseDown(); // pick object under cursor
				m_mouseActive = true;
			}

			m_mousePadTrackedButton = gamePad->wasButtonPressed(HKG_PAD_BUTTON_0)? HKG_PAD_BUTTON_0: -1;
		}
		else if (m_mouseInMenuOverlay || m_mouseActive)
		{
			if( m_env->m_window->getKeyboard().wasKeyReleased(HKG_VKEY_SPACE) ||
				(m_mouseInMenuOverlay && mouse.wasButtonReleased(HKG_MOUSE_LEFT_BUTTON)) ||
				( (m_mousePadTrackedButton != -1) && (gamePad->wasButtonReleased((HKG_PAD_BUTTON)m_mousePadTrackedButton)) ) )
			{
				if (m_mouseInMenuOverlay)
				{
					if (mouse.wasButtonReleased(HKG_MOUSE_LEFT_BUTTON))
					{
						m_env->m_window->getCurrentViewport()->setAcceptsMouseEvents(m_viewportMouseEventBackup);
					}

					m_mouseInMenuOverlay = false;
					if (m_statGraph && inStatBarRegion)
					{
						m_statGraph->zoom( m_mouseStatZoomOriginX, m_mouseStatZoomOriginY, px, py);
					}
				}
				else // m_mouseActive
				{
					m_currentDemo->mouseUp();
				}

				m_mouseActive = false;
				m_mousePadTrackedButton = -1;
			}
			else if (m_mouseActive)
			{
				m_currentDemo->mouseDrag();
			}
		}
	}

	// Reset timer streams prior to stepping
	m_currentDemo->resetTimerStreams();

	// Start the counter for the next step
	m_performanceCounterUtility->startCounter();

	m_newTimersGathered = true;
	Result stepResult = m_currentDemo->stepDemo();

	m_simTimersEnd = hkMonitorStream::getInstance().getEnd();

	//
	// Capture the frame
	//
	m_performanceCounterUtility->stopCounter();

	switch( stepResult )
	{
		case DEMO_OK:
		{
			break;
		}

		case DEMO_PAUSED:
		{
			m_paused = true;
			return DEMO_PAUSED;
		}
		case DEMO_STOP:
		{
			stopCurrentDemo();
			// if we were running a single demo, given by -g demo
			// then we should stop now
			if( m_env->m_options->m_defaultDemo )
			{
				return DEMO_STOP;
			}
			break;
		}
		case DEMO_RESTART:
		{
			stopCurrentDemo();
			startCurrentDemo();
			break;
		}
		case DEMO_ERROR:
		{
			break;
		}
		default:
		{
			HK_ASSERT2(0x194d9b84, 0, "Should be unreachable");
		}
	}

	return DEMO_OK;
}

#include <Common/Base/DebugUtil/StatisticsCollector/MatchSnapshot/hkMatchSnapshotStatisticsCollector.h>

void MenuDemo::writeSimpleMemoryStatistics()
{
	hkReferencedObject::lockAll();

    hkDebugMemorySnapshot snapshot;
	if ( hkMemory::getInstance().isDebugMemory() )
	{
		hkDebugMemory* debugMem = (hkDebugMemory*)&hkMemory::getInstance();
        debugMem->getSnapshot( 0, true, snapshot );
	}

	hkOstream ostr("captureMemoryDump.txt");


	{
		hkSimpleStatisticsCollector collector( hkBuiltinTypeRegistry::getInstance().getVtableClassRegistry());

        collector.beginSnapshot( 0 );
        collector.addReferencedObject("All", m_currentDemo );
		collector.commitSnapshot(  );

		ostr.printf("\n\nDemoName: %s**\n", m_env->m_menuPath.cString() );
		ostr.printf("============================\n\n");

		collector.writeStatistics( ostr, hkMonitorStreamAnalyzer::REPORT_PERFRAME_TIME | hkMonitorStreamAnalyzer::REPORT_PERFRAME_SUMMARY );

		hkMemory::getInstance().printStatistics( &ostr );
	}

		// match memory managers with statistics
	if ( hkMemory::getInstance().isDebugMemory() )
	{

        //
        hkMatchSnapshotStatisticsCollector collector(hkBuiltinTypeRegistry::getInstance().getVtableClassRegistry(), snapshot, ostr );
        collector.addReached(m_memorySnapshot);

        /// Missing:
        // hkStackMemory
        // hkMonitorStream

        //collector.addChildObject("MonitorStream", &hkMonitorStream::getInstance());
		collector.addReferencedObject("All", m_currentDemo );

		// Monitor stream
		//collector.addChildObject("hkMonitorStream", &hkMonitorStream::getInstance());

		// add menu registry
		{
			for (int i =0 ; i < m_extraNames.getSize(); i++)
			{
				hkString* name = m_extraNames[i];
				collector.addNormalChunk( "MenuDemo::ExtraNames", name, sizeof(*name), 0);
				collector.addNormalChunk( "MenuDemo::ExtraNames", name->cString(), name->getCapacity(), 0);
			}
		}
		{
			for (int i =0 ; i < m_extraDemos.getSize(); i++)
			{
				hkDemoEntryRegister* demo = m_extraDemos[i];
				collector.addNormalChunk( "MenuDemo::ExtraDemos", demo, sizeof(*demo), 0);
			}
		}
		{
			hkObjectArray<hkDemoEntry>& demos = hkDemoDatabase::getInstance().m_demos;
			for (int i =0 ; i < demos.getSize(); i++)
			{
				const hkDemoEntry* de = &demos[i];

				collector.addNormalChunk( "MenuDemo::Demos", de->m_menuPath.cString(), de->m_menuPath.getCapacity(), 0);
				collector.addNormalChunk( "MenuDemo::Demos", de->m_demoPath.cString(), de->m_demoPath.getCapacity(), 0);
			}
		}
		{
			const hkObjectArray<hkProcessFactory::ProcessIdPair>& funcs = hkProcessFactory::getInstance().getName2CreationFunction();
			for (int i =0 ; i < funcs.getSize(); i++)
			{
				const hkProcessFactory::ProcessIdPair* de = &funcs[i];

				collector.addNormalChunk( "hkProcessFactory::Funcs", de->m_name.cString(), de->m_name.getCapacity(), 0);
			}
		}
		collector.addNormalChunk("hkDemoEnvironment::m_demoPath", m_env->m_demoPath.cString(), m_env->m_demoPath.getCapacity(), 0 );

		collector.addCallStackIgnoreString( hkString("hkDefaultDemo::hkDefaultDemo"));
		collector.addCallStackIgnoreString( hkString("hkDefaultDemo::setupGraphics"));
		collector.addCallStackIgnoreString( hkString("hkDefaultDemo::setupLights"));
		collector.addCallStackIgnoreString( hkString("hkDemo::setDemoName"));

		collector.addCallStackIgnoreString( hkString("hkDebugDisplayProcess::hkDebugDisplayProcess"));

		collector.addCallStackIgnoreString( hkString("hkpShapeDisplayViewer::"));
		collector.addCallStackIgnoreString( hkString("hkgDisplayHandler::"));
		collector.addCallStackIgnoreString( hkString("hkgFaceSetDX9::"));
		collector.addCallStackIgnoreString( hkString("hkgTexture::"));
		collector.addCallStackIgnoreString( hkString("hkgGeometryConverter::"));
		collector.addCallStackIgnoreString( hkString("hkpPhysicsContext::"));
		collector.addCallStackIgnoreString( hkString("hkpPhantomDisplayViewer::"));

		collector.addCallStackIgnoreString( hkString("hkgGeometry::"));
		collector.addCallStackIgnoreString( hkString("hkProcessFactory::"));
		collector.addCallStackIgnoreString( hkString("hkTextDisplay::"));

		collector.addCallStackIgnoreString( hkString("hkgDisplayContext"));
		collector.addCallStackIgnoreString( hkString("hkgInstancedDisplayObject"));

		collector.dumpRemaining();
	}

	hkReferencedObject::unlockAll();
}

void MenuDemo::stopCurrentDemo()
{
	switch (m_env->m_options->m_replayType)
	{
		case hkDemoFrameworkOptions::REPLAY_RECORD:
			m_replay.endRecordingAndSave(m_env->m_options->m_inputFilename);
			break;

		case hkDemoFrameworkOptions::REPLAY_PLAYBACK:
			m_replay.endPlayback(m_env);
			break;
	}
	
	m_performanceCounterUtility->saveFileAndReset();

	// memory dump
	if ( hkMemory::getInstance().isDebugMemory() )
	{
		hkReferencedObject::setLockMode( hkReferencedObject::LOCK_MODE_NONE );
		writeSimpleMemoryStatistics();
		hkReferencedObject::setLockMode( hkReferencedObject::LOCK_MODE_NONE );
	}

	m_env->m_displayHandler->clear();

	hkReferencedObject::setLockMode( hkReferencedObject::LOCK_MODE_AUTO );
	m_currentDemo->preDeleteDemo();
	delete m_currentDemo;
	m_currentDemo = HK_NULL;
	hkReferencedObject::setLockMode( hkReferencedObject::LOCK_MODE_NONE );

	// reset all monitors
	{
		m_activeNode = HK_NULL;
		if (m_statsTree)
		{
			delete m_statsTree;
		}
		m_statsTree = HK_NULL;

		if (m_statGraph)
		{
			delete m_statGraph;
		}
		m_statGraph = HK_NULL;

	}

	hkgVec3Copy(m_menuImageBackground, MENU_BACKGROUND_COLOR );
	m_env->m_window->setClearColor( m_menuImageBackground );
	m_env->m_window->setWantDrawHavokLogo(false);
}

void MenuDemo::startCurrentDemo()
{
	switch (m_env->m_options->m_replayType)
	{
		case hkDemoFrameworkOptions::REPLAY_RECORD:
			m_replay.startRecording();
			break;

		case hkDemoFrameworkOptions::REPLAY_PLAYBACK:
			m_replay.startPlayback(m_env->m_options->m_inputFilename, m_env);
			break;
	}

	m_env->m_window->getContext()->lock();

	m_childDemoJustCreated = true;
	// Find the index of the default or current demo
	const char* demoName = ( m_defaultDemo != HK_NULL )? m_defaultDemo: m_currentPath.cString();
	int index = hkDemoDatabase::getInstance().findDemo( demoName );

	m_env->m_window->setClearColor( DEMO_BACKGROUND_COLOR );
	m_env->m_window->setWantDrawHavokLogo(true);

	/// take a snapshot if we have debug memory
	if ( hkMemory::getInstance().isDebugMemory() )
	{
		hkDebugMemory* debugMem = (hkDebugMemory*)&hkMemory::getInstance();
		debugMem->getSnapshot( 0, true, m_memorySnapshot );
	}

	if ( index >= 0)
	{
		hkReferencedObject::lockAll();

		// Create the demo
		const hkDemoEntry& demo = hkDemoDatabase::getInstance().getDemos()[index];
		m_env->m_demoPath = demo.m_demoPath;
		m_env->m_menuPath = demo.m_menuPath;
		m_env->m_variantId  = demo.m_variantId;

		m_currentPath = demo.m_menuPath;
		m_currentDemo = (*demo.m_func)(m_env);
		m_currentDemo->postConstruct();
		m_currentDemo->setDemoName(m_currentPath);
		m_helpTimeLeft = m_helpTimeMax;

		m_performanceCounterUtility->setDemoName( m_env->m_menuPath.cString() );

		hkReferencedObject::unlockAll();

		hkArray<hkTimerData> threadStreams;
		hkArray<hkTimerData> spuStreams;

		m_currentDemo->getTimerStreamInfo( threadStreams, spuStreams, 6 );

		m_performanceCounterUtility->resetNumThreads( threadStreams.getSize(), spuStreams.getSize() );

		m_paused = false;
	}
	else
	{
		// Otherwise show the error demo
		m_currentDemo = new ErrorDemo(m_env, demoName);
	}

	m_env->m_window->getContext()->unlock();

}

hkDemo::Result MenuDemo::stepMenuDemo()
{
	const hkObjectArray<hkDemoEntry>& alldemos = hkDemoDatabase::getInstance().getDemos();
	int startx = 20 + m_env->m_window->getTVDeadZoneH();
	int starty = m_env->m_window->getTVDeadZoneV();
	int stepx = 13;

	int stepy = 20;
	hkTextDisplay* textDisplay = m_env->m_textDisplay;
	if( m_env->m_window->getKeyboard().wasKeyPressed(HKG_VKEY_OEM_2_FORWARDSLASH) )
	{
		m_searchString += "/";
		m_searchIndex = 0;
	}
	if( m_searchString.getLength() )
	{
		// keyboard accelerators
		const hkgKeyboard& kb = m_env->m_window->getKeyboard();
        const int ranges[2][2] = { {'A','Z'}, {'0','9'} };
        for( int rangeIndex = 0; rangeIndex < int(HK_COUNT_OF(ranges)); ++rangeIndex )
        {
            for( int i = ranges[rangeIndex][0]; i <= ranges[rangeIndex][1]; ++i )
            {
                if( kb.wasKeyPressed( HKG_KEYBOARD_VKEY(i) ) )
                {
                    char s[2]; s[0] = char(i); s[1] = 0;
                    if( ! kb.getKeyState(HKG_VKEY_SHIFT) )
                    {
                        s[0] = hkString::toLower(s[0]);
                    }
                    m_searchString += s;
                }
            }
        }

		starty += stepy;
		hkString displayString; displayString.printf("Searching for: %s", m_searchString.cString()+1);
		textDisplay->outputText( displayString, startx, starty);
		starty += stepy;

		hkArray<const hkDemoEntry*> entries;
		entries.reserve( (m_env->m_window->getHeight() - 2*starty) / stepy );
		for (int i = 0; i < alldemos.getSize() && entries.getSize() < entries.getCapacity(); i++)
		{
			hkString hayStack = canonicalise(alldemos[i].m_menuPath);
			hkString needle = canonicalise(m_searchString);
			if( hkString::strStr( hayStack.cString(), needle.cString()+1 ) != 0 ) // skip leading '/'
			{
				entries.pushBack( alldemos.begin() + i );
			}
		}
		m_searchIndex = hkMath::min2( entries.getSize(), m_searchIndex );
		if( entries.getSize() == 0 )
		{
			starty += stepy;
			textDisplay->outputText( "No demos found", startx, starty, TEXT_COLOR_DIRECTORY);
		}
		for (int i = 0; i < entries.getSize(); i++)
		{
			starty += stepy;
			hkUint32 color = (i == m_searchIndex) ? TEXT_COLOR_CURRENT : TEXT_COLOR_NORMAL;
			textDisplay->outputText( entries[i]->m_menuPath, startx, starty, color);
		}
		if( entries.getSize() == entries.getCapacity() )
		{
			starty += stepy;
			textDisplay->outputText( "...", startx, starty);
		}

		int numEntries = entries.getSize();
		hkgPad* gamePad = m_env->m_gamePad;

		if (gamePad->wasButtonPressed(HKG_PAD_DPAD_UP))
		{
			m_searchIndex = (m_searchIndex + numEntries - 1) % numEntries;
		}
		else if (gamePad->wasButtonPressed(HKG_PAD_DPAD_DOWN))
		{
			m_searchIndex = (m_searchIndex + 1) % numEntries;
		}
		else if (gamePad->wasButtonPressed(HKG_PAD_BUTTON_0)  || gamePad->wasButtonPressed(HKG_PAD_START))
		{
			if( m_searchIndex < entries.getSize() )
			{
				m_currentPath = entries[m_searchIndex]->m_menuPath;
				hkSetLastDemo(m_currentPath.cString());
				if (! hkString::strStr( m_currentPath.cString(), "ootstrap") )
					hkSetLastDemoBootstrap(m_currentPath.cString());
				startCurrentDemo();
			}
		}
		else if (gamePad->wasButtonPressed(HKG_PAD_BUTTON_1)) //cancel
		{
			m_searchString = "";
		}
		if( kb.wasKeyPressed( HKG_VKEY_BACK ) || gamePad->wasButtonPressed(HKG_PAD_DPAD_LEFT) )
		{
			m_searchString = m_searchString.substr(0, m_searchString.getLength()-1);
		}

		return DEMO_OK;
	}

	// get the entries for this level
	hkObjectArray<Entry> entries;
	int selectedIndex = 0;
	while (1)
	{
		hkObjectArray<hkString> alreadyHadIt;

		// initialize the path
		hkString path;
		{
			int li = m_currentPath.lastIndexOf('/');
			if ( li >= 0)
			{
				path = m_currentPath.substr(0, li+1);
			}
			else
			{
				path = "";
			}
		}
		int skip = path.getLength();


		for (int i = 0; i < alldemos.getSize(); i++)
		{
			hkString n = alldemos[i].m_menuPath;
			if( n.beginsWith(path) && !n.endsWith("Menu") )
			{
				int slash = n.indexOf('/', skip);
				int len;
				const char* help = HK_NULL;

				if(slash >= 0)
				{
					len = slash - skip;
				}
				else
				{
					len = n.getLength() - skip;
					help = alldemos[i].m_help;
				}
				hkString name = n.substr(skip, len);

				if( alreadyHadIt.indexOf(name) == -1)
				{
					Entry e(name, path+name, help, i);
					entries.pushBack(e);
					alreadyHadIt.pushBack(name);
				}
			}
		}

		for(int k=0; k<entries.getSize(); ++k)
		{
			if ( m_currentPath == entries[k].m_fullPath)
			{
				selectedIndex = k;
				break;
			}
		}
		if ( entries.getSize() )
		{
			break;
		}
		m_currentPath = "Utilities";
	}

	hkgPad* gamePad = m_env->m_gamePad;

	// Show Build Number
	{
		const int h = m_env->m_window->getHeight() - m_env->m_window->getTVDeadZoneV();
		const int dx =  m_env->m_window->getTVDeadZoneH();

		hkString buf;
		buf.printf("Havok v%d.%d.%d - Build(%d)",
			HAVOK_SDK_VERSION_MAJOR,	HAVOK_SDK_VERSION_MINOR,	HAVOK_SDK_VERSION_POINT,	HAVOK_BUILD_NUMBER);

		if (hkgSystem::g_RendererType != hkgSystem::HKG_RENDERER_CONSOLE)
		{
			buf += hkString(" - Renderer: ") + hkString(hkgSystem::getRendererString());
		}

		textDisplay->outputText( buf.cString(), 20+dx, h-40, TEXT_COLOR_NORMAL, 1);

	}

	// show status and location
	{
		starty += stepy;
		textDisplay->outputText(getStatus(), startx, starty);
		starty += stepy;

		hkString loc = m_currentPath;
		int slash;
		while( (slash = loc.indexOf('/')) != -1)
		{
			hkString label = "";
			label += loc.substr(0, slash+1);
			label += "";
			loc = loc.substr(slash+1);
			starty += stepy;
			textDisplay->outputText(insertSpaces(label), startx, starty, TEXT_COLOR_NORMAL);
			startx += stepx;
		}
	}

	// show current entries
	{
		for(int i = 0; i < entries.getSize(); ++i)
		{
			starty += stepy;
			hkString name = insertSpaces(entries[i].m_name);

			if ( entries[i].isDir() )
			{
				hkUint32 color = (i == selectedIndex) ? TEXT_COLOR_CURRENT : TEXT_COLOR_DIRECTORY;
				hkString str = "[";
				for(int j=0; j<=name.getLength(); j++)
				{
					str += " ";
				}
				str += "]";
				textDisplay->outputText(str, startx-4, starty, color);
			}

			hkUint32 color = (i == selectedIndex) ? TEXT_COLOR_CURRENT : TEXT_COLOR_NORMAL;
			hkString str = " ";
			str += name;
			str += " ";
			textDisplay->outputText(str, startx, starty, color);
		}
	}


	// process user events
	{
		const int numEntries = entries.getSize();

		// next item
		if (gamePad->wasButtonPressed(HKG_PAD_DPAD_UP))
		{
			selectedIndex = (selectedIndex + numEntries - 1) % numEntries;
		}
		// prev item
		else if (gamePad->wasButtonPressed(HKG_PAD_DPAD_DOWN))
		{
			selectedIndex = (selectedIndex + 1) % numEntries;
		}
		// up a level
		else if(	gamePad->wasButtonPressed(HKG_PAD_DPAD_LEFT)
				||	gamePad->wasButtonPressed(HKG_PAD_BUTTON_1)
			  	||	m_env->m_window->getKeyboard().wasKeyPressed(HKG_VKEY_BACK) )
		{
			selectedIndex = -1;
			addStatus("");
		}
		// down a level
		else if( gamePad->wasButtonPressed(HKG_PAD_DPAD_RIGHT) )
		{
			if( entries[selectedIndex].isDir() )
			{
				m_currentPath += "/.";
				selectedIndex = 0;
				addStatus("");
			}
		}
		// start a demo, or go down a level
		else if (gamePad->wasButtonPressed(HKG_PAD_BUTTON_0)  || gamePad->wasButtonPressed(HKG_PAD_START))
		{
			if( entries[selectedIndex].isDir() == false)
			{
				hkSetLastDemo(m_currentPath.cString());
				if (! hkString::strStr( m_currentPath.cString(), "ootstrap") )
					hkSetLastDemoBootstrap(m_currentPath.cString());
				startCurrentDemo();
			}
			else // directory
			{
				m_currentPath += "/.";
				selectedIndex = 0;
				addStatus("");
			}
		}
		// cancel the game
		else if (gamePad->wasButtonPressed(HKG_PAD_BUTTON_1)) //cancel
		{
			// can't cancel the menu demo
		}
		// show help for current demo
		else
		{
			if(entries[selectedIndex].isDir() == false)
			{
				hkString help = entries[selectedIndex].m_help;
				int end = help.indexOf('\n');
				if(end > 0)
				{
					help = help.substr(0,end);
				}
				addStatus(help);
			}
			else
			{
				addStatus("");
			}

			// keyboard accelerators
			const hkgKeyboard& kb = m_env->m_window->getKeyboard();
			for( int i = 0; i < entries.getSize(); ++i )
			{
				int j = (i + selectedIndex + 1) % entries.getSize();
				int c = hkString::toUpper( entries[j].m_name[0] );
				if( kb.wasKeyPressed( HKG_KEYBOARD_VKEY(c) ) )
				{
					selectedIndex = j;
					break;
				}
			}
		}
	}

	// update our current demo
	{
		int lastSlash = m_currentPath.lastIndexOf('/');
		if( lastSlash >= 0)
		{
			m_currentPath = m_currentPath.substr(0, lastSlash); // empty string if / not found
		}
		else
		{
			m_currentPath = "";
		}

		if ( selectedIndex >= 0)
		{
			if ( m_currentPath.getLength() )
			{
				m_currentPath += "/";
			}
			m_currentPath += entries[selectedIndex].m_name;
		}
	}

	// Can change current menu background color here if you like



	return DEMO_OK;
}


void MenuDemo::addStatus(hkString s)
{
	if(s.getLength())
	{
		m_statusLine = s;
	}
	else
	{
		m_statusLine.printf("Use %c%c%c%c to navigate and %c or %c to start.",
		TEXT_BUTTON_UP, TEXT_BUTTON_DOWN, TEXT_BUTTON_LEFT,	TEXT_BUTTON_RIGHT,
		TEXT_BUTTON_0, TEXT_BUTTON_START);
	}
}

hkString MenuDemo::getStatus()
{
	return m_statusLine;
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO(MenuDemo, HK_DEMO_TYPE_MENU, "Display a menu of available demos", "I'm the menu demo");

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
