/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef INC_HKG_SPLASH_SCREEN_H
#define INC_HKG_SPLASH_SCREEN_H

class SplashScreen: public hkReferencedObject
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		// Currently supported splash types
		enum SplashType
		{
			CLOTH_SPLASH,
			DESTRUCTION_SPLASH
		};
	
		// Create and render immediately a splash screen (usually while a demo is initializing/loading).
		// If no progress bar is desired, set enableProgressBar to false (in this case, updateProgress() and setMessage() do nothing).
		// Otherwise, the progress bar is displayed (0% filled initially), and updated via updateProgress() and setMessage().
		SplashScreen (hkDefaultDemo* theDemo, SplashType type, bool enableProgressBar=true);

		~SplashScreen();

		// Fill the progress bar to the specified percentage (and re-render)
		void updateProgress(hkReal percentage);

		// Return the length of the progress bar as a percentage of total width
		hkReal getProgress() { return m_progressPercentage; }
			
		// Provide a message string to be displayed henceforth under the progress bar (and re-render)
		void setMessage(const char* msg);
		void setMessage(const hkString& msg);

		struct SplashFile
		{
			const char*	m_filename;
			float m_aspect;
		};

		const SplashFile* getSplashFileUsed() const { return &m_splashFile; }

	protected:

		static const SplashFile m_clothSplashFiles[];
		static const SplashFile m_destructionSplashFiles[];
		
		SplashFile m_splashFile;

		void draw();
		void drawSplash();
		void drawProgressBar();

		class hkDefaultDemo* m_demo;
		class hkgWindow* m_window;
		class hkgTexture* m_splashTexture;
		class hkgFont* m_font;

		bool m_splashOK;
		bool m_progressBarEnabled;
		float m_progressPercentage;
		hkString m_message;
		int m_charHeight;

};


#endif //INC_HKG_SPLASH_SCREEN_H

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
