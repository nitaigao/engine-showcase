/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/DemoCommon/DemoFramework/hkDemo.h>
#include <Demos/demos.h>

#include <Demos/DemoCommon/DemoFramework/hkDemo.h>
#include <Demos/DemoCommon/DemoFramework/hkDemoDatabase.h>
#include <Demos/DemoCommon/DemoFramework/hkDemoFramework.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Demos/DemoCommon/DemoFramework/hkDemoFilesystem.h>
#include <Common/Base/Algorithm/Sort/hkSort.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Base/System/Io/FileSystem/hkNativeFileSystem.h>
#include <Common/Base/Reflection/hkClassEnum.h>


hkDemoDatabase::hkDemoDatabase()
{
	rebuildDatabase();
}


hkDemoEntryRegister::hkDemoEntryRegister( DemoCreationFunction func, int type, const char* path,
										 hkDemoEntryRegister* entries, int numVariants, int sizeOfVariantStruct,
										 const char*const* variantNames, const char* help, const char*const* details, bool actuallyReg )
{
	for (int i = 0; i < numVariants;i++)
	{
		const char* variantName = *variantNames;
		if ( !variantName )
		{
			break;
		}
		const char* detail = *details;
		if ( !detail )
		{
			detail = "";
		}
		const char* h = (help) ? help : detail;

		new (&entries[i]) hkDemoEntryRegister( func, type, path, i, variantName, h, detail, actuallyReg );
		variantNames = hkAddByteOffsetConst( variantNames, sizeOfVariantStruct );
		details = hkAddByteOffsetConst( details, sizeOfVariantStruct );
	}
}

// for demos which create other entries
hkDemoEntryRegister::hkDemoEntryRegister(DemoCreationFunction func, int type, const char* path,
					DemoEntriesCreationFunction entryCreateFunc, 
					const char* help, const char* details, bool actuallyReg )
{
	new(this)hkDemoEntryRegister( func, type, path, -1, HK_NULL, help, details, actuallyReg ); 
	m_createEntryFunc = entryCreateFunc;
}


hkDemoEntryRegister::hkDemoEntryRegister( DemoCreationFunction func, int type, const char* path,
										 hkDemoEntryRegister* entries, int numEntries,
										 const hkClassEnum& cenum, 
										 const char* help, const char* details, bool actuallyReg )
{
	int nitems = cenum.getNumItems();
	HK_ASSERT(0x7d4ed233, numEntries >= nitems );
 	for( int i = 0; i < nitems; i++ )
	{
		const hkClassEnum::Item& item = cenum.getItem(i);
		new (&entries[i]) hkDemoEntryRegister( func, type, path, item.getValue(), item.getName(), help, details, actuallyReg );
	}
}

hkDemoEntryRegister::hkDemoEntryRegister(	DemoCreationFunction func, int typeflags, const char* path,
										 int variant, const char* variantName, 
										 const char* help, const char* details, bool actuallyReg )
										 :	m_func(func), m_createEntryFunc(HK_NULL), m_demoTypeFlags(typeflags), m_demoPath(path),
										 m_variantId( variant ), m_variantName( variantName ),
										 m_help(help), m_details(details)
{
	// If we don't auto reg, we assume that it will be in a static array of demos / tests etc
	if (actuallyReg )
	{
		m_next = hkDemoDatabase::s_demoList;
		hkDemoDatabase::s_demoList = this;
	}
	else
	{
		m_next = HK_NULL;
	}
}

void hkDemoEntryRegister::registerDemo()
{
	HK_ASSERT2( 0xf0323454, !m_next, "Demo already registered");
	m_next = hkDemoDatabase::s_demoList;
	hkDemoDatabase::s_demoList = this;
}

// to sort the demos array alphabetically and by variant id using hkSort
class demoEntryPathAndVariantIdLess
{
private:
	hkBool areVariants( const hkDemoEntry* a, const hkDemoEntry* b )
	{
		// check if they have a valid variant field
		if( (a->m_variantId >= 0) && (b->m_variantId >= 0) ) 
		{
			// if so, check if the base path is the same
			int lastSlashA = a->m_menuPath.lastIndexOf('/');
			hkBool sameBasePath = false;
			if( lastSlashA >  0 )
			{
				int lastSlashB = b->m_menuPath.lastIndexOf('/');
				sameBasePath = (lastSlashA == lastSlashB)? hkString::strNcmp(a->m_menuPath.cString(),b->m_menuPath.cString(),lastSlashA-1)==0 : false;
			}
			return sameBasePath;
		}
		else
		{
			return false;
		}
	}
	// true if the paths are the same and a->variantId < b->variantId
	HK_FORCE_INLINE hkBool ifSameLessVariantId( const hkDemoEntry* a, const hkDemoEntry* b )
	{
		// if they are variants of the same demo check the variant id
		return areVariants(a,b) && ( a->m_variantId < b->m_variantId );
	}

public:

	HK_FORCE_INLINE hkBool operator() ( const hkDemoEntry* a, const hkDemoEntry* b )
	{
		// order by variant id if they are variants of the same demo
		if (areVariants(a,b))
			return ( a->m_variantId < b->m_variantId );

		// alphabetically otherwise
		return ( a->m_menuPath < b->m_menuPath );
	}
};


hkString findCommonRootPath( const hkArray< hkDemoEntry* >& demos )
{
	hkString prefix = "";
	//
	// Find the common root, be careful if there is only one demo
	// special: if we see "*/Demo/" means <toplevel>/Demo
	{
		int i = 0;
		while( i < demos.getSize() && prefix.getLength() == 0 )
		{
			if(demos[i]->m_menuPath[0] != '*') // get a regular path.
			{
				int lastSlash = demos[i]->m_menuPath.lastIndexOf('/');
				if(lastSlash >= 0)
				{
					prefix = demos[i]->m_menuPath.substr(0, lastSlash + 1);
					prefix.makeLowerCase();
				}
			}
			++i;
		}
	}
	for(int i = 0; i < demos.getSize() && prefix.getLength() != 0; ++i)
	{
		if( demos[i]->m_menuPath[0] != '*')
		{
			hkString gp = demos[i]->m_menuPath.asLowerCase();
			while( !gp.beginsWith(prefix) )
			{
				// commonRoot is foo/bar/ we want the second last /
				int lastSlash = prefix.lastIndexOf('/', 0, prefix.getLength() - 1 );
				if(lastSlash >= 0)
				{
					// want the trailing / in the commonRoot
					prefix.setAsSubstr(0, lastSlash + 1);
				}
				else
				{
					prefix = "";
				}
			}
		} // if != *
	} // for

	return prefix;
}

void hkDemoDatabase::rebuildDatabase()
{
	m_demos.clear();

	//
	// Normalize and collect all demo register entries
	// into a temporary array
	//
	hkArray<hkDemoEntry*> demos;	
	hkDemoEntryRegister* e = s_demoList;
	while(e)
	{
		// Convert the file path to Havok path format
		hkArray<char> buffer;
		const char* tmp = hkNativeFileSystem::platformToHavokConvertPath( e->m_demoPath, buffer );
		hkString resourcePath( hkDemoFileSystem::removeDemoBasePath( tmp, buffer ) );

		int lastSlash  = resourcePath.lastIndexOf('/');		
		HK_ASSERT2(0x6e7879d4, lastSlash >= 0,  "probably a metrowerks build with no 'pragma fullpath_file on': " << resourcePath );

		// Copy as the menu path
		hkString menuPath( resourcePath.substr(0, lastSlash) );	// remove the demo filename for the menu

		// If we have a variant, append the variant name
		if( e->m_variantName )
		{
			menuPath += "/";
			menuPath += e->m_variantName;
		}

		// Store the entry
		hkDemoEntry* entry = new hkDemoEntry();
		entry->m_func = e->m_func;
		entry->m_demoTypeFlags = e->m_demoTypeFlags;
		entry->m_menuPath = menuPath;
		entry->m_demoPath = resourcePath;
		entry->m_variantId = e->m_variantId;
		entry->m_help = e->m_help;
		entry->m_details = e->m_details;

		if ( e->m_createEntryFunc )	// now we have a create other demos function
		{
			e->m_createEntryFunc(*entry, demos);
			delete entry;
		}
		else
		{
			demos.pushBack(entry);
		}
		e = e->m_next;
	}

	HK_ASSERT2(0x4467bbc3, demos.getSize() != 0, "No demos registered!");

	// find a common root for the demos, if any
	m_prefix = findCommonRootPath( demos );

	// strip the common root plus from the names
	int prefixLen = m_prefix.getLength();
	for(int i = 0; i < demos.getSize(); ++i)
	{
		// prune the prefix from each name 
		hkString& n = demos[i]->m_menuPath;
		if( n[0] == '*' ) // this comes from unit tests, */means "put me at the demos level"???
		{
			n.setAsSubstr(2);
		}
		else if( m_prefix.getLength() > 0 )
		{
			n.setAsSubstr( prefixLen, n.getLength() - prefixLen );
		}
	}

	//
	// Sort the array alphabetically and by ID in the same variant group
	//
	hkSort( demos.begin(), demos.getSize(), demoEntryPathAndVariantIdLess() );

	// Collect the menu order files into an array
	extern const char* DemoOrder[];
	// Build the member demos, sorting according to the menu order
	for (int i = 0; DemoOrder[i] != 0; i++)
	{
		// the demos are already sorted alphabetically
		int j=0;
		// look for the first matching path.. 
		while( j < demos.getSize() && !( demos[j]!=HK_NULL && demos[j]->m_menuPath.beginsWith(DemoOrder[i]) ) ) 
			++j;
		// ..and add it together with all the following matching ones
		while( j < demos.getSize() && demos[j]!=HK_NULL && (demos[j]->m_menuPath.beginsWith(DemoOrder[i])) ) 
		{
			m_demos.pushBack( *(demos[j]) );
			delete demos[j];
			demos[j] = HK_NULL;
			++j;
		}
	}

	// Store any leftover demos
	for (int j = 0; j < demos.getSize(); j++)
	{
		if(demos[j] != HK_NULL)
		{
			m_demos.pushBack( *(demos[j]) );
		}
		delete demos[j];
	}
	// cleanup
	demos.clear();

	/*
	//
	// Write the demo list to a file
	//
	hkOstream stream(HK_DEMO_LIST_FILENAME);
	if ( stream.isOk() )
	{
	stream << ";Havok '" << productCode << "' demo list:" << hkendl << hkendl;
	for (int j = 0; j < m_demos.getSize(); j++)
	{
	stream << m_demos[j].m_menuPath << hkendl;
	}
	}
	*/
}

// Case insensitive demo index finder
int hkDemoDatabase::findDemo(const char* cname) const
{
	// try full path first
	for(int i = 0; i < m_demos.getSize(); ++i)
	{
		if( hkString::strCasecmp( m_demos[i].m_menuPath.cString(), cname ) == 0 )
		{
			return i;
		}
	}
	// try to match the name only
	for(int i = 0; i < m_demos.getSize(); ++i)
	{
		int lastSlash = m_demos[i].m_menuPath.lastIndexOf('/');
		if( lastSlash >= 0 && hkString::strCasecmp( m_demos[i].m_menuPath.cString()+lastSlash+1, cname ) == 0 )
		{
			return i;
		}
	}
	// try the requested name as a demo prefix
	for(int i = 0; i < m_demos.getSize(); ++i)
	{
		int lastSlash = m_demos[i].m_menuPath.lastIndexOf('/');
		if( lastSlash >= 0 && hkString::strNcasecmp( m_demos[i].m_menuPath.cString()+lastSlash+1, cname, m_demos[i].m_menuPath.getLength()-lastSlash-1 ) == 0 )
		{
			return i;
		}
	}

	return -1;
}

// Create a demo by index
hkDemo* hkDemoDatabase::createDemo(int index, hkDemoEnvironment* env) const
{
	env->m_menuPath     = m_demos[index].m_menuPath;
	env->m_variantId	= m_demos[index].m_variantId;
	return (*m_demos[index].m_func)(env);
}

// Find and create a demo, otherwise return the error demo
hkDemo* hkDemoDatabase::createDemo(const char* name, hkDemoEnvironment* env) const
{
	int i = findDemo(name);
	if( i != -1 )
	{
		return createDemo(i, env);
	}

	return new ErrorDemo(env, name);
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#endif

hkDemoEntryRegister* hkDemoDatabase::s_demoList;
HK_SINGLETON_IMPLEMENTATION(hkDemoDatabase);

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
