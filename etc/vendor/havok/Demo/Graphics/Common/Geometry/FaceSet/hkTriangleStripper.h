/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


// Post T&L cache aware triangle stripifier in O(n.log(n)).
// based on the T&L cache simulator by Tanguy Fautré <softdev@telenet.be>


#ifndef HK_TRIANGLE_STRIPPER_H
#define HK_TRIANGLE_STRIPPER_H

#include <Common/Base/Algorithm/Sort/hkSort.h>
#include <Common/Base/Container/Array/hkObjectArray.h>
#include <Graphics/Common/Geometry/FaceSet/hkGraphArray.h>
#include <Graphics/Common/Geometry/FaceSet/hkHeapArray.h>

class hkTriangleStripper
{
 public:

    typedef hkUint32       Index;
    typedef hkArray<Index> Indices;

    typedef enum
	{
	    TRIANGLES		= 0x0004,	// = GL_TRIANGLES
	    TRIANGLE_STRIP	= 0x0005	// = GL_TRIANGLE_STRIP
	} PrimitiveType;

    class PrimitiveGroup
    {
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_GEOMETRY, hkTriangleStripper::PrimitiveGroup);
		PrimitiveGroup() : m_indices(HK_NULL) {}
		~PrimitiveGroup() {}

		Indices*      m_indices;
		PrimitiveType m_type;
    };

    typedef hkObjectArray<PrimitiveGroup> PrimitiveArray;

    hkTriangleStripper(const Indices & triIndices);

    void strip(PrimitiveArray * out_pPrimitivesVector);

	void concat(PrimitiveArray * inout_pPrimitivesVector);

    /// Stripifier Algorithm Settings

    /// Set the minimum size of a triangle strip (should be at least 2 triangles).
    /// The stripifier discards any candidate strips that does not satisfy the minimum size condition.
    HK_FORCE_INLINE void setMinStripSize(hkUint32 minStripSize = 2)
	{
	    if (minStripSize < 2)
			m_minStripSize = 2;
	    else
			m_minStripSize = minStripSize;
	}

    /// Set the backward search mode in addition to the forward search mode.
    /// In forward mode, the candidate strips are build with the current candidate triangle being the first
    /// triangle of the strip. When the backward mode is enabled, the stripifier also tests candidate strips
    /// where the current candidate triangle is the last triangle of the strip.
    /// Enable this if you want better results at the expense of being slightly slower.
    HK_FORCE_INLINE void setBackwardSearch(hkBool enabled = false) { m_backwardSearch = enabled; }


 private:

    class Triangle
	{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_GEOMETRY, hkTriangleStripper::Triangle);
	    Triangle() { }
	    HK_FORCE_INLINE Triangle(Index indexA, Index indexB, Index indexC) : m_A(indexA), m_B(indexB), m_C(indexC), m_stripNumber(0) { }
		~Triangle() {}

	    HK_FORCE_INLINE void resetStripNumber() { m_stripNumber = 0; }
	    HK_FORCE_INLINE void setStripNumber(hkUint32 id) { m_stripNumber = id; }
	    HK_FORCE_INLINE hkUint32 stripNumber() const { return m_stripNumber; }

	    HK_FORCE_INLINE Index A() const { return m_A; }
	    HK_FORCE_INLINE Index B() const { return m_B; }
	    HK_FORCE_INLINE Index C() const { return m_C; }

	private:
	    Index    m_A;
	    Index    m_B;
	    Index    m_C;
	    hkUint32 m_stripNumber;
	};



    class TriangleEdge
	{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_GEOMETRY, hkTriangleStripper::TriangleEdge);
		TriangleEdge() : m_A(0), m_B(0), m_triPos(0) {}
	    HK_FORCE_INLINE TriangleEdge(Index indexA, Index indexB, hkUint32 p=0) : m_A(indexA), m_B(indexB), m_triPos(p) { }
		~TriangleEdge() {}

	    HK_FORCE_INLINE Index A() const { return m_A; }
	    HK_FORCE_INLINE Index B() const { return m_B; }
		HK_FORCE_INLINE hkUint32 triPos() const { return m_triPos; }

	    HK_FORCE_INLINE hkBool operator == (const TriangleEdge & Right) const
		{
		    return ((A() == Right.A()) && (B() == Right.B()));
		}

	private:
	    Index    m_A;
	    Index    m_B;
		hkUint32 m_triPos;
	};

    typedef enum { ABC, BCA, CAB } TriangleOrder;

    class Strip
	{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_GEOMETRY, hkTriangleStripper::Strip);
	    Strip() : m_start(0), m_order(ABC), m_size(0) { }
	    HK_FORCE_INLINE Strip(hkUint32 s, TriangleOrder o, hkUint32 i) : m_start(s), m_order(o), m_size(i) { }
		~Strip() {}

	    HK_FORCE_INLINE hkUint32 start() const { return m_start; }
	    HK_FORCE_INLINE TriangleOrder order() const { return m_order; }
	    HK_FORCE_INLINE hkUint32 getSize() const { return m_size; }

	private:
	    hkUint32      m_start;
	    TriangleOrder m_order;
	    hkUint32      m_size;
	};

    void buildConnectivityGraph(hkGraphArray<Triangle> & tris, const Indices & indices);

    class CompareTriEdgeLess
	{
	public:
	    HK_FORCE_INLINE hkBool operator() (const TriangleEdge & a, const TriangleEdge & b) const
		{
		    const Index A1 = a.A();
		    const Index B1 = a.B();
		    const Index A2 = b.A();
		    const Index B2 = b.B();

		    if ((A1 < A2) || ((A1 == A2) && (B1 < B2)))
				return true;
		    else
				return false;
		}
	};


    typedef hkObjectArray<TriangleEdge> EdgeMap;


    void linkNeighbours(hkGraphArray<Triangle> & tris, const EdgeMap & edgeMap, const TriangleEdge edge)
	{
	    // Find the first edge equal to Edge
		EdgeMap::const_iterator it = edgeMap.begin();
		CompareTriEdgeLess cmpOp;
		for (; it != edgeMap.end(); ++it)
		{
			if (!cmpOp(*it,edge)) break;
		}

	    // See if there are any other edges that are equal
	    // (if so, it means that more than 2 triangles are sharing the same edge,
	    //  which is unlikely but not impossible)
	    for (; (it != edgeMap.end()) && (edge == (* it)); ++it)
			tris.insertArc(edge.triPos(), it->triPos());

	    // Note: degenerated triangles will also point themselves as neighbor triangles
	}


    typedef hkGraphArray<Triangle>                                 TriangleGraph;
    typedef hkHeapArray<hkUint32, hkAlgorithm::greater<hkUint32> > TriangleHeap;
    typedef hkArray<hkUint32>                                      Candidates;
    typedef TriangleGraph::NodeIterator                            TriangleGraphIterator;
    typedef TriangleGraph::ConstNodeIterator                       ConstTriangleGraphIterator;
    typedef TriangleGraph::ConstArcIterator                        ConstLinkIterator;

    void initTriHeap();
    void stripify();
    void addLeftoverTriangles();
    void resetStripNumbers();

    Strip findBestStrip();
    Strip extendToStrip(hkUint32 start, TriangleOrder order);
    Strip backExtendToStrip(hkUint32 start, TriangleOrder order, hkBool clockWise);
    ConstLinkIterator linkToNeighbour(ConstTriangleGraphIterator node, hkBool clockWise,
									  TriangleOrder & order, hkBool notSimulation);
    ConstLinkIterator backLinkToNeighbour(ConstTriangleGraphIterator node, hkBool clockWise,
										  TriangleOrder & order);
    void buildStrip(const Strip s);
    void markTriAsTaken(hkUint32 i);
    void addIndex(Index i, hkBool notSimulation);
    void addTriangle(const Triangle & tri, TriangleOrder order, hkBool notSimulation);

    static TriangleEdge firstEdge(const Triangle & tri, TriangleOrder order);
    static TriangleEdge lastEdge(const Triangle & tri, TriangleOrder order);

    PrimitiveArray m_primitivesArray;
    TriangleGraph  m_triangles;
    TriangleHeap   m_triangleHeap;
    Candidates     m_candidates;
    hkUint32       m_stripNumber;
    hkUint32       m_minStripSize;
    hkBool         m_backwardSearch;
    hkBool         m_firstRun;
};

#endif // HK_TRIANGLE_STRIPPER_H

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
