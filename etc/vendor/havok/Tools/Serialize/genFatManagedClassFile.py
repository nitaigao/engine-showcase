#
# Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
# prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
# Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
#

#! /usr/bin/env python

###

import os
import sys
import hkcToDom
import headerToDom
import domToManagedClass
import warnings
import mergeBind
import util

HK_DEFAULT_MANAGED_DEFINITION = "HK_HAVOK_MANAGED_CLASSES"
HK_CUSTOM_MANAGED_DEFINITION = "HK_CUSTOM_MANAGED_CLASSES"

def genManagedHeaderStart(managedHeaderContent, custom=False):
    managedDefinition = (custom and HK_CUSTOM_MANAGED_DEFINITION) or HK_DEFAULT_MANAGED_DEFINITION
    managedHeaderContent.append("#ifndef %s"%managedDefinition)
    managedHeaderContent.append("#define %s"%managedDefinition)

def genManagedHeaderEnd(managedHeaderContent, custom=False):
    managedDefinition = (custom and HK_CUSTOM_MANAGED_DEFINITION) or HK_DEFAULT_MANAGED_DEFINITION
    managedHeaderContent.append("#endif // %s"%managedDefinition)

def writeIfDifferent(text, fname, makebackup=True):
    try:
        if open(fname).read() == text:
            return
    except IOError:
        pass
    else:
        if makebackup:
            for i in range(0,100):
                try:
                    backup = "%s.%i" % (fname,i)
                    open( backup )
                except IOError:
                    os.rename(fname, backup)
                    break
    open(fname,"w").write(text)

def getName(filePath):
    return filePath.replace("\\","/").split("/")[-1]

def processDir(where, options):
    if options.output:
        fileNamePrefix = options.output
    else:
        fileNamePrefix = ""
    managedClassesHeaderFile = fileNamePrefix+"Classes.h"
    managedClassesImplFile = fileNamePrefix+"Classes.cpp"
    managedClassesHeader = domToManagedClass.FormatedSourceCode()
    managedClassesImpl = domToManagedClass.FormatedSourceCode()

    customLines = []
    genManagedHeaderStart( customLines, (options.manifest=="2") )
    domToManagedClass.genFileHeadlines( managedClassesHeader, customLines=customLines )
    if options.manifest=="2":
        ### custom manifest file, so include header for the Havok wrappers
        managedClassesHeader.append("#include <HavokAssembly/hkHavokManagedClasses.h>")
    
    domToManagedClass.genFileHeadlines(managedClassesImpl)
    managedClassesImpl.append("#include <HavokAssembly/%s>" % getName(managedClassesHeaderFile))
    
    nfile = 0
    def get_reflected_files(where, lst):
        for f in lst:
            full = os.path.join(where,f)
            if f.endswith(".h"):
                content = open(full).read()
                tkbms = util.extract_tkbms(content)
                valid_tkbms = lambda x : (not tkbms.has_key(x) or tkbms[x] != "NONE")
                if valid_tkbms("platform") and valid_tkbms("product") and util.hasReflectionDeclaration(content):
                    yield full
            elif f.endswith(".hkclass") and not os.path.exists(full.replace(".hkclass",".h")):
                yield full

    reflected_files = []
    excludeDirs = []
    if options.exclude:
        excludeDirs = options.exclude
    for dirname, subdirs, files in os.walk(where):
        files.sort()
        subdirs.sort()
        #print dirname
        if "CVS" in subdirs:
            subdirs.remove("CVS")
        if "UnitTest" in subdirs:
            subdirs.remove("UnitTest")
        if dirname in excludeDirs:
            del subdirs[:]
        else:
            reflected_files.extend( get_reflected_files(dirname,files) )

    fatDom = hkcToDom.Document(fileNamePrefix)
    fatDom.file = hkcToDom.File()
    fatDom.file.product = ""
    fatDom.file.platform = ".NET"
    fatDom.file.visibility = "PUBLIC"
    fatDom.file.includeheaders = ""

    def updatePath(filepath):
        updatedPath = filepath.replace("\\","/")
        if options.manifest=="2":
            updatedPath = "/".join( [ p for p in updatedPath.replace(where.replace("\\","/"),"").split("/") if p ] )
        return updatedPath

    def mergeDomDocs(domDocTo, domDocFrom):
        # merge
        domDocTo.file.mergeWith(domDocFrom.file)
        domDocTo.file.includeheaders += "#include <%s>\n" % updatePath(domDocFrom.localfilename)

    for reflected in reflected_files:
        nfile += 1
        if reflected.endswith(".h"):
            dom = headerToDom.headerToDom(reflected)
            if os.path.exists( reflected+"kclass" ):
                override = hkcToDom.hkcToDom( reflected+"kclass" )
                mergeBind.mergeBind(dom.file, override.file, dom.name)
        elif reflected.endswith(".hkclass"):
            dom = hkcToDom.hkcToDom( reflected )
        else:
            assert 0
        ### collect all doms info
        mergeDomDocs(fatDom, dom)

    if len(fatDom.file.includeheaders):
        headerfiles = [x for x in fatDom.file.includeheaders.split("\n") if not x.endswith(".inl>")]
        if options.manifest!="2":
            headerfiles.sort()
        cleanList = [""]
        for line in headerfiles:
            if line != cleanList[-1]:
                cleanList.append(line)
        headerfiles = "\n".join(cleanList[1:])
        managedClassesHeader.append("\n//\n// All reflected class headers\n//")
        managedClassesHeader.append("#pragma unmanaged")
        managedClassesHeader.append(headerfiles)
        managedClassesHeader.append("#pragma managed")

    managedClassDecl, managedClassDef = domToManagedClass.domToManagedClass(fatDom, collectAll=True)
    
    managedClassesHeader.append("\n".join(managedClassDecl.replace("\t","").split("/n")))
    managedClassesImpl.append("\n".join(managedClassDef.replace("\t","").split("/n")))

    genManagedHeaderEnd( managedClassesHeader, (options.manifest=="2") )
    
    updatedClassesHeaderContent = "\n".join(managedClassesHeader.lines)
    updatedClassesImplContent = "\n".join(managedClassesImpl.lines)
    if options.output:
        ### we open two files for output
        writeIfDifferent(updatedClassesHeaderContent, managedClassesHeaderFile, False)
        writeIfDifferent(updatedClassesImplContent, managedClassesImplFile, False)
    else:
        print >>sys.stdout, updatedClassesHeaderContent
        print >>sys.stdout, updatedClassesImplContent

    if not options.quiet:
        print >>sys.stderr, "DONE", where, nfile, "files processed"

USAGE = """%prog <WHERE>...

Recursively searches WHERE for class information.
Generates FILE.hkclass for each file found."""

def main(argv):
    import optparse
    parser = optparse.OptionParser(usage=USAGE)
    parser.add_option("-v", "--verbose", action="store_true", default=False, help="Verbose execution")
    parser.add_option("-q", "--quiet", action="store_true", default=False, help="Quiet execution")
    parser.add_option("-o", "--output", action="store", default=None, help="Output filename")
    parser.add_option("-e", "--exclude", action="append", default=None, help="Exclude dirs from processing")
    parser.add_option("--manifest", default="", help="Generate managed classes manfest")

    options, args = parser.parse_args(argv)
    if not options.manifest:
        parser.error("No manifest flag given.")

    for arg in args:
        if os.path.isdir(arg):
            processDir(arg, options)
        else:
            warnings.warn("'%s' is not a directory, skipping." % arg)
    if len(args) == 0:
        parser.error("No search path given.")

if __name__=="__main__":
    main(sys.argv[1:])


#
# Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20090216)
# 
# Confidential Information of Havok.  (C) Copyright 1999-2009
# Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
# Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
# rights, and intellectual property rights in the Havok software remain in
# Havok and/or its suppliers.
# 
# Use of this software for evaluation purposes is subject to and indicates
# acceptance of the End User licence Agreement for this product. A copy of
# the license is included with this software and is also available at www.havok.com/tryhavok.
# 
#
