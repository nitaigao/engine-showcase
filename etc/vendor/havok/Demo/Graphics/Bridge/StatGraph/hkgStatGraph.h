/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKG_SYSTEM_H
#define HKG_STAT_H

#include <Common/Base/Monitor/MonitorStreamAnalyzer/hkMonitorStreamAnalyzer.h>

enum HKG_STAT_GRAPH_TYPE
{
	HKG_STAT_GRAPH_LINE, // Line graph (like the VDB stat graph), just uses Lines
	HKG_STAT_GRAPH_BAR   // Bar chart (like the tga dumps from the monitor streams), uses a Texture
};
class hkgFont;
class hkgTexture;

class hkgStatGraph : public hkReferencedObject
{
	public:

		hkgStatGraph();
		virtual ~hkgStatGraph();

			// can be called more than once (to change type, resize, etc)
		bool init(HKG_STAT_GRAPH_TYPE type, hkgDisplayContext* context, int width, int height, hkUint32 alphaKeyColor = 0x0 );  

			// Assumes Nodes in   [One Node Per Thread]->[With One Child per Frame], even if you only have one Frame
			// Warning: update may alter the node tree given to disable timers	
		void update( hkArray< hkMonitorStreamAnalyzer::Node*>& perThreadNodes, hkMonitorStreamFrameInfo& frameInfo, int numThreads, int numSpus );
		
		enum StatGraphUserActions 
		{
			HKG_STAT_GRAPH_ACTION_NONE = 0,
			HKG_STAT_GRAPH_ACTION_RESET_ZOOM,
			HKG_STAT_GRAPH_ACTION_PAN_NEGX,
			HKG_STAT_GRAPH_ACTION_PAN_POSX,
			HKG_STAT_GRAPH_ACTION_PAN_NEGY,
			HKG_STAT_GRAPH_ACTION_PAN_POSY
		};

		void zoom( int fromPixelX, int fromPixelY, int toPixelX, int toPixelY);
		void pan( hkReal percentX, int numThreadsY ); // usually say (-0.1f, 0) or (0.f, 1) etc 

		void display( int xPos, int yPos );

			// call this if the user presses a button over the stat graph. Will return what action it preformed, if any
		StatGraphUserActions userAction( int xMousePos, int yMousePos );

			// given an [x,y] in graph space (so within width / height as given in init()), display the stat details.
			// Handy for mouse hover etc 
		hkMonitorStreamAnalyzer::Node* findTimerNodeAtSample( hkArray< hkMonitorStreamAnalyzer::Node*>& perThreadNodes, hkMonitorStreamFrameInfo& frameInfo, int numThreads, int numSpus, int localX, int localY);

		void disableTimerBranch( const char* name ); // eg: "PostSimulateCb"
		void enableTimerBranch( const char* name );

		inline HKG_STAT_GRAPH_TYPE getType() const { return m_type; }

		inline hkInt32 getDisplayWidth() const { return m_displayWidth; }
		inline hkInt32 getDisplayHeight() const { return m_displayHeight; }
		inline hkInt32 getDisplayLastX() const { return m_displayLastX; }
		inline hkInt32 getDisplayLastY() const { return m_displayLastY; }

		void displayZoomExtents( int originX, int originY, int mouseX, int mouseY) const;

		struct ZoomData
		{
			hkReal m_zoomStartTime;
			hkReal m_zoomEndTime; 
			int m_zoomStartThread;
			int m_zoomEndThread;
			int m_zoomStartFrame;
			int m_zoomEndFrame;
		};

		const ZoomData& getCurrentZoomData() const { return m_zoomData.back(); }

	protected:
			
		void removeDisabledTimers( hkArray< hkMonitorStreamAnalyzer::Node*>& perThreadNodes );

		HKG_STAT_GRAPH_TYPE m_type;
		hkInt32 m_displayWidth;
		hkInt32 m_displayHeight;
		hkInt32 m_displayLastX;
		hkInt32 m_displayLastY;
		
		hkInt32 m_startOffset[2];
		hkInt32 m_statWidthHeight[2];

		hkUint32 m_alphaKeyColor; 
		hkgDisplayContext* m_context;
		hkgTexture* m_barChartTexture;
		hkgTexture* m_barChartSurroundTexture;
		hkgFont* m_statFont;
		hkReal m_lastFrameTimeBackup;

		// Color table for drawing the stats overlay
		struct hkMonitorStreamColorTable* m_colorTable;
		class hkPackfileData* m_colorTablePackfileData; 

		struct StatLine
		{
			hkVector4 start;
			hkVector4 end;
			hkUint32 color;
		};

		hkObjectArray< hkString > m_disabledTimers;

		struct LastUpdateData
		{
			int m_numFrames;
			int m_numThreads;
			int m_numSpus;
			int m_numRealThreads;
			int m_numRealSpus;
			int m_heightPerThread;
			int m_gapBetweenThreads;
			hkReal m_maxSampleTime;
			hkReal m_startSampleTimeLimit;
		};
		LastUpdateData m_lastUpdateData;

		hkArray< ZoomData > m_zoomData; // always has one element anyway 
	
		hkArray< StatLine > m_lines;
};	

#endif // HKG_SYSTEM_H

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
