#
# Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
# prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
# Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
#

#! /usr/bin/env python

###
### Recursively search a directory for class definitions to update
### Classes may be described in two ways:
###
###     1) "master" file is a c++ header. (default)
###        this is indicated by the header file containing the line
###        "HK_DECLARE_REFLECTION();" (usually part of a class declaration)
###        In this case we extract the class info from the c++ and
###        merge it into the xml description. Finally we generate
###        the hkClass instance from the xml description
###        c++ -> (extract + merge) -> xml -> (generate) hkClass instance
###
###     2) "master" file is an hkclass xml file. (rarely used)
###        In this case both the header file and cpp file are generated
###        from the xml description.
###        xml -> (generate) -> hkClass instance cpp
###        xml -> (generate) -> class definition header
###

import sys
import os
import re
import traceback
import domToHeader
import domToClass
import headerToDom
import hkcToDom
import bindingToXml
import mergeBind
import warnings
import util
warnings.filterwarnings(action="ignore", category=FutureWarning, module="optparse")

MAGIC_DESTINATION_MARK = re.compile(r'^\s*HK_REFLECTION_CLASSFILE_HEADER\s*\(\s*"([^"]*)"\s*\)', re.M)

option_verbose = 0
option_quiet = 0

def _error(e):
    print e
    traceback.print_exc()
    sys.exit(1)

def writeIfDifferent(text, fname, force_output=False):
    try:
        if open(fname).read() == text:
            if option_verbose:
                print " same ", fname
            if not force_output:
                return
    except IOError:
        if not option_quiet:
            print "CREATE", fname
    else:
        if not option_quiet:
            print "UPDATE", fname
    open(fname,"w").write(text)

class CvsIgnore:
    def __init__(self, folder):
        self.cvsignore = os.path.join(folder, ".cvsignore")
        try:
            self.origentries = open( self.cvsignore ).read().split()
        except IOError:
            self.origentries = []
        self.entries = self.origentries[:]
    def write(self):
        if self.entries != self.origentries:
            if len(self.entries):
                try:
                    open( self.cvsignore, "w" ).write( "\n".join(self.entries) + "\n" )
                except IOError, e:
                    print "Cant write to '%s'" % self.cvsignore
                    _error(e)
            else:
                try:
                    os.unlink( self.cvsignore )
                except IOError, e:
                    pass

def addToCvsignore(path):
    folder, fname = os.path.split(path)
    cvs = CvsIgnore(folder)
    if not fname in cvs.entries:
        cvs.entries.append( fname )
    cvs.write()

def removeFromCvsignore(path):
    folder, fname = os.path.split(path)
    cvs = CvsIgnore(folder)
    try:
        cvs.entries.remove( fname )
    except ValueError:
        pass
    cvs.write()

def genPathToFile(filePath, destSpecifier):
    if not destSpecifier:
        return filePath
    relativePath = os.path.normpath(destSpecifier.group(1))
    folder, fname = os.path.split(filePath)
    return os.path.normpath(os.path.join(folder, relativePath, fname))

def headerToClass(headerfile, options):
    if options.verbose:
        print "Processing '%s'" % headerfile
    classfile = headerfile.replace(".h", ".hkclass")
    master = headerToDom.headerToDom(headerfile)
    if options.verbose:
        print "Found:", ", ".join( [c.name for c in master.file._class] )

    if os.path.exists(classfile):
        edited = hkcToDom.hkcToDom(classfile)
        rejects = mergeBind.mergeBind(master.file, edited.file, master.name)
        if len(rejects):
            rejfile = "%s.rej" % classfile
            open(rejfile,"a").write( "\n".join(rejects)+"\n" )
            print "%s:1:Conflicts found" % headerfile

    pch_file = options.pch_file
    cppfile = master.file.destinationFilename(master.origfilename, "Class%s.cpp" % options.version_suffix )
    if master.file.overridedestination:
        pch_file = hkcToDom.Document(cppfile).pchfile
    try:
        cpptext = domToClass.domToClass(master, collectAll=False, pchfile=pch_file)
    except:
        print "Error in: " + headerfile
        raise
    try:
        writeIfDifferent( cpptext, cppfile, options.force_output )
    except IOError:
        print "Error in: %(headerfile)s\n\nDestination class file path is wrong:\n%(cppfile)s.\n" % locals()
        raise
    if options.dump_xml:
        xmltext = bindingToXml.bindingToXml("document",master)
        try:
            xmlfile = genPathToFile(headerfile.replace(".h", "Class%s.xml" % options.version_suffix ), destSpecifier)
            writeIfDifferent( xmltext, xmlfile, options.force_output )
        except IOError:
            print "Error in: %(headerfile)s\n\nDestination class file path is wrong:\n%(xmlfile)s.\n" % locals()
            raise
    if options.cvsignore:
        removeFromCvsignore(headerfile)
        addToCvsignore(cppfile)
        addToCvsignore(classfile)

def classToHeader(classfile, options):
    if options.verbose:
        print "Processing '%s'" % classfile
    dom = hkcToDom.hkcToDom(classfile)
    headerfile, cppfile = classfile.replace(".hkclass",".h"), classfile.replace(".hkclass","Class.cpp")
    try:
        if util.hasReflectionDeclaration(open(headerfile).read()):
            raise RuntimeError("'%s' contains HK_DECLARE_REFLECTION. Not overwriting" % headerfile)
    except IOError:
        pass
    writeIfDifferent( domToHeader.domToHeader(dom), headerfile )
    writeIfDifferent( domToClass.domToClass(dom), cppfile )
    if options.cvsignore:
        removeFromCvsignore(classfile)
        addToCvsignore(headerfile)
        addToCvsignore(cppfile)

def processDir(where, options):
    ismeta = lambda f : f.endswith("h") and util.hasReflectionDeclaration(open(os.path.join(dirname,f)).read())
    nfile = 0
    for dirname, subdirs, files in os.walk(where):
        headers = [ os.path.join(dirname,f) for f in files if ismeta(f) ]
        hkclasses = [ os.path.join(dirname,f) for f in files if f.endswith(".hkclass") ]
        generated = {}

        for header in headers:
            nfile += 1
            classfile = header.replace(".h",".hkclass")
            if classfile in hkclasses:
                hkclasses.remove(classfile)
            headerToClass(header, options)
            generated[header.replace(".h","Class.cpp")] = 1
        for hkcname in hkclasses:
            nfile += 1  
            classToHeader(hkcname, options)
            generated[hkcname.replace(".hkclass","Class.cpp")] = 1
        for f in [ os.path.join(dirname,f) for f in files if (f.endswith("Class.cpp") and f != "hkClass.cpp") ]:
            if generated.get(f,0) == 0 and not MAGIC_DESTINATION_MARK.search(open(f).read()): # rename stale files to numbered backup
                for i in range(0,100):
                    try:
                        backup = "%s.%i" % (f,i)
                        open( backup )
                    except IOError:
                        print "STALE %s -> %s" % (f,backup)
                        os.rename(f, backup)
                        break
        try:
            subdirs.remove("CVS")
        except ValueError:
            pass
        if options.local_only:
            break

    if not options.quiet:
        print "DONE", where, nfile, "files processed"

def processFile(fname, options):
    try:
        if fname.endswith(".hkclass"):
            classToHeader(fname, options)
        elif fname.endswith(".h"):
            headerToClass(fname, options)
        else:
            warnings.warn("Don't know how to process '%s', skipping" % fname)
    
    except hkcToDom.HkcParseError, msg:
        print "Error in %s:\n" % fname 
        print msg

USAGE = """%prog <WHERE>...

Recursively searches WHERE for class information.
Generates FILE.hkclass for each file found."""

def main():
    import optparse
    parser = optparse.OptionParser(usage=USAGE)
    parser.add_option("-v", "--verbose", action="store_true", default=False, help="Verbose execution")
    parser.add_option("-q", "--quiet", action="store_true", default=False, help="Quiet execution")
    parser.add_option("-c", "--cvsignore", action="store_true", default=True, help="Update .cvsignore")
    parser.add_option("-l", "--local-only", action="store_true", default=False, help="Don't recurse directories")
    parser.add_option("-x", "--dump-xml", action="store_true", default=False, help="Dump XML")
    parser.add_option("-f", "--force-output", action="store_true", default=False, help="Write output file even if not changed")
    parser.add_option("--version-suffix", default="", help="Enable versioning mode")
    parser.add_option("--pch-file", action="store", default="", help="Override pch file")
    options, args = parser.parse_args()
    if options.version_suffix:
        options.cvsignore = False
    for arg in args:
        if os.path.isdir(arg):
            processDir(arg, options)
        elif os.path.isfile(arg):
            processFile(arg, options)
        else:
            warnings.warn("'%s' is not a file nor directory, skipping." % arg)
    if len(args) == 0:
        parser.error("No search path given.")

if __name__=="__main__":
    main()


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
