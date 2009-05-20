/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/SplashScreen/SplashScreen.h>

#include <Graphics/Common/hkGraphics.h>
#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/Texture/hkgTexture.h>
#include <Graphics/Common/Font/hkgFont.h>
#include <Common/Base/Types/Color/hkColor.h>
#include <Common/Base/Algorithm/Sort/hkSort.h>

/*static*/ const SplashScreen::SplashFile SplashScreen::m_clothSplashFiles[] =
{
	{ "./resources/cloth/splash/splash-C_final_4_3.png",  1.333f },
	{ "./resources/cloth/splash/splash-C_final_16_9.png",  1.777f },
};

/*static*/ const SplashScreen::SplashFile SplashScreen::m_destructionSplashFiles[] =
{
	{ "Resources/Destruction/splash/splash-D_final_4_3.png", 1.333f   },
	{ "Resources/Destruction/splash/splash-D_final_16_9.png", 1.777f  },
};


SplashScreen::SplashScreen (hkDefaultDemo* theDemo, SplashType type, bool enableProgressBar) :

m_demo(theDemo),
m_splashOK(false),
m_progressBarEnabled(enableProgressBar),
m_progressPercentage(0.0f)

{
	m_message = "";

	m_window = theDemo->m_env->m_window;
	hkgDisplayContext* ctx = m_window->getContext();
	ctx->lock();

	// Found out what splash files of the requested type are available
	hkArray<SplashFile> splashFiles;
	switch (type)
	{
		case CLOTH_SPLASH:
		{
			int numClothSplashes = sizeof(SplashScreen::m_clothSplashFiles) / sizeof(SplashScreen::SplashFile);
			for (int i=0; i<numClothSplashes; ++i)
			{
				splashFiles.pushBack(SplashScreen::m_clothSplashFiles[i]);
			}
		}
		break;

		case DESTRUCTION_SPLASH:
		{
			int numDestructionSplashes = sizeof(SplashScreen::m_destructionSplashFiles) / sizeof(SplashScreen::SplashFile);
			for (int i=0; i<numDestructionSplashes; ++i)
			{
				splashFiles.pushBack(SplashScreen::m_destructionSplashFiles[i]);
			}
		}
		break;

		default:
		{
			ctx->unlock();
			return;
		}
	}

	// Determine current screen aspect ratio
	float currentAspect = (float) m_window->getWidth() / (float) m_window->getHeight() ;

	float minDistAsspectDif = 2;
	int bestMatch = 1; // choose 16:9 if in doubt
	for (int i=0; i<splashFiles.getSize(); ++i)
	{
		float diff = hkMath::fabs( splashFiles[i].m_aspect - currentAspect);
		if ( diff < minDistAsspectDif )
		{
			minDistAsspectDif = diff;
			bestMatch = i;
		}
	}

	m_splashFile = splashFiles[ bestMatch ];
	const char* splashFilename = m_splashFile.m_filename;

	// Load splash texture
	m_splashTexture = hkgTexture::create(ctx);
	m_splashTexture->loadFromFile(splashFilename);
	m_splashTexture->realize();

	// Create texture for the text
	hkgTexture* ft = hkgTexture::create(ctx);
	ft->loadBuiltIn( HKG_TEXTURE_FONT );
	ft->realize();

	m_font = hkgFont::create();
	m_font->setTexture( ft );
	float charW = m_font->getCharWidth();
	float charH = m_font->getCharHeight();
	float ratio = charW / charH; 
	m_charHeight =  m_window->getHeight() > 480 ? 12 : 8;
	m_font->setCharHeight((float)m_charHeight);
	m_font->setCharWidth(((float)m_charHeight)*ratio);
	ft->release();

	m_splashOK = true;

	// Draw splash now
	draw();

	ctx->unlock();
}


SplashScreen::~SplashScreen()
{
	m_window->getContext()->lock();

	if (m_splashTexture) m_splashTexture->release();
	if (m_font) m_font->release();

	m_window->getContext()->unlock();
}


// Fill the progress bar to the specified percentage (and re-render)
void SplashScreen::updateProgress(hkReal percentage)
{
	if (!m_progressBarEnabled || !m_splashOK) return;

	m_progressPercentage = percentage;
	m_progressPercentage = hkMath::clamp(m_progressPercentage, 0.f, 100.f);
	draw();
}

// Provide a message string to be displayed henceforth under the progress bar (and re-render)
void SplashScreen::setMessage(const char* msg)
{
	if (!m_progressBarEnabled || !m_splashOK) return;
	m_message = msg;
	draw();
}
void SplashScreen::setMessage(const hkString& msg)
{
	if (!m_progressBarEnabled || !m_splashOK) return;
	m_message = msg;
	draw();
}


void SplashScreen::draw()
{
	hkgDisplayContext* ctx = m_window->getContext();
	ctx->lock();

	ctx->setLightingState( false );

	// Adds a ref to m_splashTexture
	ctx->setCurrentSoleTexture( m_splashTexture, HKG_TEXTURE_MODULATE );

	hkgViewport* orthoView = m_window->getWindowOrthoView();
	orthoView->setAsCurrent(ctx);

	bool logoState = m_window->getWantDrawHavokLogo();
	m_window->setWantDrawHavokLogo(false);

	drawSplash();
	if (m_progressBarEnabled)
	{
		drawProgressBar();
	}


	float restoreBK[3];
	m_window->getClearColor(restoreBK);
	m_window->setClearColor(HKG_VEC3_ZERO);
	
	m_window->swapBuffers();
	m_window->clearBuffers();

	m_window->setClearColor(restoreBK);

	m_window->setWantDrawHavokLogo(logoState);

	// Removes a ref from m_splashTexture
	ctx->setCurrentSoleTexture(HK_NULL, HKG_TEXTURE_MODULATE);
	ctx->unlock();
}


void SplashScreen::drawSplash()
{
	hkgDisplayContext* ctx = m_window->getContext();

	float tl[2];
	float lr[2];
	tl[0] = (float)0;
	tl[1] = (float)m_window->getHeight();
	lr[0] = (float)m_window->getWidth();

	if (m_progressBarEnabled)
	{
		lr[1] = (float)(m_charHeight * 2);
	}
	else
	{
		lr[1] = 0.0f;
	}
	

	ctx->setTexture2DState( true );
	ctx->setBlendState( false );

	float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	ctx->setCurrentColor4( white );

	struct SplashPoint
	{
		float x, y;
		float u, v;
	};
	SplashPoint points[6] =
	{
		{tl[0], tl[1], 0.0f, 1.0f},
		{tl[0], lr[1], 0.0f, 0.0f},
		{lr[0], tl[1], 1.0f, 1.0f},
		{tl[0], lr[1], 0.0f, 0.0f},
		{lr[0], tl[1], 1.0f, 1.0f},
		{lr[0], lr[1], 1.0f, 0.0f}
	};

	float p[3];
	float uv[2];
	p[2] = -0.01f;

	ctx->beginGroup( HKG_IMM_TRIANGLE_LIST );
	for (int i=0; i<6; ++i)
	{
		p[0] = points[i].x; p[1] = points[i].y;
		uv[0] = points[i].u; uv[1] = points[i].v;
		ctx->setCurrentTextureCoord( uv );
		ctx->setCurrentPosition( p );
	}
	ctx->endGroup();
}

void SplashScreen::drawProgressBar()
{
	hkgDisplayContext* ctx = m_window->getContext();

	ctx->setTexture2DState( false );
	ctx->setBlendState( false );

	float W = (float)m_window->getWidth();

	// Height of progress region in window
	float h = ( m_charHeight * 2.0f );

	float bh = 0.2f;  // vertical height of progress bar as a fraction of h
	float bc = 0.7f;  // vertical position of center of progress bar as a fraction of h
	float bw = 0.75f; // width of progress bar as a fraction of total screen width

	// Coords of full bar
	float tl[2];
	float lr[2];
	tl[0] = W * (0.5f - 0.5f*bw);
	tl[1] = h * (bc + 0.5f*bh);
	lr[0] = tl[0] + W * bw * (m_progressPercentage/100.0f);
	lr[1] = h * (bc - 0.5f*bh);

	struct SplashPoint
	{
		float x, y;
	};
	SplashPoint points[6] =
	{
		{tl[0], tl[1]},
		{tl[0], lr[1]},
		{lr[0], tl[1]},
		{tl[0], lr[1]},
		{lr[0], tl[1]},
		{lr[0], lr[1]}
	};

	float p[3];
	p[2] = -0.01f; //depth

	// Draw bar
	float grey[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	ctx->setCurrentColor4( grey );

	ctx->beginGroup( HKG_IMM_TRIANGLE_LIST );
	for (int i=0; i<6; ++i)
	{
		p[0] = points[i].x; p[1] = points[i].y;
		ctx->setCurrentPosition( p );
	}
	ctx->endGroup();

	// Draw border line
	float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	ctx->setCurrentColor4( white );

	float lrborder = W * (0.5f + 0.5f*bw);
	SplashPoint border[4] =
	{
		{tl[0], tl[1]},
		{tl[0], lr[1]},
		{lrborder, lr[1]},
		{lrborder, tl[1]},
	};

	ctx->beginGroup( HKG_IMM_LINES );
	for (int i=0; i<4; ++i)
	{
		p[0] = border[i].x; p[1] = border[i].y;
		ctx->setCurrentPosition( p );
		p[0] = border[(i+1)%4].x; p[1] = border[(i+1)%4].y;
		ctx->setCurrentPosition( p );
	}
	ctx->endGroup();

	// Text message
	float textX = tl[0];
	float textY = lr[1] - 1.1f * m_charHeight;

	hkgFont::setDrawState( ctx );
	ctx->setTexture2DState( true );
	ctx->setBlendState( true );
	m_font->render( ctx, m_message.cString(), textX, textY, white);
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
