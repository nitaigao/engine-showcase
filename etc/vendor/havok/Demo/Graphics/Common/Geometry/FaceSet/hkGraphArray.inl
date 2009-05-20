/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


template <class NODETYPE>
hkGraphArray<NODETYPE>::Arc::Arc(NodeIterator term) 
	: m_terminal(term) 
{ 
}

template <class NODETYPE>
hkGraphArray<NODETYPE>::Arc::~Arc()
{
}

template <class NODETYPE>
HK_FORCE_INLINE typename hkGraphArray<NODETYPE>::NodeIterator 
hkGraphArray<NODETYPE>::Arc::terminal() const 
{ 
	return m_terminal; 
}

template <class NODETYPE>
hkGraphArray<NODETYPE>::Node::Node(ArcArray * arcs) 
	: m_arcs(arcs)
	, m_begin(0xffffffff)
	, m_end(0xffffffff)
	, m_marker(false)
{ 
}

template <class NODETYPE>
hkGraphArray<NODETYPE>::Node::~Node() 
{
}

template <class NODETYPE>
HK_FORCE_INLINE void 
hkGraphArray<NODETYPE>::Node::mark() 
{ 
	m_marker = true; 
}

template <class NODETYPE>
HK_FORCE_INLINE void 
hkGraphArray<NODETYPE>::Node::unmark() 
{ 
	m_marker = false; 
}

template <class NODETYPE>
HK_FORCE_INLINE hkBool 
hkGraphArray<NODETYPE>::Node::marked() const 
{ 
	return m_marker; 
}

template <class NODETYPE>
HK_FORCE_INLINE hkBool 
hkGraphArray<NODETYPE>::Node::outEmpty() const 
{ 
	return (m_begin == m_end); 
}

template <class NODETYPE>
HK_FORCE_INLINE hkUint32 
hkGraphArray<NODETYPE>::Node::outSize() const 
{ 
	return (m_end - m_begin); 
}

template <class NODETYPE>
HK_FORCE_INLINE typename hkGraphArray<NODETYPE>::ArcIterator 
hkGraphArray<NODETYPE>::Node::outBegin() 
{ 
	return (m_arcs->begin() + m_begin); 
}

template <class NODETYPE>
HK_FORCE_INLINE typename hkGraphArray<NODETYPE>::ArcIterator 
hkGraphArray<NODETYPE>::Node::outEnd() 
{ 
	return (m_arcs->begin() + m_end); 
}

template <class NODETYPE>
HK_FORCE_INLINE typename hkGraphArray<NODETYPE>::ConstArcIterator 
hkGraphArray<NODETYPE>::Node::outBegin() const 
{ 
	return (m_arcs->begin() + m_begin); 
}

template <class NODETYPE>
HK_FORCE_INLINE typename hkGraphArray<NODETYPE>::ConstArcIterator 
hkGraphArray<NODETYPE>::Node::outEnd() const 
{ 
	return (m_arcs->begin() + m_end); 
}

template <class NODETYPE>
HK_FORCE_INLINE NODETYPE & 
hkGraphArray<NODETYPE>::Node::operator * () 
{ 
	return m_elem; 
}

template <class NODETYPE>
HK_FORCE_INLINE NODETYPE * 
hkGraphArray<NODETYPE>::Node::operator -> () 
{ 
	return &m_elem; 
}

template <class NODETYPE>
HK_FORCE_INLINE const NODETYPE & 
hkGraphArray<NODETYPE>::Node::operator * () const 
{ 
	return m_elem; 
}

template <class NODETYPE>
HK_FORCE_INLINE const NODETYPE * 
hkGraphArray<NODETYPE>::Node::operator -> () const 
{ 
	return &m_elem; 
}

template <class NODETYPE>
NODETYPE & 
hkGraphArray<NODETYPE>::Node::operator = (const NODETYPE & e) 
{ 
	return (m_elem = e); 
}


template <class NODETYPE>
hkGraphArray<NODETYPE>::hkGraphArray() 
{ 
}

template <class NODETYPE>
HK_FORCE_INLINE 
hkGraphArray<NODETYPE>::hkGraphArray(hkUint32 numNodes) 
	: m_arcs()
	, m_nodes(numNodes, Node(&m_arcs))
{
	// optimization: we consider that, averagely, a triangle may have at least 2 neighbors
	// otherwise we are just wasting a bit of memory, but not that much
	m_arcs.reserve(numNodes * 2);
}

// Node related member functions
template <class NODETYPE>
HK_FORCE_INLINE hkBool 
hkGraphArray<NODETYPE>::isEmpty() const 
{ 
	return m_nodes.isEmpty(); 
}

template <class NODETYPE>
HK_FORCE_INLINE hkUint32 
hkGraphArray<NODETYPE>::getSize() const 
{ 
	return m_nodes.getSize(); 
}

template <class NODETYPE>
HK_FORCE_INLINE typename hkGraphArray<NODETYPE>::Node & 
hkGraphArray<NODETYPE>::operator [] (NodeID i) 
{ 
	HK_ASSERT(0x84e6340a,i < getSize()); 
	return m_nodes[i]; 
}

template <class NODETYPE>
HK_FORCE_INLINE const typename hkGraphArray<NODETYPE>::Node & 
hkGraphArray<NODETYPE>::operator [] (NodeID i) const 
{ 
	HK_ASSERT(0x84e6340b,i < getSize()); 
	return m_nodes[i]; 
}

template <class NODETYPE>
HK_FORCE_INLINE typename hkGraphArray<NODETYPE>::NodeIterator 
hkGraphArray<NODETYPE>::begin() 
{ 
	return m_nodes.begin(); 
}

template <class NODETYPE>
HK_FORCE_INLINE typename hkGraphArray<NODETYPE>::NodeIterator 
hkGraphArray<NODETYPE>::end() 
{ 
	return m_nodes.end(); 
}

template <class NODETYPE>
HK_FORCE_INLINE typename hkGraphArray<NODETYPE>::ConstNodeIterator 
hkGraphArray<NODETYPE>::begin() const 
{ 
	return m_nodes.begin(); 
}

template <class NODETYPE>
HK_FORCE_INLINE typename hkGraphArray<NODETYPE>::ConstNodeIterator 
hkGraphArray<NODETYPE>::end() const 
{ 
	return m_nodes.end(); 
}

// Arc related member functions
template <class NODETYPE>
HK_FORCE_INLINE typename hkGraphArray<NODETYPE>::ArcIterator 
hkGraphArray<NODETYPE>::insertArc(NodeID initial, NodeID terminal) 
{
	HK_ASSERT(0x84e6340c,initial < getSize());
	HK_ASSERT(0x84e6340d,terminal < getSize());
	return insertArc(m_nodes.begin() + initial, m_nodes.begin() + terminal);
}

template <class NODETYPE>
HK_FORCE_INLINE typename hkGraphArray<NODETYPE>::ArcIterator 
hkGraphArray<NODETYPE>::insertArc(NodeIterator initial, NodeIterator terminal) 
{
	HK_ASSERT(0x84e6340e,(initial >= begin()) && (initial < end()));
	HK_ASSERT(0x84e6340f,(terminal >= begin()) && (terminal < end()));

	Node & node = * initial;

	if (node.outEmpty()) 
	{
		node.m_begin = m_arcs.getSize();
		node.m_end = m_arcs.getSize() + 1;
	} 
	else 
	{
		// we optimize here for hkTriangleStripper::buildConnectivityGraph()
		// we know all the Arcs for a given node are successively and sequentially added
		HK_ASSERT(0x84e63410,node.m_end == (hkUint32)m_arcs.getSize());

		++(node.m_end);
	}

	m_arcs.pushBack(Arc(terminal));

	ArcIterator it = m_arcs.end();
	return (--it);
}

// Optimized (overloaded) functions
template <class NODETYPE>
HK_FORCE_INLINE void 
hkGraphArray<NODETYPE>::swap(GraphType & right) 
{ 
	m_nodes.swap(right.m_nodes); 
	m_arcs.swap(right.m_arcs); 
}

//template <class NODETYPE>
//void swap(typename hkGraphArray<NODETYPE>::GraphType & left, typename hkGraphArray<NODETYPE>::GraphType & right)
//{ 
//	left.swap(right); 
//}


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
