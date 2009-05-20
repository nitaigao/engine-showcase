/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_HKGRAPHARRAY_H
#define HKBASE_HKGRAPHARRAY_H

#include <Common/Base/Container/Array/hkObjectArray.h>

// a class for a directed graph storing its data in arrays

template <class NODETYPE>
class hkGraphArray
{
 public:

    class Arc;
    class Node;

    // New types
    typedef hkUint32                           NodeID;
    typedef hkObjectArray<Node>                NodeArray;
    typedef typename NodeArray::iterator       NodeIterator;
    typedef typename NodeArray::const_iterator ConstNodeIterator;

    typedef hkGraphArray<NODETYPE>             GraphType;


    // hkGraphArray::Arc class
    class Arc
	{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_GEOMETRY, Arc);
		~Arc();

#if defined HK_COMPILER_MWERKS && (defined (HK_PLATFORM_PS2) || defined (HK_PLATFORM_PSP))
		HK_FORCE_INLINE hkGraphArray<NODETYPE>::NodeIterator terminal() const;
#else
		HK_FORCE_INLINE NodeIterator terminal() const;
#endif

	protected:
	    friend class hkGraphArray<NODETYPE>;

	    Arc(NodeIterator terminal);

	    NodeIterator m_terminal;
	};


    // New types
    typedef hkObjectArray<Arc>                ArcArray;
    typedef typename ArcArray::iterator       ArcIterator;
    typedef typename ArcArray::const_iterator ConstArcIterator;


    // hkGraphArray::Node class
    class Node
	{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_GEOMETRY, Node);
		~Node();

	    HK_FORCE_INLINE void mark();
	    HK_FORCE_INLINE void unmark();
	    HK_FORCE_INLINE hkBool marked() const;

	    HK_FORCE_INLINE hkBool outEmpty() const;
	    HK_FORCE_INLINE hkUint32 outSize() const;

#if defined HK_COMPILER_MWERKS && (defined (HK_PLATFORM_PS2) || defined (HK_PLATFORM_PSP))
	    HK_FORCE_INLINE hkGraphArray<NODETYPE>::ArcIterator outBegin();
	    HK_FORCE_INLINE hkGraphArray<NODETYPE>::ArcIterator outEnd();
	    HK_FORCE_INLINE hkGraphArray<NODETYPE>::ConstArcIterator outBegin() const;
	    HK_FORCE_INLINE hkGraphArray<NODETYPE>::ConstArcIterator outEnd() const;
#else
	    HK_FORCE_INLINE ArcIterator outBegin();
	    HK_FORCE_INLINE ArcIterator outEnd();
	    HK_FORCE_INLINE ConstArcIterator outBegin() const;
	    HK_FORCE_INLINE ConstArcIterator outEnd() const;
#endif

	    HK_FORCE_INLINE NODETYPE & operator * ();
	    HK_FORCE_INLINE NODETYPE * operator -> ();
	    HK_FORCE_INLINE const NODETYPE & operator * () const;
	    HK_FORCE_INLINE const NODETYPE * operator -> () const;

	    NODETYPE & operator = (const NODETYPE & e);

	protected:
	    friend class hkGraphArray<NODETYPE>;
	    friend class hkArray<Node>;

	    Node(ArcArray * arcs);

	    ArcArray * m_arcs;
	    hkUint32   m_begin;
	    hkUint32   m_end;

	    NODETYPE   m_elem;
	    hkBool     m_marker;
	};


    hkGraphArray();
    HK_FORCE_INLINE explicit hkGraphArray(hkUint32 numNodes);

    // Node related member functions
    HK_FORCE_INLINE hkBool isEmpty() const;
    HK_FORCE_INLINE hkUint32 getSize() const;

    HK_FORCE_INLINE Node & operator [] (NodeID i);
    HK_FORCE_INLINE const Node & operator [] (NodeID i) const;

    HK_FORCE_INLINE NodeIterator begin();
    HK_FORCE_INLINE NodeIterator end();
    HK_FORCE_INLINE ConstNodeIterator begin() const;
    HK_FORCE_INLINE ConstNodeIterator end() const;

    // Arc related member functions
    HK_FORCE_INLINE ArcIterator insertArc(NodeID initial, NodeID terminal);
    HK_FORCE_INLINE ArcIterator insertArc(NodeIterator initial, NodeIterator terminal);

    // Optimized (overloaded) functions
    HK_FORCE_INLINE void swap(GraphType & right);
    //friend void swap(GraphType & left, GraphType & right);

 protected:
    hkGraphArray(const GraphType &);
    GraphType & operator = (const GraphType &);

	ArcArray  m_arcs;
    NodeArray m_nodes;
};

#include <Graphics/Common/Geometry/FaceSet/hkGraphArray.inl>

#endif // HKBASE_HKGRAPHARRAY_H

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
