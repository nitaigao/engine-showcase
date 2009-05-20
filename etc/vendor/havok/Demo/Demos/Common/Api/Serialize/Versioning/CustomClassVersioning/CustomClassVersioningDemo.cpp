/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Common/Api/Serialize/Versioning/CustomClassVersioning/CustomClassVersioningDemo.h>

// Serialize includes
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Base/Config/hkConfigVersion.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkClassMemberAccessor.h>
#include <Common/Base/System/Io/FileSystem/hkNativeFileSystem.h>
#include <Common/Serialize/Util/hkStructureLayout.h>
#include <Common/Serialize/Util/hkPointerMultiMap.h>
#include <Common/Base/Reflection/Registry/hkClassNameRegistry.h>
#include <Common/Serialize/Util/hkStaticClassNameRegistry.h>
#include <Common/Serialize/Version/hkVersionUtil.h>
#include <Demos/Common/Api/Serialize/Versioning/CustomClassVersioning/CustomClass.h>
#include <Demos/Common/Api/Serialize/Versioning/CustomClassVersioning/Havok510r1Classes.h>
#include <Common/Serialize/Util/hkVersionCheckingUtils.h>
#include <Common/Compat/hkHavokAllClassUpdates.h>

#define REGISTER_RUNTIME_CUSTOM_CLASSES_MANUALLY

//
// This demo register custom classes manually with corresponding class name registry.
// The following lines show how to create list of runtime custom classes and define custom
// static class name registry.
// NOTE: Normally you should register your reflected classes using HK_CLASSES_FILE macro
// and hkBuiltinTypeRegistry.cxx file.
// #define HK_CLASSES_FILE <Common/Serialize/Classlist/CustomClasses.h>
// #include <Common/Serialize/Util/hkBuiltinTypeRegistry.cxx>
//

// Define list of runtime custom classes
	extern const hkClass CustomClassLocalDataClass;
	extern const hkClass CustomClassClass;
	extern const hkClass CustomClass2Class;

	static const hkClass* classes[] =
	{
		&CustomClassLocalDataClass,
		&CustomClassClass,
		&CustomClass2Class,
		HK_NULL /* Zero-terminated array */
	};
// Define static class registry with list of the classes.
// This static class name registry will be used to register our classes with default built-in
// class name registry, so our runtime classes are visible to versioning system.
	static const hkStaticClassNameRegistry s_staticClassRegistry(classes, -1, HAVOK_SDK_VERSION_STRING);

//
// The CustomClass class was changed between 510r1 and 550b1 by removing old member "m_minNum" and by
// adding new member "m_status".
// The CustomClass2 was removed.
// 
// The following tables must be specified to describe the changes in CustomClass and CustomClass2 classes.
//
// 510r1 to 550b1
//
// Define static table of class renames between 510r1 and 550b1.
	static hkVersionRegistry::ClassRename renames510r1To550b1[] =
	{
		/* no class renames between 510r1 and 550b1 */
		{ HK_NULL, HK_NULL } // Must be zero-terminated.
	};
	static hkVersionRegistry::ClassAction actions510r1To550b1[] =
	{
		{ hkUint32(hkVersionRegistry::AUTO_SIGNATURE), hkUint32(hkVersionRegistry::AUTO_SIGNATURE), hkVersionRegistry::VERSION_COPY, "CustomClass", HK_NULL }, // CustomClass update action
		{ hkUint32(hkVersionRegistry::AUTO_SIGNATURE), hkUint32(hkVersionRegistry::AUTO_SIGNATURE), hkVersionRegistry::VERSION_REMOVED, "CustomClass2", HK_NULL }, // CustomClass2 update action
		{ HK_NULL, HK_NULL, HK_NULL, HK_NULL, HK_NULL } // Must be zero-terminated.
	};
//
// Define update description for custom classes changes between 510r1 and 550b1.
// We need class renames, update actions and static class name registry of changed custom classes for 550b1 version.
// Current version of application is newer than 550b1, and no custom classes (in this demo) changed between 550b1
// and current - we can pass HK_NULL indicating that there is no class changes since 550b1.
// (STATIC_CLASS_NAME_REGISTRY_FOR_550b1 defined as HK_NULL).
//
	#define STATIC_CLASS_NAME_REGISTRY_FOR_550b1 HK_NULL
	hkVersionRegistry::UpdateDescription update510r1To550b1(renames510r1To550b1, actions510r1To550b1, STATIC_CLASS_NAME_REGISTRY_FOR_550b1);

CustomClassVersioningDemo::CustomClassVersioningDemo( hkDemoEnvironment* env) 
	: hkDefaultDemo(env)
{
	static bool hasRegistered = false;
	hkVersionRegistry& verReg = hkVersionRegistry::getInstance();

	if( !hasRegistered )
	{
		//
		// Register runtime classes manually using static class name registry defined above.
		// Normally you should do this using HK_CLASSES_FILE macro and hkBuiltinTypeRegistry.cxx file.
		//
		hkVersionRegistry::getInstance().registerStaticClassRegistry(s_staticClassRegistry);
		//
		// Register old classes for 510r1
		//
		verReg.registerStaticClassRegistry(hkHavok510r1Classes::CustomStaticRegistry);
		//
		// Register update description for the class changes between 510r1 and 550b1.
		//
		verReg.registerUpdateDescription(update510r1To550b1, hkHavok510r1Classes::VersionString, hkHavok550b1Classes::VersionString);

		hasRegistered = true;
	}

	//
	// Run version checking utility function to validate registered update descriptions (renames and update actions).
	//
	const hkClassNameRegistry* classreg510r1 = verReg.getClassNameRegistry(hkHavok510r1Classes::VersionString);
	const hkClassNameRegistry* classreg550b1 = verReg.getClassNameRegistry(hkHavok550b1Classes::VersionString);
	if( classreg510r1 && classreg550b1 )
	{
		hkVersionCheckingUtils::verifyUpdateDescription( hkcout, *classreg510r1, *classreg550b1, 
			hkCompat_hk510r1_hk550b1::hkVersionUpdateDescription, hkVersionCheckingUtils::NONE );
	}
}

hkDemo::Result CustomClassVersioningDemo::stepDemo()
{
	return hkDefaultDemo::stepDemo();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows how to register custom class for versioning.";

HK_DECLARE_DEMO(CustomClassVersioningDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_SERIALIZE, "Shows how to register custom class for versioning", helpString);

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
