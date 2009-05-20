#
# Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
# prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
# Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
#

# This script (processAssets.py) batch processes all the Max/Maya/XSI assets in the directory specified by the user,
# using an optional HKO file if desired

# To run this script install Python 2.3 (http://www.python.org), open the command prompt,
# navigate to the directory containing this script and type "processAssets.py"

# A user interface will open asking you to specify:
# - The directory containing the assets you wish to process (assets in subdirectories are also processed).
# - A .hko options file to use when processing assets. If this is not specified the options saved with the assets
#    will be used. If an asset has no saved options then the same options that were used last time will be used again.
#    Finally, if an override or upgrade file is present (see the Havok Manual for more details) its options take precedence over all others
# - Wheter you want any changes to the assets to be saved (eg. by an upgrade HKO file)
# - Paths to each of the Max/Maya/XSI application directories. These may be left blank if the modeler path(s) are included the PATH environment var.

# If you wish to automate this script you can bypass the user interface by specifying the following arguments:
def usage():
    print "Usage: processAssets.py <assetPath> [--hko path] [-s] [--3dsmax path] [--maya path] [--xsi path]"
    sys.exit(0)


import sys, getopt
import os
import Tkinter as tk
from tkCommonDialog import Dialog


# Globals
scriptPath = sys.argv[0]

assetPath = ""
havokResourcesDir = os.path.abspath( os.path.join("..","..","Demo","Demos","Resources") )
if os.path.isdir( havokResourcesDir ):
    assetPath = havokResourcesDir.replace('\\','/')
else:
    assetPath = os.path.dirname( sys.argv[0] ).replace('\\','/')

hkoPath = ""
saveAssets = "0";

maxPath = "C:/3dsmax6"
mayaPath = ""
xsiPath = "C:/Softimage/XSI_5.0/Application/bin"


#=============================================
#
# This is the template for maxBatchScript.ms
#
#=============================================

maxScript = """

-- This script processes all 3ds Max files in the chosen directory and its subdirectories.

-- A list of all the directories that may contain 3ds Max files.
global directoryArray = #("ASSET_DIR")

-- Search the specified root directory and its subdirectories for 3ds Max files.
fn getAllDirectoriesRecursive root = 
( 
	-- Get a list of immediate directories (i.e. those in the current directory).
	immDirectories = getDirectories ( root + "/*" )
	
	-- Create an empty list to fill with immediate directories with the trailing '\\' removed.
	subStringArray = #()
	
	-- If there are any directories in our list...
	if( immDirectories.count != 0 ) then
	(
		-- Remove any trailing '\\' characters and add the directory to the list.
		for d in immDirectories do
		(
			dirNameLength = d.count
			directorySubString = substring d 1 ( dirNameLength-1 )
			append subStringArray directorySubString
		)
	
		-- Add the list of directories to the overall list.
		join directoryArray subStringArray
	
		-- Look for more directories in all the directories that were just found.
		for d in subStringArray do
		(
			getAllDirectoriesRecursive( d )
		)
	)
)

-- Create a log file to show which files were processed.
outlog = createFile "maxBatchScriptOutput.txt"
format "\\nThe following .max files were processed:" to:outlog
format "\\n----------------------------------------\\n" to:outlog

-- Get a list of all the directories to look for 3ds Max files in.
print "Generating directory list ...\\n"
getAllDirectoriesRecursive "ASSET_DIR"

-- Process any 3ds Max files found in the directories.
print "Searching for Max files in directory list ...\\n"

-- Check each directory for 3ds Max files.
for d in directoryArray do
(
	-- Look for files that end in '.max'.
	for fileName in getFiles ( d + "/*.max" ) do
	(
		name = getFileNameFile fileName
		format "Found file '%' in directory '%'\\n" name d to:outlog
	)
	
	-- Process files that end in '.max'.
	for fileName in getFiles ( d + "/*.max" ) do
	(
		-- filename without extension
		name = getFileNameFile fileName

		-- Load the file.
		loadMaxFile fileName useFileUnits:true

		-- Set HKO file if required
		optionsFile = "OPTIONS_FILE"
		if( optionsFile.count > 0 ) then
		(
			hkSceneExportUtility.useOptionsFile = TRUE
			hkSceneExportUtility.optionsFile = optionsFile
		)

		-- Export the file and display the results (true == batchMode)
		if ( ( hkSceneExportUtility.exportScene true ) == TRUE ) then
		(
			format "Processed file '%' in directory '%'\\n" fileName d to:outlog

			-- Save any changes to the asset file (caused by specifying a separate options file of using a hkupgrade.hko file).
			if ( SAVE_CHANGES != 0 ) then
			(
				saveMaxFile filename
			)
		)
		else
		(
			format "Failed to process file '%' in directory '%'\\n" fileName d to:outlog
		)
	)
)

-- Processing complete ... close 3ds Max.
quitMax() #noprompt

"""

#===============================================
#
# This is the template for mayaBatchScript.mel
#
#===============================================

melScript = """

// This script processes all Maya files in the chosen directory and its subdirectories.

// Search the specifed directory for Maya files and other directories to search.
// Also allows for the path to be a single file as well as a path.
global proc processAllFilesRecursive( string $path, int $fileID )
{
	// if the path is a directory, look through it
	if( `filetest -d $path` )
	{
		// make sure $path ends with a slash
		if( !endsWith($path, "/") )
		{
			$path = $path + "/";
		}
		
		// get a list of all the files in the path
		string $fileList[] = `getFileList -folder $path`;
		
		// recurse on each file
		for( $file in $fileList )
		{
			processAllFilesRecursive( $path + $file, $fileID );
		}
	}
	else
	{
		// process the file only if it has a valid maya extension
		string $ext = `fileExtension $path`;
		if (( "mb" == $ext ) || ( "ma" == $ext )) 
		{
			processMayaFile( basename($path, ""), dirname($path)+"/", $fileID );
		}
	}
	
	return;
};

// Get the export options saved with the scene.
global proc string createExportCommand()
{
	// Get the options node
	string $node = hkProcExporter_getOptionsNode(true);
	
	// Build the export command. The -b flag enables batch mode, so that the export is performed silently.
	string $exportCommand = "hkCmdExportScene -b";
	
	// Scene data
	$exportCommand += " -ms "	+ `getAttr ($node+".ExportMeshes")`;
	$exportCommand += " -mt "	+ `getAttr ($node+".ExportMaterials")`;
	$exportCommand += " -att "	+ `getAttr ($node+".ExportAttributes")`;
	$exportCommand += " -ann "	+ `getAttr ($node+".ExportAnnotations")`;
	$exportCommand += " -l "	+ `getAttr ($node+".ExportLights")`;
	$exportCommand += " -c "	+ `getAttr ($node+".ExportCameras")`;
	if( `getAttr ($node+".VisibleOnly")` )
	{
		$exportCommand += " -v ";
	}
	if( `getAttr ($node+".SelectedOnly")` )
	{
		$exportCommand += " -s ";
	}
	
	// Range
	int $rangeType = `getAttr ($node+".RangeType")`;
	switch( $rangeType )
	{
	case 0:	// current time
		float $currentTime = `currentTime -q`;
		$exportCommand += " -t1 " + $currentTime;
		$exportCommand += " -t2 " + $currentTime;
		break;
	case 1:	// current range
		$exportCommand += " -t1 " + `playbackOptions -q -minTime`;
		$exportCommand += " -t2 " + `playbackOptions -q -maxTime`;
		break;
	case 2:	// specific range
		$exportCommand += " -t1 " + `getAttr ($node+".StartTime")`;
		$exportCommand += " -t2 " + `getAttr ($node+".EndTime")`;
		break;
	default:
		error( ($node+".RangeType is invalid") );
		break;
	}

	// Options file, if required
	string $options_file = "OPTIONS_FILE";
	if ( $options_file == "" )
	{
		// Filter filename
		if( `getAttr ($node+".UseOptionsFile")` )
		{
			$exportCommand += " -o \\\"" + `getAttr ($node+".OptionsFile")` + "\\\"";
		}
	}
	else
	{
		$exportCommand += " -o \\\"" + $options_file + "\\\"";
	};

	// Advanced
	$exportCommand += " -rp " + `getAttr ($node+".UseRotatePivot")`;
	$exportCommand += " -op " + `getAttr ($node+".UseOldPhysics")`;
	
	// Run the command
	return $exportCommand;
}

// Process the specified Maya file.
global proc processMayaFile( string $filename, string $fullpath, int $fileID )
{
	// Whether to save changes or not:
	int $saveChanges = SAVE_CHANGES;
	
	// Open the file.
	file -f -prompt false -o $filename;

	// Convert any old nodes in the scene to the current format.
	hkProcExporter_convertLegacyOptionNodes();

	// The export command (this command uses the export options saved with the scene).
	string $cmd = createExportCommand();

	// Export the file.
	int $result = `eval $cmd`;

	// Display the results.
	if ( $result == false )
	{
		string $outputString = ( "Failed to process file: " + $fullpath + $filename + "\\n" );
		fwrite $fileID $outputString;
		print $outputString;
	}
	else
	{
		string $outputString = ( "Processed file: " + $fullpath + $filename + "\\n" );
		fwrite $fileID $outputString;
		print $outputString;
		
		// Save any changes to the asset file (caused by specifying a separate options file of using a hkupgrade.hko file).
		if ( $saveChanges != 0 )
		{
			file -f -save;
		}
	}
}

// Get ready for processing.
global proc hkxBatchProcess( string $rootDirectory )
{
	// Load the hkExport_Utilities script so that old option nodes can be upgraded
	// to the current format (using hkProcExporter_convertOldOptionNodes()),
	// and so that the options node can be found (using hkProcExporter_getOptionsNode()).
	source "hctExporter_Utilities";

	print( "\\nParsing directory tree for Maya files...\\n" );

	// Open a log file to write out which files were processed.
	int $fileID = `fopen "mayaScriptOutput.txt" "w"`;
	
	// Begin recursively checking directories starting with $rootDirectory.
	processAllFilesRecursive( $rootDirectory, $fileID );

	// Close the log file.
	fclose $fileID;	

	// Done.
	print( "Processing complete.\\n" );
}

// Begin by force loading havok plugins silently
loadPlugin -qt "hctMayaPhysics.mll";
loadPlugin -qt "hctMayaSceneExport.mll";

// Process the specified directory.
hkxBatchProcess "ASSET_DIR/";

// Shut down Maya.
quit -force;

"""

#===============================================
#
# This is the template for xsiBatchScript.vbs
#
#===============================================

xsiScript = """

' This script processes all XSI files in the chosen directory and its subdirectories

Sub processFile( file, log )

	' open the scene
	OpenScene file.Path, false

    ' override HKO file if required
    Dim batchHkoFile
    batchHkoFile = "OPTIONS_FILE"
    If Len(batchHkoFile) > 0 Then
            Dim optionsNode
            Set optionsNode = hkCmdGetExportOptions()
            optionsNode.Parameters( "useHko" ).Value = true
            optionsNode.Parameters( "hkoFile" ).Value = batchHkoFile            
    End If

    ' export in batch mode
    If hkCmdExportScene( true ) Then
            log.WriteLine( "Processed: " & file.Path )
            
            ' May need to save any changes
            If SAVE_CHANGES Then
                SaveScene
            End If
        Else
            log.WriteLine( "Failed to process: " & file.Path )
        End If

End Sub

Sub processFolder( folder, log )

    ' process any XSI scene files
    Dim files: Set Files = folder.Files
    Dim file
    For Each file In files
            If StrComp( file.Type, "XSI Scene file", 1 ) = 0 Then
                processFile file, log
            End If
            Next

    ' process any subfolders
    Dim subFolders: Set subFolders = folder.SubFolders
    Dim subFolder
    For Each subFolder In subFolders
            processFolder subFolder, log
            Next

End Sub

Sub hkxBatchProcess( rootPath )
	
	LogMessage( "Parsing directory tree for XSI files" )
	
	Dim fso, log
	Set fso = CreateObject( "Scripting.FileSystemObject" )
	Set log = fso.CreateTextFile( "xsiScriptOutput.txt" )
	
	processFolder fso.GetFolder( rootPath ), log
	
	LogMessage( "Processing complete" )
	
End Sub

' process the contents of the specified directory
hkxBatchProcess "ASSET_DIR"

' shut down XSI
Quit

"""

#===========================================
#
# Create and execute the scripts
#
#===========================================

def doit():
    
    global scriptPath, assetPath, hkoPath, saveAssets, maxPath, mayaPath, xsiPath
    
    # Make sure path formats are correct
    scriptPath = os.path.abspath( os.path.dirname( scriptPath ) ).replace('\\','/')
    assetPath = os.path.abspath( assetPath ).replace('\\','/')
    hkoPath = hkoPath.replace('\\','/')
    
    # Write out the scripts with the chosen options.
    maxScriptPath = "%s/maxBatchScript.ms" % scriptPath
    melScriptPath = "%s/mayaBatchScript.mel" % scriptPath
    xsiScriptPath = "%s/xsiBatchScript.vbs" % scriptPath
    open(maxScriptPath,'w').write( maxScript.replace('ASSET_DIR', assetPath).replace('OPTIONS_FILE', hkoPath ).replace('SAVE_CHANGES', saveAssets) )
    open(melScriptPath,'w').write( melScript.replace('ASSET_DIR', assetPath).replace('OPTIONS_FILE', hkoPath ).replace('SAVE_CHANGES', saveAssets) )
    open(xsiScriptPath,'w').write( xsiScript.replace('ASSET_DIR', assetPath).replace('OPTIONS_FILE', hkoPath ).replace('SAVE_CHANGES', saveAssets) )
    
    # Execute the maxBatchScript.ms script.
    maxCmd = "3dsmax.exe"
    if len(maxPath)>0: maxCmd = "%s/%s" % (maxPath, maxCmd)
    maxBatchCommand = "\"%s\" -q -silent -mip -U MAXScript %s" % (maxCmd, maxScriptPath)
    print "---\nExecuting:%s" % maxBatchCommand
    os.system(maxBatchCommand)
    
    # Execute the mayaBatchScript.mel script.
    mayaCmd = "mayabatch.exe"
    if len(mayaPath)>0: mayaCmd = "%s/%s" % (mayaPath, mayaCmd)
    mayaBatchCommand = "\"%s\" -script %s" % (mayaCmd, melScriptPath)
    print "---\nExecuting:%s" % mayaBatchCommand
    os.system(mayaBatchCommand)
    
    # Execute the xsiBatchScript.vbs script.
    xsiCmd = "xsibatch.bat"
    if len(xsiPath)>0: xsiCmd = "%s/%s" % (xsiPath, xsiCmd)
    xsiBatchCommand = "\"%s\" -script %s" % (xsiCmd, xsiScriptPath)
    print "---\nExecuting:%s" % xsiBatchCommand
    os.system(xsiBatchCommand)
    
    # Done - exit the script.
    sys.exit(0)


#===========================================
#
# User Interface functions
#
#===========================================

# Create a row of buttons.
class ButtonBar(tk.Frame):
    def __init__(self, master=None, buttons=[]):
        tk.Frame.__init__(self, master, {})
        for b in buttons:
            tk.Button(self, text=b[0], command=b[1]).pack(side=tk.LEFT)

# Create a dialog which allows the user to choose a directory.
def askOpenDirectory(**options):
	"return directory chosen or None if cancelled"
	class OpenDirectory(Dialog):
		command = "tk_chooseDirectory"
	chosen = apply(OpenDirectory, (), options).show()
	return (chosen != "") and chosen or None

# Create a dialog which allows the user to choose a file.
def askOpenFile(**options):
	"return file chosen or None if cancelled"
	class OpenFile(Dialog):
		command = "tk_getOpenFile"
	chosen = apply(OpenFile, (), options).show()
	return (chosen != "") and chosen or None

# Create a gui frame for text, a text box and a button.
class _EntryBase(tk.Frame):
	def __init__(self, master=None, textvariable=None):
		tk.Frame.__init__(self, master, {})
		self.textvariable = textvariable or tk.StringVar()
		e = tk.Entry(self, width=40)
		e.configure(textvariable=self.textvariable)
		e.pack(side=tk.LEFT)
		tk.Button(self, text="Choose...", command=self.choose).pack(side=tk.LEFT)

# Create a gui row containing text, a textbox, and a directory chooser button.
class DirectoryEntry(_EntryBase):
	def choose(self):
		v=self.textvariable
		v.set( askOpenDirectory(initialdir=v.get()) or v.get() )

# Create a gui row containing text, a textbox, and a file chooser button.
class FileEntry(_EntryBase):
	def choose(self):
		v=self.textvariable
		v.set( askOpenFile(initialfile=v.get(), filetypes = '{{HKO Files} {.hko} }') or v.get() )
		

#===========================================
#
# Entry point
#
#===========================================

# If any command line arguments are supplied simply run the script as instructed
if len(sys.argv) > 1:
    
    assetPath = sys.argv[1]
    try:
        opts = getopt.getopt( sys.argv[2:], "h:s3:m:x:", ["hko=", "s", "3dsmax=", "maya=", "xsi="] )
        for opt, arg in opts[0]:
            if opt in ("-h", "--hko"):
                hkoPath = arg
            elif opt == '-s':
                saveAssets = "1"
            elif opt in ("-3", "--3dsmax"):
                maxPath = arg
            elif opt in ("-m", "--maya"):
                mayaPath = arg
            elif opt in ("-x", "--xsi"):
                xsiPath = arg
        if len(opts[1]) > 0:
            raise getopt.GetoptError("Too many parameters : [%s]" % " ".join(opts[1]),None)
    except getopt.error, msg:
        usage()
        sys.exit(2)
    
    doit()
    
else:
    
    # Otherwise create a GUI asking the user to fill in the options.
    optframe = tk.Frame()
    optframe.pack(side=tk.TOP)

    currow = 0

    # Create a text box for the target directory.
    uiAssetPath = tk.StringVar()
    uiAssetPath.set( assetPath )
    tk.Label(optframe, text="Asset directory:").grid(row=currow, column=0, sticky=tk.E)
    DirectoryEntry(optframe, textvariable=uiAssetPath).grid(row=currow, column=1, sticky=tk.W )
    currow += 1

    # Create a text box for the user to specify a .hko options file to use.
    uiHkoPath = tk.StringVar()
    uiHkoPath.set( hkoPath )
    tk.Label(optframe, text="Optional .HKO file:").grid(row=currow, column=0, sticky=tk.E)
    FileEntry(optframe, textvariable=uiHkoPath).grid(row=currow, column=1, sticky=tk.W )
    currow += 1

    # Create a checkbox for the user to choose whether to save changes or not.
    uiSaveAssets = tk.StringVar()
    uiSaveAssets.set( saveAssets )
    cb = tk.Checkbutton( optframe, text="Save changes to processed assets (e.g. updated filter options)", variable=uiSaveAssets).grid(row=currow, column=1, sticky=tk.E )
    currow += 1

    # Create a text box for the path to 3ds Max.
    uiMaxPath = tk.StringVar()
    uiMaxPath.set( maxPath )
    tk.Label(optframe, text="3ds Max directory:").grid(row=currow, column=0, sticky=tk.E)
    DirectoryEntry(optframe, textvariable=uiMaxPath).grid(row=currow, column=1, sticky=tk.W )
    currow += 1

    # Create a text box for the path to Maya.
    uiMayaPath = tk.StringVar()
    uiMayaPath.set( mayaPath )
    tk.Label(optframe, text="Maya bin directory:").grid(row=currow, column=0, sticky=tk.E)
    DirectoryEntry(optframe, textvariable=uiMayaPath).grid(row=currow, column=1, sticky=tk.W )
    currow += 1

    # Create a text box for the path to XSI.
    uiXsiPath = tk.StringVar()
    uiXsiPath.set( xsiPath ) 
    tk.Label(optframe, text="XSI bin directory:").grid(row=currow, column=0, sticky=tk.E)
    DirectoryEntry(optframe, textvariable=uiXsiPath).grid(row=currow, column=1, sticky=tk.W )
    currow += 1

    # Tell the script what to do when the user presses the OK button.
    def call_doit():
            cursor_save = tk._default_root['cursor']
            tk._default_root.configure(cursor="watch")
            tk._default_root.update()
            
            # update globals with UI values
            global assetPath; assetPath = uiAssetPath.get()
            global hkoPath; hkoPath = uiHkoPath.get()
            global saveAssets; saveAssets = uiSaveAssets.get()
            global maxPath; maxPath = uiMaxPath.get()
            global mayaPath; mayaPath = uiMayaPath.get()
            global xsiPath; xsiPath = uiXsiPath.get()
            
            # do the processing
            doit()
            
            tk._default_root.configure(cursor=cursor_save)

    # Create OK and CANCEL buttons.
    ButtonBar( buttons= [
            (" Run ", call_doit),
            ("Quit", sys.exit),
            ] ).pack(side=tk.BOTTOM)

    # Wait for the user to choose their options.
    tk.mainloop()


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
