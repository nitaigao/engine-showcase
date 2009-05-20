/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef WINDOWED_AVERAGE_H
#define WINDOWED_AVERAGE_H

#include <Common/Base/Algorithm/Sort/hkSort.h>

/// Utility class to compute a windowed average
/// Also capable of computing total average
class WindowedAverage
{
public:

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, WindowedAverage );

	/// Constructor
	WindowedAverage( int windowSize = 0 )
	{
		init( windowSize );
	}

	/// Push a new value into the windowed average
	void pushNewValue( hkReal value )
	{
		int mod = m_numSamples % m_windowSize;

		m_numSamples++;

		if ( m_numSamples <= m_windowSize )
		{
			m_samples.setSize( m_numSamples );
		}

		m_samples[ mod ] = value;
		m_total += value;
	}

	/// \return The last time pushed (or -1.0f if none pushed)
	hkReal getLastTime()
	{
		if( m_numSamples == 0)
		{
			return -1.0f;
		}
		int mod = (m_numSamples-1) % m_windowSize;
		return m_samples[ mod ];
	}

	/// \return The mean over the last min( getWindowSize(), getNumSamples() ) samples
	hkReal getWindowedMean() const
	{
		hkReal avg = 0.0f;
		const int size = m_samples.getSize();

		for( int i = 0; i < size; i++ )
		{
			avg += m_samples[i];
		}

		return avg / hkReal( size );
	}

	/// \return The median over the last min( getWindowSize(), getNumSamples() ) samples
	hkReal getWindowedMedian() const
	{
		const int size = m_samples.getSize();

		hkSort( ( hkReal* )m_samples.begin(), size );

		return 0.5f * ( m_samples[ size / 2 ] + m_samples[ ( size - 1 ) / 2 ] );
	}

	/// \return The standard deviation over the last min( getWindowSize(), getNumSamples() ) samples
	hkReal getWindowedStandardDeviation() const
	{
		const int size = m_samples.getSize();

		const hkReal mean = getWindowedMean();

		hkReal acc = 0.0f;

		for( int i = 0; i < size; i++ )
		{
			hkReal diff = ( m_samples[ i ] - mean );
			acc += diff * diff;
		}

		return hkMath::sqrt( 1.0f / hkReal( m_samples.getSize() - 1 ) * acc );
	}


	/// \return The max over the last min( getWindowSize(), getNumSamples() ) samples
	hkReal getWindowedMax() const
	{
		hkReal val = -HK_REAL_MAX;

		for ( int i = 0; i < m_samples.getSize(); i++ )
		{
			if ( m_samples[ i ] > val )
			{
				val = m_samples[ i ];
			}
		}

		return val;
	}

	hkReal getWindowedMin() const
	{
		hkReal val = HK_REAL_MAX;

		for ( int i = 0; i < m_samples.getSize(); i++ )
		{
			if ( m_samples[ i ] < val )
			{
				val = m_samples[ i ];
			}
		}

		return val;
	}

	/// \return The mean over ALL samples
	hkReal getTotalMean() const
	{
		return m_total / hkReal( m_numSamples );
	}

	/// \return The size of the sample window
	int getWindowSize() const
	{
		return m_windowSize;
	}

	/// \return The total number of all samples taken
	int getNumSamples() const
	{
		return m_numSamples;
	}

private:

	void init( int windowSize )
	{
		m_windowSize = windowSize;
		m_numSamples = 0;
		m_total = 0.0f;
		m_samples.reserve( windowSize );
	}

	hkArray< hkReal > m_samples;

	int m_numSamples; ///
	int m_windowSize; ///
	hkReal m_total; ///
};

#endif // WINDOWED_AVERAGE_H

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
