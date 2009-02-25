/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef INC_COMMON_RESOURCE_HANDLE_H
#define INC_COMMON_RESOURCE_HANDLE_H


#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkClassMemberAccessor.h>
#include <Common/Base/Container/StringMap/hkStringMap.h>


extern const hkClass hkMemoryResourceHandleClass;
extern const hkClass hkMemoryResourceContainerClass;


class hkResourceContainer;

class hkResourceMap;

	/// A virtual interface to a resource which is owned by the hkResourceContainer
class hkResourceHandle: public hkReferencedObject
{
	public:

		HK_DECLARE_REFLECTION();
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_EXPORT );

			/// A buffer to store temporary names
		typedef char NameBuffer[32];

			/// An external link
		struct Link
		{
			Link() {}
			Link(void* object, const hkClassMember* member) : m_memberAccessor(object, member) {}

			const char*				m_memberName;
			const char*				m_externalId;
			hkClassMemberAccessor	m_memberAccessor;
		};

			/// Returns the name of the resource.
			/// If the name has to be created on the fly the supplied buffer will be used.
		virtual const char* getName(NameBuffer buffer) const = 0;

			/// Set the name of the resource.
			/// A copy of the name will be stored.
		virtual void setName(const char* name) = 0;

			/// Returns a pointer to the object.
		virtual void* getObject() const = 0;

			/// Returns a pointer to the class.
		virtual const hkClass* getClass() const = 0;

			/// Set the object and class.
		virtual void setObject(void* object, const hkClass* klass) = 0;

			/// Adds a new external link, specified by the 'name' of the referencing object as well as the memberName.
			/// If the object uses nested structures, the memberName will look like:   xxx.yyy.zzz
		virtual void addExternalLink(const char* memberName, const char* m_externalId) = 0;

			/// removes an internal link
		virtual void removeExternalLink( const char* memberName ) = 0;

			/// Returns a list with all unresolved external links.
		virtual void getExternalLinks(hkArray<Link>& linksOut) = 0;

			/// Clears the list with all external links.
		virtual void clearExternalLinks() = 0;

			/// tryToResolveLinks
		virtual void tryToResolveLinks(hkResourceMap& map);

			// serializing constructor
		hkResourceHandle(hkFinishLoadedObjectFlag flag): hkReferencedObject(flag) {}

	protected:

		hkResourceHandle() {}
		virtual ~hkResourceHandle() {}
};

	/// The owner of a resource handle
class hkResourceContainer: public hkReferencedObject
{
	public:

		HK_DECLARE_REFLECTION();
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_EXPORT );

			/// Empty constructor
		hkResourceContainer(){}

			// Serializing constructor
		hkResourceContainer(hkFinishLoadedObjectFlag flag) : hkReferencedObject(flag) {}

			/// Create an owned resource 
		virtual hkResourceHandle* createResource(const char* name, void* object, const hkClass* klass) = 0;

			/// Destroys a resource
		virtual void destroyResource( hkResourceHandle* handle ) {}

			///	Get number of resources
		virtual int getNumResources() = 0;

			/// Tries to find a resource with matching name and hkClass
			/// to the first object after prevObject, with a name corresponding to 'objectName'.
			///   - If prevObject is null then the search begins from the start of the container.
			///   - If objectName is null than all objects will match.
			///   = \a klass is only used for extra debugging checks, it cannot be used for searching for a type!
		virtual hkResourceHandle* findByName( const char* objectName, const hkClass* klass = HK_NULL, const hkResourceHandle* prevObject = HK_NULL ) const = 0;

			/// Remove a resource from the container.
		virtual void removeResource(hkResourceHandle* resourceHandle) = 0;

		//
		//	Simple helper functions
		//

			/// helper function which tries to resolve external links
		virtual void tryToResolveLinks( hkResourceMap& resourceMap );
};


	/// Simple class which allows for fast searching of object/classes
class hkResourceMap
{
	public:

			/// Tries to find a resource with matching name and hkClass
			/// to the first object after prevObject, with a name corresponding to 'objectName'.
			///   - If prevObject is null then the search begins from the start of the container.
			///   - If objectName is null than all objects will match.
			///   = \a klass is only used for extra debugging checks, it cannot be used for searching for a type!
		virtual void* findObjectByName( const char* objectName, const hkClass** klassOut = HK_NULL ) const = 0;

	protected:

		virtual ~hkResourceMap(){}

		hkResourceMap(){}
};




	/// A simple Havok serializable version of a resource 
class hkMemoryResourceHandle: public hkResourceHandle
{
	//+vtable(true)
	public:

		HK_DECLARE_REFLECTION();

			/// Constructor
		hkMemoryResourceHandle();

			// Serializing Constructor
		hkMemoryResourceHandle( hkFinishLoadedObjectFlag flag);
			
			/// Destructor
		virtual ~hkMemoryResourceHandle();

			/// This structure stores information about an external link.
		struct ExternalLink 
		{
			public:

				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_EXPORT, hkMemoryResourceHandle::ExternalLink );
				HK_DECLARE_REFLECTION();

					/// The member name that stores the link to the external object.
					/// If the name will look like xxx.yyy.zzz
				const char* m_memberName;

					/// The 'name' of the linked object.
				const char* m_externalId;

					/// True if m_externalId has been allocated by the handle itself and has to be deallocated when clearing this link.
				hkBool m_externalIdIsAllocated;

					/// True if m_memberName has been allocated by the handle itself and has to be deallocated when clearing this link.
				hkBool m_memberNameIsAllocated;
		};

			/// Returns the name of the resource.
			/// If the name has to be created on the fly the supplied buffer will be used.
		const char* getName(NameBuffer buffer) const;

			/// Set the name of the resource.
			/// A copy of the name will be stored.
		void setName(const char* name);

			/// Returns a pointer to the object.
		void* getObject() const;

			/// Returns a pointer to the class.
		const hkClass* getClass() const;

			/// Set the object and class.
		void setObject(void* object, const hkClass* klass);

		void addExternalLink(const char* memberName, const char* m_externalId);

			/// removes an external link
		void removeExternalLink( const char* memberName );

			/// Returns a list with all unresolved external links.
		void getExternalLinks(hkArray<Link>& linksOut);

			/// Clears the list with all external links.
		void clearExternalLinks();

	protected:

		hkVariant   m_variant;
		hkBool      m_nameIsAllocated;
		const char* m_name;

		hkArray<struct hkMemoryResourceHandle::ExternalLink> m_references;
};





	/// Simple Havok serializable version of a hkResourceContainer
class hkMemoryResourceContainer : public hkResourceContainer
{
	//+vtable(true)
	public:

		HK_DECLARE_REFLECTION();

		hkMemoryResourceContainer();

		hkMemoryResourceContainer(hkFinishLoadedObjectFlag flag);

		virtual ~hkMemoryResourceContainer();

		hkResourceHandle* createResource(const char* name, void* object, const hkClass* klass);

		int getNumResources() { return m_resourceHandles.getSize(); }

		hkResourceHandle* findByName( const char* objectName, const hkClass* klass = HK_NULL, const hkResourceHandle* prevObject = HK_NULL ) const;

		void removeResource(hkResourceHandle* resourceHandle);

	protected:

		hkArray<hkMemoryResourceHandle*> m_resourceHandles; 
};


	/// A simple hash table to hkResourceHandles
class hkContainerResourceMap: public hkResourceMap
{
	public:

		hkContainerResourceMap( class hkResourceContainer* container );

		virtual void* findObjectByName( const char* objectName, const hkClass** klassOut = HK_NULL ) const;

	protected:

		hkStringMap<hkResourceHandle*> m_resources;
};



#endif // INC_COMMON_RESOURCE_HANDLE_H


/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20080925)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2008
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
