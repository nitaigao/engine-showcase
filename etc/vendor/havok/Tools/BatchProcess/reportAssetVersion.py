#
# Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
# prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
# Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
#

#! /usr/bin/env python
import glob, os, sys, re

def getFilesByPattern( srcDir, filePattern, recurse ):
    """
    Get a list of all the files that match the specified file pattern in the specified directory.
    Also, search subdirectories if the 'recurse' parameter is set to 1.
    """
    fileList = []
    
    def getMatchingFiles(pattern, curDir, dirContents):
        relDir = curDir[len(srcDir)+len(os.path.sep):]
        matchingFiles = glob.glob1(os.path.join(srcDir,relDir), pattern)
        for matchingFile in matchingFiles:
            fileList.append(os.path.join(relDir,matchingFile))

    if recurse == 1:        
        os.path.walk(srcDir, getMatchingFiles, filePattern)        
    else:
        getMatchingFiles(filePattern, srcDir, [])

    return fileList


#===========================================
#
# Setup
#
#===========================================

if len(sys.argv) < 2:
    print ""
    print "You must specify a target directory as the first parameter to this script."
    print "Full usage is: reportAssetVersion.py srcDir [filePattern recurse]"
    print ""
    print "  srcDir                : The directory to get files to convert from. Can be relative or absolute.                                [no default]"
    print "  filePattern           : Files matching this pattern are converted. *,? and [] wildcards are supported.                          [default = *.hkx]"
    print "  recurse               : Whether to recurse or not. Should be 1 or 0.                                                            [default = 1]"
    sys.exit(0)

srcDir = os.path.abspath( sys.argv[1] )
if not os.path.isdir(srcDir):
    print '\nPlease specify a valid target directory, either absolute or relative to your current directory.\nYou specified %s' % srcDir
    sys.exit(0)

# Set up the optional parameters (using their default values).
orderedScriptArgs = ['filePattern', 'recurse']
scriptArgs = {
    'filePattern':'*.hkx',
    'recurse':1,
    }

# Overwrite any user-specified parameters with their new values.
argCount = 0
for sysArg in sys.argv[2:]:
    sysArgComponents = sysArg.split('=')
    if len( sysArgComponents ) == 2:
        scriptArgs[sysArgComponents[0]] = sysArgComponents[1]
    else:
        scriptArgs[orderedScriptArgs[argCount]] = sysArg
    argCount += 1

# Change the format of the non-string parameters.
scriptArgs['recurse'] = int(scriptArgs['recurse'])

# Get the list of assets to convert.
assetsToCheck = getFilesByPattern( srcDir, scriptArgs['filePattern'], scriptArgs['recurse'] )

for asset in assetsToCheck:
    data = open( os.path.join(srcDir, asset), "rb" ).read(1024)
    match = re.search( r'Havok-[0-9]+\.[0-9]+\.[0-9]+-..', data)
    if match:
        print '%-14s' % match.group(0), asset
    else:
        match2 = re.search( r'Havok-[0-9]+\.[0-9]+\.[0-9]', data)
        if match2:
            print '%-14s' % match2.group(0), asset
        else:
            # Get the binary revision.
            binaryRevision = int(ord(data[12]))
            if binaryRevision == 0:
                # Check other endian-ness
                binaryRevision = int(ord(data[15]))

            if binaryRevision in (1,2):
                print '%-14s' % ["Havok-3.0.0","Havok-3.1.0"][binaryRevision==1], asset
            else:
                print '%-14s' % '<unknown>', asset


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
