/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>

#include <Demos/DemoCommon/DemoFramework/hkDemo.h>
#include <Graphics/Common/hkGraphics.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>

#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/Camera/hkgCamera.h>
#include <Graphics/Common/Font/hkgFont.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>
#include <Common/Base/Thread/CriticalSection/hkCriticalSection.h>

#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Base/Algorithm/Sort/hkSort.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

const int ROWS_IN_FONT = 8;
const int ROWS_IN_EXTRA = 5;


hkTextDisplay::~hkTextDisplay()
{
	m_font->release();
	m_fontCam->release();

	delete m_stringLock;
}

void hkTextDisplay::wipeText()
{
	m_strings.clear();
	m_strings3D.clear();
}

struct hkBox2D
{
	int minX;
	int maxX;
	int minY;
	int maxY;
};


class TextDistanceMore
{
public:
	TextDistanceMore(hkgCamera* cam) : m_camera(cam) { }
	hkBool operator() ( const hkTextDisplay::OutputString3D& a, const hkTextDisplay::OutputString3D& b )
	{
		float v[3];
		hkgVec3Sub(v, &a.x, m_camera->getFromPtr() );
		const float da = hkgVec3LengthSqrd(v);
		hkgVec3Sub(v, &b.x, m_camera->getFromPtr() );
		const float db = hkgVec3LengthSqrd(v);
		return ( da < db ); // reverse order.
	}

	hkgCamera* m_camera;
};

void hkTextDisplay::displayText(hkgWindow* window)
{
	m_stringLock->enter();

	// render text first:
	hkgDisplayContext* ctx = window->getContext();
	ctx->lock();

	hkgFont::setDrawState( ctx );
	hkgViewport* v = window->getCurrentViewport();
	hkgCamera* cam = v->getCamera();

	// 3D text first
	HK_ALIGN16(float m[16]);
	float charW = m_font->getCharWidth();
	float charH = m_font->getCharHeight();
	float ratio = charW / charH; // will keep the ratio, alter the height

	HKG_TIMER_BEGIN("3DText", this);

	if (m_strings3D.getSize() > 0)
	{
		// Want depth for 3D text so that it fits in scene properly
		HKG_ENABLED_STATE state = ctx->getEnabledState();
		if (!(state & HKG_ENABLED_ZWRITE))
			ctx->setDepthWriteState(true);
		if (!(state & HKG_ENABLED_ZREAD))
			ctx->setDepthReadState(true);

		// sort the 3d text array wrt to camera
		// as 3d text freqently overlaps, it is better to
		// draw in depth order to improve visual effect
		// ( the use of the double arg template doses not compile in VC6 )
		#if !defined(HK_COMPILER_MSVC_VERSION) || (HK_COMPILER_MSVC_VERSION >= 1300)		
			HKG_TIMER_BEGIN("HeapSort 3D Text", this);
			hkAlgorithm::heapSort<OutputString3D, TextDistanceMore>(m_strings3D.begin(), m_strings3D.getSize(), TextDistanceMore(cam));
			HKG_TIMER_END();
		#endif	
	}

	for (int j = m_strings3D.getSize() - 1; j >= 0; --j)
	{
		OutputString3D& s = m_strings3D[j];    

		ctx->pushMatrix();	
		float dist = cam->computeBillboardMatrix((float*)(&s.x), m);
		float newCharH = cam->computeIconVerticalDrawSize(dist, (int)charH, (int)(v->getHeight()) );
		ctx->multMatrix(m);
		
		m_font->setCharHeight(newCharH);
		m_font->setCharWidth(newCharH * ratio);

		m_font->render( window->getContext(), s.str.cString(), 0, 0, s.c); // render font at 0,0 wrt to billboard matrix

		if (!m_holdTextForDebug)
			s.frames--;
		if (s.frames <= 0)
		{
			m_strings3D.removeAt(j); // this is why we recurse in reverse order.
		}

		ctx->popMatrix();
	}
	HKG_TIMER_END();
	HKG_TIMER_BEGIN("2DText", this);

	m_font->setCharHeight(charH);
	m_font->setCharWidth(charW);

	// Don't want depth for 2D text or its borders.
	HKG_ENABLED_STATE state = ctx->getEnabledState();
	if (state & HKG_ENABLED_ZWRITE)
		ctx->setDepthWriteState(false);
	if (state & HKG_ENABLED_ZREAD)
		ctx->setDepthReadState(false);

	//
	// 2D text
	//
	m_fontCam->computeOrtho( 0, hkReal(v->getWidth()), 0, hkReal(v->getHeight()), 0, 1);
	m_fontCam->setAsCurrent( window->getContext() );

	HKG_TIMER_BEGIN("BoundingBoxes", this)

	hkLocalArray<hkBox2D> boxes(m_strings.getSize());
	hkLocalArray<hkBox2D> highlights(10);
	boxes.setSize(m_strings.getSize());

	{
		// Find the extents of the text
		for (int s=0 ; s < m_strings.getSize(); s++)
		{
			int width = 0;	// Max width in characters
			int height = 1; // max height
			int t = 0;
			for (int ch=0; ch < m_strings[s].str.getLength(); ch++)
			{
				// Handle \n
				if (m_strings[s].str[ch] == '\n')
				{
					height++;
					t = 0;
				}
				else
				{
					t++;
				}
				width = (width < t) ? t : width;
			}

			// highlights?
			if (m_strings[s].highlightLine >= 0 )
			{
				hkBox2D& b = highlights.expandOne();

				b.minX = (int)(m_strings[s].x);
				b.maxX = (int)(m_strings[s].x) + (width * (int)m_font->getCharWidth());
				b.minY = (int)(m_strings[s].y) + (m_strings[s].highlightLine * (int)m_font->getCharHeight());
				b.maxY = b.minY + (int)m_font->getCharHeight() ;
			}

			// main background
			boxes[s].minX = (int)(m_strings[s].x);
			boxes[s].maxX = (int)(m_strings[s].x) + width * (int)m_font->getCharWidth();
			boxes[s].minY = (int)(m_strings[s].y);
			boxes[s].maxY = (int)(m_strings[s].y) + height * (int)m_font->getCharHeight();
		}
	}


	// Merge boxes
	{
		for (int k=0; k < boxes.getSize(); k++)
		{
			for (int j=0; j < boxes.getSize() && k < boxes.getSize();)
			{
				if (j!=k)
				{
					const int tolerance = 12;
					//Check for overlap
					if ((boxes[k].minX - tolerance < boxes[j].maxX) && (boxes[k].maxX + tolerance > boxes[j].minX) &&
						(boxes[k].minY - tolerance < boxes[j].maxY) && (boxes[k].maxY + tolerance > boxes[j].minY))
					{
						//Merge j into k
						boxes[k].minX = hkMath::min2(boxes[k].minX, boxes[j].minX);
						boxes[k].minY = hkMath::min2(boxes[k].minY, boxes[j].minY);
						boxes[k].maxX = hkMath::max2(boxes[k].maxX, boxes[j].maxX);
						boxes[k].maxY = hkMath::max2(boxes[k].maxY, boxes[j].maxY);

						//Erase j
						boxes[j] = boxes[boxes.getSize()-1];
						boxes.popBack();
						k=0;
					}
					else
					{
						j++;
					}
				}
				else
				{
					j++;
				}

			}
		}
	}

	window->getContext()->setTexture2DState( false );

	float highlightcolor[4] = { 0.75f, 0.75f, 0.75f, 0.5f};
	float white[4] = { 0.5f, 0.5f, 0.5f, 0.5f};
	float white2[4] = { 0.2f, 0.2f, 0.5f, 0.5f};
	window->getContext()->setCurrentColor4( white );

	// Draw an alpha background overlay 
	for (int i=0; i < boxes.getSize(); i++)
	{
		float p[3];	p[2] = -0.01f; //depth (0..-2)
		float tl[2]; float lr[2];
		const int border = 5;
		tl[0] = (hkReal)boxes[i].minX - border; 
		tl[1] = (hkReal)window->getHeight() - boxes[i].minY + border; 
		lr[0] = (hkReal)boxes[i].maxX + border; 
		lr[1] = (hkReal)window->getHeight() - boxes[i].maxY - border;

		window->getContext()->setCurrentColor4( white );
		window->getContext()->beginGroup( HKG_IMM_TRIANGLE_LIST );
		p[0] = tl[0]; p[1] = tl[1]; 
		window->getContext()->setCurrentPosition( p );
		p[0] = tl[0]; p[1] = lr[1]; 
		window->getContext()->setCurrentPosition( p );
		p[0] = lr[0]; p[1] = tl[1]; 
		window->getContext()->setCurrentPosition( p );
		p[0] = lr[0]; p[1] = tl[1]; 
		window->getContext()->setCurrentPosition( p );
		p[0] = tl[0]; p[1] = lr[1];
		window->getContext()->setCurrentPosition( p );
		p[0] = lr[0]; p[1] = lr[1]; 
		window->getContext()->setCurrentColor4( white2 );
		window->getContext()->setCurrentPosition( p );
		window->getContext()->endGroup();

		float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f};
		window->getContext()->setCurrentColor4( black );
		window->getContext()->beginGroup( HKG_IMM_LINES );
		p[0] = tl[0]; p[1] = tl[1]; 
		window->getContext()->setCurrentPosition( p );
		p[0] = tl[0]; p[1] = lr[1]; 
		window->getContext()->setCurrentPosition( p );
		window->getContext()->setCurrentPosition( p );
		p[0] = lr[0]; p[1] = lr[1]; 
		window->getContext()->setCurrentPosition( p );
		window->getContext()->setCurrentPosition( p );
		p[0] = lr[0]; p[1] = tl[1];
		window->getContext()->setCurrentPosition( p );
		window->getContext()->setCurrentPosition( p );
		p[0] = tl[0]; p[1] = tl[1];
		window->getContext()->setCurrentPosition( p );
		window->getContext()->endGroup();
	}
	
	// highlights
	for (int i=0; i < highlights.getSize(); i++)
	{
		float p[3];	p[2] = -0.005f; //depth (0..-2)
		float tl[2]; float lr[2];
		const int border = 2;
		tl[0] = (hkReal)highlights[i].minX - border; 
		tl[1] = (hkReal)window->getHeight() - highlights[i].minY + border; 
		lr[0] = (hkReal)highlights[i].maxX + border; 
		lr[1] = (hkReal)window->getHeight() - highlights[i].maxY - border;

		window->getContext()->setCurrentColor4( highlightcolor );
		window->getContext()->beginGroup( HKG_IMM_TRIANGLE_LIST );
		p[0] = tl[0]; p[1] = tl[1]; 
		window->getContext()->setCurrentPosition( p );
		p[0] = tl[0]; p[1] = lr[1]; 
		window->getContext()->setCurrentPosition( p );
		p[0] = lr[0]; p[1] = tl[1]; 
		window->getContext()->setCurrentPosition( p );
		p[0] = lr[0]; p[1] = tl[1]; 
		window->getContext()->setCurrentPosition( p );
		p[0] = tl[0]; p[1] = lr[1];
		window->getContext()->setCurrentPosition( p );
		p[0] = lr[0]; p[1] = lr[1]; 
		window->getContext()->setCurrentColor4( white2 );
		window->getContext()->setCurrentPosition( p );
		window->getContext()->endGroup();
	}

	HKG_TIMER_END();

	window->getContext()->setTexture2DState( true );

	int offset = int(window->getHeight()) - int(m_font->getCharHeight());

	for (int k = m_strings.getSize() - 1; k >= 0; --k)
	{
		OutputString& s = m_strings[k];
		m_font->render( window->getContext(), s.str.cString(), s.x, offset - s.y, s.c);

		if (!m_holdTextForDebug)
			s.frames--;
		if (m_strings[k].frames <= 0)
		{
			m_strings.removeAt(k);
		}
	}

	m_stringLock->leave();
	ctx->unlock();

	HKG_TIMER_END();

//CK:	v->setCamera(cam);
}

void hkTextDisplay::outputTextWithWrapping(const hkString& str, int x, int y, int wrapX, hkUint32 color, int frames )
{
	// Fill up a string a char at a time
	int curX = x;
	int curY = y;

	// Can't draw text that's off screen
	if (curX > wrapX)
	{
		return;
	}

	int startCh = 0;
	int curCh = 0;
	int lastSpace = 0;
	int charWidth = (int)( getFont()->getCharWidth() );
	while (curCh < str.getLength())
	{
		while ( (curCh < str.getLength()) && ( curX < (wrapX - charWidth) ) && ( str[curCh] != '\n') )
		{
			if ( str[curCh] == ' ' )
			{
				lastSpace = curCh+1;
			}
			curCh++;
			curX += (int)( getFont()->getCharWidth() );
		}

		// If we didn't wrap 
		if (curX < (wrapX - charWidth))
		{
			lastSpace = curCh;
		}

		// Check for a line break
		if ( lastSpace == startCh )
		{
			// Couldn't wrap properly.
			if (str[lastSpace] != '\n')
			{
				break;
			}
			// Ignore the \n
			lastSpace++;
			//curY += getFont()->getCharHeight() + 2;
			curX = x;
			curCh++;
		}

		if ( lastSpace > startCh )
		{
			// Dump the string
			hkString s( &str.cString()[startCh], lastSpace - startCh);
			outputText(s, (float)x, (float)curY, color, frames, -1);

			// Move on...
			startCh = curCh = lastSpace;
			curY += (int)( getFont()->getCharHeight() ) + 2;
			curX = x;
		}
	}
}

void hkTextDisplay::outputText(const hkString& str, float x, float y, hkUint32 color, int frames, int highlightLine )
{
	if( str.getLength() > 0 )
	{
		m_stringLock->enter();

		OutputString& s = *m_strings.expandBy(1);
		
		s.frames = frames;
		s.highlightLine = highlightLine;
		// Move text in a bit on consoles (TV deadzone, normally we give about 20 pixels)
		s.x = x + m_curWindow->getTVDeadZoneH();
		s.y = y; 

		s.str = str;
		s.c[0] = ((color & 0x00ff0000) >> 16) / 256.0f;
		s.c[1] = ((color & 0x0000ff00) >>  8) / 256.0f;
		s.c[2] = ((color & 0x000000ff)      ) / 256.0f;
		s.c[3] = ((color & 0xff000000) >> 24) / 256.0f;
		if (s.c[3] < 0.05f)
			s.c[3] = 1.0f; // forgot the alpha in the color def.

		m_stringLock->leave();
	}
}

void hkTextDisplay::outputText3D(const hkString& str, float x, float y, float z, hkUint32 color, int frames )
{
	if( str.getLength() > 0 )
	{
		m_stringLock->enter();

		OutputString3D& s = *m_strings3D.expandBy(1);

		s.frames = frames;
		s.x = x;
		s.y = y; 
		s.z = z; 
		s.str = str;
		s.c[0] = ((color & 0x00ff0000) >> 16) / 256.0f;
		s.c[1] = ((color & 0x0000ff00) >>  8) / 256.0f;
		s.c[2] = ((color & 0x000000ff)      ) / 256.0f;
		s.c[3] = ((color & 0xff000000) >> 24) / 256.0f;
		if (s.c[3] < 0.05f)
			s.c[3] = 1.0f; // forgot the alpha in the color def.

		m_stringLock->leave();

	}
}


int hkTextDisplay::getNumVisibleLines(const hkgWindow* window) const
{
	int wh = window->getHeight();
	int ch = (int)(getFont()->getCharHeight());
	return wh / ch;	
}

hkTextDisplay::hkTextDisplay(hkgWindow* window)
{
	m_holdTextForDebug = false;
	m_stringLock = new hkCriticalSection(1000);

	m_curWindow = window;
	m_font = hkgFont::create();
	
	hkgTexture* ft = hkgTexture::create( window->getContext() );
	ft->loadBuiltIn( HKG_TEXTURE_FONT );

	// add special platform dependent parts to font texture

	// we modify the font texture by taking one row from the hkgFontExtra png
	// and copying it over the second row of the font texture. (not the first row 
	// because it contains the tab character which must be printable)
	{
		extern const unsigned char hkgFontExtraPNGData[];
		extern const unsigned int  hkgFontExtraPNGSize;

		hkgTexture* t = hkgTexture::create( window->getContext() );
		hkIstream s( hkgFontExtraPNGData, hkgFontExtraPNGSize );
		
		if ( t->loadFromPNG( s ) )
		{
			HK_ASSERT2(0x260998c7,  ft, "font texture not loaded");
			HK_ASSERT2(0x1f6dd5a0,  t->getBPP() == ft->getBPP(), "special font bpp does not match");
			HK_ASSERT2(0x22fbc480,  t->getWidth() == ft->getWidth(), "special font width does not match");
			// builtin font has 8 rows
			// specials have 5 rows (PlayStation(R)2, ngc, xbox, pc, wii)
			HK_ASSERT2(0x5f0c3701,  t->getHeight() / ROWS_IN_EXTRA <= ft->getHeight() / ROWS_IN_FONT, "special font height does not fit");
			
			// now overwrite the font data with the specials
			unsigned char* pfont = const_cast<unsigned char*>(ft->getDataPointer());
			int fontlen = (ft->getWidth() * ft->getHeight() * ft->getBPP()) / 8;
			int extralen = (t->getWidth() * t->getHeight() * t->getBPP()) / (8 * ROWS_IN_EXTRA);
			const unsigned char* src = t->getDataPointer();

#if defined(HK_PLATFORM_PS2) || defined( HK_PLATFORM_PSP ) || defined(HK_PLATFORM_PS3_PPU)
			src += 3 * extralen;
#elif defined(HK_PLATFORM_XBOX) || defined(HK_PLATFORM_XBOX360)
			src += 2 * extralen;
#elif defined(HK_PLATFORM_GC)
			
			src += extralen;

#else // PC
			//no offset needed
#endif
			hkString::memCpy( pfont + fontlen - 2 * extralen, src, extralen);
		}				

		t->release();
	}

	ft->realize();

	m_font->setTexture( ft );
	ft->release();

	m_fontCam = hkgCamera::create();
	m_fontCam->computeModelView(true);
}


hkTextLog::hkTextLog( int numLines, int top, int left )
{
	m_stringLock = new hkCriticalSection(1000);
	m_top = top;
	m_left = left;
	m_numLines = numLines;
}
hkTextLog::~hkTextLog()
{
	delete m_stringLock;
}


void hkTextLog::outputText( const hkString& str  )
{
	m_stringLock->enter();
	if (m_lines.getSize() == m_numLines )
	{
		m_lines.removeAtAndCopy( 0 );
	}
	m_lines.pushBack( str );
	m_stringLock->leave();
}

void hkTextLog::displayLog( hkTextDisplay& display )
{
	m_stringLock->enter();
	for (int i = 0; i < m_lines.getSize(); ++i )
	{
		display.outputText(m_lines[i], m_left, m_top + 20 * i );
	}
	m_stringLock->leave();
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
