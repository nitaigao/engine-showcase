#include "Win32Clock.h"

float Win32Clock::GetDeltaMilliseconds()
{
	m_endFrameTime = timeGetTime( );

	float deltaMilliseconds = ( m_endFrameTime - m_startFrameTime ) / 1000.0f;

	m_startFrameTime = timeGetTime( );

	return deltaMilliseconds;
}