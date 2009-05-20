/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/DemoCommon/DemoFramework/hkTestDemo.h>
#include <Demos/DemoCommon/DemoFramework/hkDemoConsole.h>
#include <Demos/DemoCommon/DemoFramework/hkDoubleConvCheck.h>
#include <Demos/DemoCommon/DemoFramework/hkDemoFramework.h>

#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Graphics/Common/Window/hkgWindow.h>

#include <Common/Base/System/Io/OStream/hkOStream.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Base/System/Io/Reader/hkStreamReader.h>
#include <Common/Base/System/Stopwatch/hkStopwatch.h>
#include <Common/Base/System/Error/hkDefaultError.h>


#include <Common/Base/Fwd/hkstandardheader.h>
#include HK_STANDARD_HEADER(stdlib)
#include HK_STANDARD_HEADER(string)
#include HK_STANDARD_HEADER(setjmp)

	// C4611 interaction between '_setjmp' and C++ object destruction is non-portable
	// yes we know.
#	pragma warning(disable: 4611)

using namespace std;

/////////////////////////////

class TestDemo;
extern hkTestEntry* hkUnitTestDatabase;
hkTestEntry* hkUnitTestDatabase;
static jmp_buf s_jumpBuffer;
static TestDemo* s_testGame;


static const char* nthLine(const hkString& s, int n)
{
	const char* p = s.cString();
	while(*p && n)
	{
		n -= *p++ == '\n';
	}
	return p;
}


hkBool HK_CALL hkTestReport(hkBool32 cond, const char* desc, const char* file, int line)
{
	if(cond == 0)
	{
#if !defined(HKTEST_STANDALONE)
        s_testGame->failCurrentTest(TestDemo::FAIL_NORMAL);
#endif
		hkString demoOutput;
		demoOutput.printf("%s(%i):[HKTEST FAIL %s]\n", file, line, desc);
		s_testGame->readFileAndAppend(demoOutput);

        //.\Utilities\Menu\MenuDemo.cpp(28) : fatal error C1189: #error :  test
		demoOutput.printf("%s (%i) : error : %s\n", file, line, desc);
		s_testGame->readFileAndAppend(demoOutput);

	}
	return cond != false;
}

#if defined(HKTEST_STANDALONE)
#include <Common/Base/Memory/Memory/Pool/hkPoolMemory.h>
#include <cstdio>
using namespace std;

void HK_CALL pushDoubleConversionCheck(hkBool enable) { }
void HK_CALL popDoubleConversionCheck() { }

static void errorReport(const char* msg, void*)
{
	s_testGame->readFileAndAppend( hkString(msg) );
}

#ifdef __SPU__
extern "C" void write();
extern "C" void close();
extern "C" void unlink();
void write(){}
void close(){}
void unlink(){}
#endif


static char stackMemory[0x16000];
int main()
{
	hkPoolMemory poolMemory;
	hkThreadMemory threadMemory( &poolMemory ) ;
	threadMemory.setStackArea(stackMemory, sizeof(stackMemory));
	hkBaseSystem::init( &poolMemory, &threadMemory, errorReport );


	for( hkTestEntry* entry = hkUnitTestDatabase; entry != HK_NULL; entry = entry->m_next )
	{
		s_testGame->readFileAndAppend( hkString(entry->m_name)+" "+entry->m_path+"\n");
		(*entry->m_func)();
	}
	hkBaseSystem::quit();
	return 0;
}

#else

TestDemo::TestDemo(hkDemoEnvironment* env, hkBool autoRecoverFromErrors)
	:	hkDefaultDemo(env),
		m_autoRecoverFromErrors(autoRecoverFromErrors),
		m_firstLineShown(0),
		m_numPasses(0),
		m_numFails(0)
{
	
	// gets the demo name running ie all tests or slow and creates a file name
	// which we use to write to the appropriate file
	m_unitTestFileName = env->m_menuPath;
	m_unitTestFileName.replaceInplace("UnitTest/","",hkString::REPLACE_ALL);
	m_unitTestFileName += ".txt";
	
	// using the file name we write an empty string to make sure the file is empty also
	// this will create the file if it does not already exist
	char* empty = "";
	hkOstream Out(m_unitTestFileName.cString());
	Out.write(empty,0);
	Out.flush();

	m_error.m_owner = this;

	//hkprintf( "[HKTEST START]\n" );

	hkTestEntry* test = hkUnitTestDatabase;
	while (test)
	{
		hkString envPath(env->m_menuPath);
		hkString testPath(test->m_path);
		testPath = testPath.replace("\\","/");
		testPath = testPath.replace("./UnitTest/","");

		if (testPath.endsWith(env->m_menuPath) || envPath.endsWith(test->m_category) || envPath.endsWith("All Tests"))
		{
			m_tests.pushBack(test);
		}

		test = test->m_next;
	}
	m_currentTest = 0;
}

TestDemo::~TestDemo()
{
	hkString s;
	s.printf("[%i passes, %i fails] ('F'ailure, 'W'arning, 'A'ssert, 'E'rror)", m_numPasses, m_numFails);
	hkcout << s.cString() << hkendl;

	m_env->m_exitCode =  m_numFails > 0;
}

hkDemo::Result TestDemo::stepDemo()
{
	// Window dressing
	{
		int windowLines = m_env->m_textDisplay->getNumVisibleLines(m_env->m_window);
		const char* p = m_passString.cString();
		int textLines = 0;
		while(*p)
		{
			textLines += *p++ == '\n';
		}

		{
			m_env->m_textDisplay->outputText( nthLine(m_testString, m_firstLineShown), 70, 40);
			m_env->m_textDisplay->outputText( nthLine(m_failString, m_firstLineShown), 40, 40, 0xff0000);
			m_env->m_textDisplay->outputText( nthLine(m_passString, m_firstLineShown), 40, 40, 0x00ff00);
		}

		if( textLines + 6 > windowLines )
		{
			if(m_firstLineShown > 0)
			{
				m_env->m_textDisplay->outputText("^\n^", 10, 40);
				if( m_env->m_gamePad->isButtonPressed(HKG_PAD_DPAD_UP) )
				{

					{
						int amount = 1;
						m_firstLineShown = hkMath::max2(0, m_firstLineShown-amount);
					}
				}
			}

			if( textLines + 6 - m_firstLineShown > windowLines )
			{
				m_env->m_textDisplay->outputText("v\nv", 10, (int)m_env->m_window->getHeight() - 40);

				if( m_env->m_gamePad->isButtonPressed(HKG_PAD_DPAD_DOWN) )
				{
					{
						int amount = 1;
						m_firstLineShown = hkMath::min2(textLines+6-windowLines, m_firstLineShown+amount);
					}
				}
			}
		}
		{
			hkString s;
			s.printf("[%i passes, %i fails] ('F'ailure, 'W'arning, 'A'ssert, 'E'rror)", m_numPasses, m_numFails);
			m_env->m_textDisplay->outputText(s, 10, 10);
		}
	}

	// Run tests
	if (m_currentTest < m_tests.getSize())
	{
		hkTestEntry* currentTest = m_tests[m_currentTest];

		hkError* savedError = HK_NULL;
		if( m_autoRecoverFromErrors )
		{
			// change error handler to attempt to continue
			savedError = &hkError::getInstance();
			savedError->addReference();
			hkError::replaceInstance( &m_error );
		}
		
		m_currentTestOk = true;
		s_testGame = this;
		testRun(currentTest);

		// restore default error handler
		if( savedError )
		{
			m_error.addReference();
			hkError::replaceInstance(savedError);
		}

		if( m_currentTestOk )
		{
			m_numPasses++;
			m_passString += "O";
		}
		else
		{
			m_numFails++;
		}
		m_passString += "\n";
		m_failString += "\n";

	}

	// Next test
	m_currentTest++;

	// Finished
	if (m_currentTest == m_tests.getSize())
	{
		// this out put needs to match "[HKTEST FINISH]\n" or the python script will not
		// be able to find the finish tag
		readFileAndAppend( "[HKTEST FINISH]\n" );
		hkString demoOutput;
		demoOutput.printf("[%i passes, %i fails] ('F'ailure, 'W'arning, 'A'ssert, 'E'rror)", m_numPasses, m_numFails);
		readFileAndAppend(demoOutput);
		if (m_env->m_options->m_renderer
			&& m_env->m_options->m_renderer[0] == 'n')
		{
			return DEMO_STOP;
		}
	}

	return DEMO_OK;
}

void TestDemo::testRun(hkTestEntry* entry)
{
	const char* name = entry->m_name;
	m_testString += entry->m_name;
	m_testString += "\n";

	hkString demoOutput;
	demoOutput.printf("[HKTEST BEGIN '%s']\n", name);
	readFileAndAppend(demoOutput);
	if( setjmp(s_jumpBuffer) == 0 )
	{
		pushDoubleConversionCheck(true);

		(*entry->m_func)();

		popDoubleConversionCheck();
	}
	else
	{
        //entry-
		hkString path( entry->m_path );
		path = path.replace( "Test/UnitTest", ".\\" );
		path = path.replace( "/", "\\" );
		demoOutput.printf("%s (%i) : '%s' was aborted\n", __FILE__, __LINE__, name);
		readFileAndAppend(demoOutput);
	}

	// print the test's pass status
	if (m_currentTestOk != false)
	{
		readFileAndAppend("[HKTEST PASS]\n");
	}

	demoOutput.printf("[HKTEST END '%s']\n", name);
	readFileAndAppend(demoOutput);
	hkcout.flush();
}

void TestDemo::readFileAndAppend(hkString outPut)
{
	// prints the message to the console
	hkprintf(outPut.cString());
	m_TotalOutput += outPut;
	
	// opens IFstream to write out the array and append outPut to it
	hkOfstream* fileOut = new hkOfstream(m_unitTestFileName.cString());

	// writes out the array to the file
	fileOut->write( m_TotalOutput.cString(),m_TotalOutput.getLength());
	
	delete fileOut;
}
void TestDemo::failCurrentTest(FailReason f)
{
	if(m_currentTestOk)
	{
		m_currentTestOk = false;
		const char* fails[FAIL_MAX] = { "F", "A", "E" };
		m_failString += fails[f];
	}
}

static void errorReport(const char* msg, void* errorReportObject)
{
	s_testGame->readFileAndAppend( hkString(msg) );
}

TestDemoError::TestDemoError() : hkDefaultError( ( hkErrorReportFunction )( errorReport ) ), m_owner(HK_NULL)
{
}

int TestDemoError::message(Message msg, int id, const char* description, const char* file, int line)
{
	if( isEnabled(id) )
	{
		hkString demoOutput;
		switch (msg)
		{
			case hkDefaultError::MESSAGE_REPORT:
				if (id > 0)
				{
					demoOutput.printf("%s (%i) : Report 0x%x: %s\n", file, line, id, description);
					m_owner->readFileAndAppend(demoOutput);
				}
				break;
			case hkDefaultError::MESSAGE_ASSERT:
			{
				m_owner->failCurrentTest(TestDemo::FAIL_ASSERT);
				hkString mg( description );
				mg = mg.replace( "\n", "; " );
				demoOutput.printf("%s (%i) : assert error 0x%x: %s\n", file, line, id, mg.cString());
				m_owner->readFileAndAppend(demoOutput);
				longjmp(s_jumpBuffer, 1);
				break;
			}
			case hkDefaultError::MESSAGE_ERROR:
			{
				m_owner->failCurrentTest(TestDemo::FAIL_ERROR);
				hkString mg( description );
				mg = mg.replace( "\n", "; " );
				demoOutput.printf("%s (%i) : fatal error 0x%x: %s\n", file, line, id, mg.cString());
				m_owner->readFileAndAppend(demoOutput);
				longjmp(s_jumpBuffer, 1);
				break;
			}
			default:
			{
					// suspend warnings fired from:
					// - hkRigidBody::checkPerformance
					// - getConvexHullFromStriding
				if ( id == 0x2ff8c16f || id == 0x34df5494 )
				{
					break;
				}
				m_owner->m_passString += "W";
				hkString mg( description );
				mg = mg.replace( "\n", "; " );
				demoOutput.printf("%s (%i) : warning 0x%x: %s\n", file, line, id, mg.cString());
				m_owner->readFileAndAppend(demoOutput);
				// this is a place-holder.  we do not want so many warnings, so a verbose
				// flag would be nice.
				//hkprintf("%s (%i) : warning (0x%x): %s\n", file, line, id, description);
				break;
			}
		}
	}
	return 0;
}

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
