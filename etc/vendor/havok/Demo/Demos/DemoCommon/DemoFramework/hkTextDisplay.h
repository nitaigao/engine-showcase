/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_TEXT_DISPLAY_H
#define HK_TEXT_DISPLAY_H

#include <Common/Base/Container/Array/hkObjectArray.h>
class hkgFont;
class hkgCamera;
class hkgWindow;

// These correspond to the buttons in the same order as hkgPad.h
#define TEXT_BUTTON_0		hkUchar(0220)
#define TEXT_BUTTON_1		hkUchar(0221)
#define TEXT_BUTTON_2		hkUchar(0222)
#define TEXT_BUTTON_3		hkUchar(0223)
#define TEXT_BUTTON_UP		hkUchar(0224)
#define TEXT_BUTTON_DOWN	hkUchar(0225)
#define TEXT_BUTTON_LEFT	hkUchar(0226)
#define TEXT_BUTTON_RIGHT	hkUchar(0227)
#define TEXT_BUTTON_SELECT	hkUchar(0230)
#define TEXT_BUTTON_START	hkUchar(0231)
#define TEXT_BUTTON_L1		hkUchar(0232)
#define TEXT_BUTTON_R1		hkUchar(0233)
#define TEXT_BUTTON_L2		hkUchar(0234)
#define TEXT_BUTTON_R2		hkUchar(0235)

/// Utility class for persistent text display.
class hkTextDisplay : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO_FRAMEWORK);

			/// Create a text display attached to window 'window'
		hkTextDisplay( hkgWindow* window );

		virtual ~hkTextDisplay();

			/// Render the stored text onto 'window'.
		void displayText(hkgWindow* window);

			/// Request 'str' to be rendered at absolute screen position x,y.
		void outputText(const hkString& str, hkUint32 x, hkUint32 y, hkUint32 color = 0xffffffffU, int frames = 1, int highlightLine = -1) { outputText(str, (float)x, (float)y, color, frames, highlightLine ); }
		void outputText(const hkString& str, int x, int y, hkUint32 color = 0xffffffffU, int frames = 1, int highlightLine = -1 ) { outputText(str, (float)x, (float)y, color, frames, highlightLine ); }
		virtual void outputText(const hkString& str, float x, float y, hkUint32 color = 0xffffffffU, int frames = 1, int highlightLine = -1 );

			/// Request 'str' to be rendered at world position x,y,z (which will be the lower left of the text)
		virtual void outputText3D(const hkString& str, float x, float y, float z, hkUint32 color = 0xffffffffU, int frames = 1 );

			/// Clear all current text.
		virtual void wipeText();

		const hkgFont* getFont() const { return m_font; }

			/// 2D lines visible
		int getNumVisibleLines(const hkgWindow* win) const;

			/// Request 'str' to be rendered at absolute screen position x,y.
			/// If the text runs over wrapX it automtically moves to the next line.
			/// The method also breaks word appropriately on spaces rather than in the middle.
		virtual void outputTextWithWrapping(const hkString& str, int x, int y, int wrapX, hkUint32 color = 0xffffffffU, int frames = 1 );


		struct OutputString
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO_FRAMEWORK, OutputString);
				hkString str;
			int frames;
			int highlightLine;
			float x;
			float y;
			float c[4]; // color rgb alpha
		};

		struct OutputString3D
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO_FRAMEWORK, OutputString3D);
				hkString str;
			int frames;
			float x;
			float y;
			float z;
			float c[4]; // color rgb alpha
		};

	public:
		bool m_holdTextForDebug;

	private:

		hkObjectArray<OutputString> m_strings;
		hkObjectArray<OutputString3D> m_strings3D;
		mutable class hkCriticalSection* m_stringLock;
		hkgWindow*			m_curWindow;
		hkgFont*			m_font;
		hkgCamera*			m_fontCam; // ortho cam for the font
};

class hkTextLog : public hkReferencedObject
{
	public:

		hkTextLog( int numLines, int top, int left );
		~hkTextLog();

		void outputText( const hkString& str  );

			// Call this every frame
		void displayLog( hkTextDisplay& display );

	protected:

		class hkCriticalSection* m_stringLock;
		hkObjectArray<hkString> m_lines;
		int m_top;
		int m_left;
		int m_numLines;
};


#endif // HK_TEXT_DISPLAY_H

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
