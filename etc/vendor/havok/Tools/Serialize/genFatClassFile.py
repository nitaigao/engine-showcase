#
# Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
# prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
# Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
#

#! /usr/bin/env python

###

import os
import re
import sys
import hkcToDom
import headerToDom
import domToClass
import warnings
import mergeBind
import util

_tkbms = """// TKBMS v1.0 -----------------------------------------------------
//
// PLATFORM   : %s
// PRODUCT    : %s
// VISIBILITY : %s
//
// ------------------------------------------------------TKBMS v1.0
#include <Common/Compat/hkCompat.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>

#include <Common/Compat/hkHavokAllClasses.h>

namespace %s
{
\tconst char VersionString[] = \"Havok-%s\";
\tconst int ClassVersion = %i;
"""

def processDir(where, options):
    nfile = 0
    cpplist = []
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
    for dirname, subdirs, files in os.walk(where):
        files.sort()
        subdirs.sort()
        reflected_files.extend( get_reflected_files(dirname,files) )

    manifest = hkcToDom.Document("Manifest")
    manifest.file = hkcToDom.File()
    manifest.file.visibility = "PUBLIC"
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
        manifest.file.mergeWith(dom.file)
    cpplist.append( domToClass.domToClass(manifest, collectAll=True))
            
    if options.output:
        outfile = open(options.output,"w")
    else:
        outfile = None
    cpptxt = "\n".join(cpplist)
    symbols = [ s for s in re.findall(r"^hkClass\s+([^\(]+)\s*\($(?m)", cpptxt) ]
    symbols.sort()
    cpptxt = re.sub("\s*//.*(?m)","", cpptxt)
    cpptxt = re.sub("^extern\s+hkClass\s+.*(?m)","", cpptxt)
    cpptxt = re.sub("\n\s*\n","\n", cpptxt)
    namespace = "hkHavok%sClasses" % options.version_suffix.replace("_","")
    versionNumString = ".".join( [ s for s in options.version_suffix.replace("_","")[:3] ] )
    versionStage = options.version_suffix.replace("_","")[3:]
    if versionStage is not "":
        versionNumString = "-".join( [versionNumString, versionStage] )
    listname = "Classes"
    print >>outfile, _tkbms % ("ALL","ALL","PUBLIC",namespace,versionNumString,int(options.class_version))
    print >>outfile, "\n".join( ["\textern hkClass %s;"%s.replace("(",";") for s in symbols])
    print >>outfile, cpptxt.replace("\n","\n\t")[:-1]
    print >>outfile, "\tstatic hkClass* const %s[] =\n\t{\n%s\n\t\tHK_NULL\n\t}; " % ( listname,
        "\n".join(["\t\t&%s,"%s for s in symbols] ) )
    print >>outfile, "\tconst hkStaticClassNameRegistry hkHavokDefaultClassRegistry\n\t(\n\t\tClasses,\n\t\tClassVersion,\n\t\tVersionString\n\t);"
    print >>outfile, "".join( ["\n} // namespace ", namespace, "\n" ] )
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
    parser.add_option("--version-suffix", default="", help="Enable versioning mode")
    parser.add_option("--class-version", default="-1", help="Class version number")

    options, args = parser.parse_args(argv)
    if not options.version_suffix:
        parser.error("No version suffix given.")

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
