#
# Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
# prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
# Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
#

#! /usr/bin/env python
import glob, os, sys

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


def createAssetCcs( assetList, srcDir, destDir, targetBinaryLayoutRulesList, assetNamingConvention, overwriteRule, useVersioning, assetCcExes ):
    """
    Create copies of the specified assets using the specified binary layout rules.
    """
    upToDateAssets = 0

    # Suppress crash dialogs which appear when processing bad assets.
    if sys.platform.startswith("win"):
        # Don't display the Windows GPF dialog if the invoked program dies.
        import ctypes
        SEM_NOGPFAULTERRORBOX = 0x0002 # From MSDN
        ctypes.windll.kernel32.SetErrorMode(SEM_NOGPFAULTERRORBOX);

    for asset in assetList:
        infile = os.path.join( srcDir, asset )
        for layoutRules in targetBinaryLayoutRulesList:
            if assetNamingConvention == 'havok_default':
                # Make the assumption that the asset file follows the naming convention: hkAssetName_L<current_layout_rules>.hkx,
                # and that the desired output filename is: hkAssetName_L<target_layout_rules>.hkx.
                # e.g. hkRig_L4101.hkx -> hkRig_L8011.hkx
                outfile = asset[:-8] + layoutRules + '.hkx'
            else:
                # Simply append _<target_layout_rules> to the filename, e.g. testRig.hkx -> testRig_8011.hkx
                splitName = os.path.splitext(asset)
                outfile = splitName[0] + '_' + layoutRules + splitName[1]
            outfile = os.path.join( destDir, outfile )

            # The AssetCc exes are expected in the AssetCc subdirectory.
            assetCcExecutable = os.path.join( os.path.dirname(sys.argv[0]), 'AssetCc', assetCcExes[useVersioning] )

            # If the target file already exists, decide whether to regenerate it based on the overwriteRule value.
            createFile = 1
            if os.path.isfile(outfile):
                if overwriteRule == 'if_newer':
                    # Update the asset if the source asset or assetCc executable are newer than it.
                    createFile = (os.path.getmtime(infile) > os.path.getmtime(outfile)) or (os.path.getmtime(assetCcExecutable) > os.path.getmtime(outfile))
                elif overwriteRule == 'yes':
                    createFile = 1
                else: # overwriteRule == 'no' or is invalid.
                    createFile = 0

            if createFile:
                print outfile
                if not os.path.isdir( os.path.dirname(outfile) ):
                    os.makedirs(os.path.dirname(outfile))
                
                assetCommand = '%s -r%s %s %s' % (assetCcExecutable, layoutRules, infile, outfile)
                if os.sys.platform == 'darwin':
                    assetCommand = "./%s" % (assetCommand)

                # Run the command
                errorCode = os.system(assetCommand)
                if errorCode:
                    print 'ERROR: AssetCc was unable to convert %s.' % infile

            else:
                upToDateAssets +=1

    if upToDateAssets > 0:
        print upToDateAssets, 'assets already exist and are up to date.'

    # Reset crash dialog suppression.
    if sys.platform.startswith("win"):
        ctypes.windll.kernel32.SetErrorMode(0);

    return


#===========================================
#
# Setup
#
#===========================================

if (sys.version_info[0] + 0.1 * sys.version_info[1]) < 2.5:
    print "Error: Python 2.5 or greater is needed to run this script."
    sys.exit(0)

if len(sys.argv) < 2:
    print ""
    print "You must specify a target directory as the first parameter to this script."
    print "Full usage is: generatePlatformSpecificAssets.py srcDir [filePattern binaryLayoutRulesList recurse destDir namingConvention overwrite useVersioning targetExes]"
    print ""
    print "  srcDir                : The directory to get files to convert from. Can be relative or absolute.                                [no default]"
    print "  filePattern           : Files matching this pattern are converted. *,? and [] wildcards are supported.                          [default = *.hkx]"
    print "  binaryLayoutRulesList : Platforms to convert to. Multiple platforms should be semicolon-delimited, e.g. 8011;4111;4011.         [default = 4101]"
    print "  recurse               : Whether to recurse or not. Should be 1 or 0.                                                            [default = 1]"
    print "  destDir               : Where to put generated files. The directory structure is mirrored if recurse is enabled.                [default = srcDir]"
    print "  namingConvention      : How to name created files. Only 'default' and 'havok_default' are currently supported.                  [default = havok_default]"
    print "  overwrite             : Whether to overwrite existing files. Can be 'yes', 'no' or 'if_newer'                                   [default = if_newer]"
    print "  useVersioning         : Whether to update generated files to the current version. Should be 1 or 0.                             [default = 1]"
    print "  targetExes            : The executables to use when generating assets. This neeed only be modified by non-win32 users.          [default = AssetCc1.exe,AssetCc2.exe]"
    print ""
    print "If named, optional parameters can be listed in any order, e.g. generatePlatformSpecificAssets.py C:/assets useVersioning=0 binaryLayoutRulesList=4011,8011"
    print "If unnamed, they should appear in the order listed above, e.g. generatePlatformSpecificAssets.py C:/assets *.hkx 4011,8011 0 C:/new_assets default"
    sys.exit(0)

srcDir = os.path.abspath( sys.argv[1] )
if not os.path.isdir(srcDir):
    print '\nPlease specify a valid target directory, either absolute or relative to your current directory.\nYou specified %s' % srcDir
    sys.exit(0)

# Set up the optional parameters (using their default values).
orderedScriptArgs = ['filePattern', 'binaryLayoutRulesList', 'recurse', 'destDir', 'namingConvention', 'overwrite', 'useVersioning', 'targetExes']
scriptArgs = {
    'filePattern':'*.hkx',
    'binaryLayoutRulesList':'4101',
    'recurse':1,
    'destDir':srcDir,
    'namingConvention':'havok_default',
    'overwrite':'if_newer',
    'useVersioning':1,
    'targetExes':'AssetCc1.exe,AssetCc2.exe',
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
scriptArgs['useVersioning'] = int(scriptArgs['useVersioning'])
scriptArgs['binaryLayoutRulesList'] = scriptArgs['binaryLayoutRulesList'].split(',')
scriptArgs['targetExes'] = scriptArgs['targetExes'].split(',')

# Platform check.
if os.sys.platform not in ('win32') and (scriptArgs['targetExes'] == ['AssetCc1.exe', 'AssetCc2.exe'] ):
    print "\n=================================================================================================="
    print "WARNING: You appear to be using a non-win32 PC. The AssetCc1 and AssetCc2 executables specified for"
    print "         use by this script (AssetCc1.exe and AssetCc2.exe) have been built for the win32 platform."
    print "         You'll need to do one of the following:"
    print "          - Specify different executables to use (via the targetExes parameter)."
    print "          - Replace the AssetCc1.exe and AssetCc2.exe executables with locally built versions."
    print "=================================================================================================="
    print "\nAttempting to continue..."

# Get the list of assets to convert.
assetsToCc = getFilesByPattern( srcDir, scriptArgs['filePattern'], scriptArgs['recurse'] )

if len(assetsToCc) > 0:
    # Convert the assets.
    createAssetCcs( assetsToCc, srcDir, scriptArgs['destDir'], scriptArgs['binaryLayoutRulesList'], scriptArgs['namingConvention'], scriptArgs['overwrite'], scriptArgs['useVersioning'], scriptArgs['targetExes'] )
else:
    print "No assets found..."

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
