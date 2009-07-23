require 'yaml'
require 'fileutils'
require 'rexml/document'
include REXML

sourcePath = ARGV[ 0 ].to_s.gsub( '\\', '/' )
file = File.new( sourcePath )

$workingDirectory = File.dirname( sourcePath )
$levelName = File.basename( sourcePath, '.scene' )
$basePath = $workingDirectory
$sharedPath = File.join( $basePath, '../../shared/entities' )

$bodiesFullPath = $basePath + '/bodies'
$texturesFullPath = $basePath + '/textures'
$meshesFullPath = $basePath + '/meshes'
$modelsFullPath = $basePath + '/models'
$scriptsFullPath = $basePath + '/scripts'

$baseGamePath = '/data/entities'
$bodiesGamePath = $baseGamePath + '/bodies'
$texturesGamePath = $baseGamePath + '/textures'
$meshesGamePath = $baseGamePath + '/meshes'
$modelsGamePath = $baseGamePath + '/models'
$scriptsGamePath = $baseGamePath + '/scripts'
$animationsGamePath = $baseGamePath + '/animations'

$entityLinks = Array.new

def createBaseStructure( )

	FileUtils.mkdir_p( $bodiesFullPath )
	FileUtils.mkdir_p( $texturesFullPath )
	FileUtils.mkdir_p( $meshesFullPath )
	FileUtils.mkdir_p( $modelsFullPath )

end

def processModels( xmlRoot )

	xmlRoot.each_element( "/scene/nodes/node" ) { | node |
		
		modelFilePath = $modelsFullPath + '/' + node.attributes[ 'name' ] + '.model'
		modelFile = File.new( modelFilePath, "w+" )
		
		outputXml = node
		outputXml.elements.delete_all( './userData' )
		outputXml.elements.delete_all( '*/userData' )
		outputXml.elements.delete_all( '*/*/userData' )
		outputXml.elements.delete_all( '*/*/*/userData' )
		outputXml.elements.delete_all( '*/*/*/*/userData' )
		outputXml.elements.delete_all( '*/*/*/*/*/userData' )
		
		formatter = REXML::Formatters::Pretty.new
		modelNode = String.new
		formatter.write( outputXml, modelNode )
		modelNode = modelNode.gsub(/meshFile='([A-Za-z0-9_\-\.]*\.mesh)'/, "meshFile='" + $meshesGamePath + "/" + '\1' + "'")

		modelFile.write( modelNode )
		modelFile.close( )
		
		puts 'Processed Model: ' + node.attributes[ 'name' ]
	}

end

def processMaterials( materialsPath )

	materialsDir = Dir.new( materialsPath )
	materialsDir.each { | entry |
	
		if ( entry.to_s.include? ".material" ) then
		
			materialFilePath = File.join( materialsPath, entry )
		
			materialFile = File.new( materialFilePath, 'r+' )
			materialContents = materialFile.read( )
			
			if ( !materialContents.to_s.include? $texturesGamePath )
			
				materialContents = materialContents.gsub(/([A-Za-z0-9_\-\.]*)(jpg|tga|bmp|gif|png)/, $texturesGamePath + '/\0 ')
				
				materialFile.close( )
				
				materialFile = File.new( materialFilePath, 'w' )
				materialFile.write( materialContents )
				materialFile.close( )
				
				puts 'Processed Material: ' + entry
				
			end
			
		end
	}

end

def processMeshes( meshesPath )

	meshesDir = Dir.new( meshesPath )
	meshesDir.each { | entry |
	
		if ( entry.to_s.include? ".mesh" ) then
		
			meshFilePath = File.join( meshesPath, entry )
		
			meshFile = File.new( meshFilePath, 'rb+' )
			meshContents = meshFile.read( )
			
			skeletonFilename = File.basename( entry, '.mesh' ) + '.skeleton'
			
			if ( !meshContents.to_s.include? $meshesGamePath )
			
				meshContents = meshContents.gsub( /([A-Za-z0-9_\-\.]*)(.skeleton)/, $meshesGamePath + '/' + '\0' )
				meshFile.close( )
				
				meshFile = File.new( meshFilePath, 'wb' )
				meshFile.write( meshContents )
				meshFile.close( )
				
				puts 'Processed Mesh: ' + entry
			
			end
			
		end
		
	}

end

createBaseStructure( )

puts ''
puts 'Processing Scene Files'
puts ''

puts ''
puts 'Processing Scene File: ' + ARGV[ 0 ].to_s
puts ''

contents = file.read.gsub( '<![CDATA[', '' ).gsub( ']]>', '' )

doc = Document.new( contents )
root = doc.root
processModels( root )


puts ''
puts 'Finished Processing Scene Files'
puts ''

puts ''
puts 'Processing Materials'
puts ''

materialPath = File.join( $workingDirectory, 'materials' )
processMaterials( materialPath )

puts ''
puts 'Processing Meshes'
puts ''

meshesPath = File.join( $workingDirectory, 'meshes' )
processMeshes( meshesPath )

puts ''
puts 'Finished Processing Materials'
puts ''