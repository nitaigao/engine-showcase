load File.join( File.dirname( $0 ),'addPaths.rb' )

require 'cl/xmlserial'
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

contents = file.read.gsub( '<![CDATA[', '' ).gsub( ']]>', '' )
doc = Document.new( contents )
root = doc.root

class Serializable 

	include XmlSerialization
	
	def initialize( )
	
		XSConf.outputTypeElements = false
	
	end

end

class Color < Serializable

	def initialize( name, xmlNode )
	
		super( )
	
		@name = name
		@r = xmlNode.attributes[ 'r' ]
		@g = xmlNode.attributes[ 'g' ]
		@b = xmlNode.attributes[ 'b' ]
		@type = 'color'
	
	end
	
	def name=( name )
        @name = name
    end
    
    def name
        @name
    end
	
	def r=( r )
        @r = r
    end
    
    def r
        @r
    end
    
    def g
        @g
    end
    
    def g=( g )
        @g = g
    end
    
    def b
        @b
    end
    
    def b=( b )
        @b = b
    end
	
	def type
		@type
	end

end


class Fog < Serializable

	def initialize( xmlNode )
	
		super( )
	
		@mode = xmlNode.attributes[ 'mode' ];
		@linearStart = Float( xmlNode.attributes[ 'linearStart' ] ) * Float( xmlNode.elements[ '//clipping' ].attributes[ 'far' ] )
		@linearEnd = Float( xmlNode.attributes[ 'linearEnd' ] ) * Float( xmlNode.elements[ '//clipping' ].attributes[ 'far' ] )
		
		@r = xmlNode.elements[ 'colourDiffuse' ].attributes[ 'r' ];
		@g = xmlNode.elements[ 'colourDiffuse' ].attributes[ 'g' ];
		@b = xmlNode.elements[ 'colourDiffuse' ].attributes[ 'b' ];
		@type = 'fog'
	
	end

	def mode
		@mode
	end
	
	def linearStart
		@linearStart
	end
	
	def linearEnd
		@linearEnd
	end
	
	def r
		@r
	end
	
	def g
		@g
	end
	
	def b
		@b
	end
	
	def type
		@type
	end

end


class Vector < Serializable

    def initialize( x, y, z )
	
		super( )
		
        @x = x
        @y = y
        @z = z
		
    end
    
    def x=( x )
        @x = x
    end
    
    def x
        @x
    end
    
    def y
        @y
    end
    
    def y=( y )
        @y = y
    end
    
    def z
        @z
    end
    
    def z=( z )
        @z = z
    end

end

class Quaternion < Serializable

    def initialize( w, x, y, z )
	
		super( )
		
        @w = w
        @x = x
        @y = y
        @z = z
		
    end
    
    def w=( w )
        @w = w
    end
    
    def w
        @w
    end
    
    def x=( x )
        @x = x
    end
    
    def x
        @x
    end
    
    def y
        @y
    end
    
    def y=( y )
        @y = y
    end
    
    def z
        @z
    end
    
    def z=( z )
        @z = z
    end

end

class Component < Serializable

    def initialize( system )
	
		super( )
    
        @system = system
		@type = 'default'
    
    end
    
    def system
        @system
    end
	
	def type
        @type
    end

end

class GraphicsComponent < Component

    def initialize( ogreNode )
	
        super( 'graphics' )
		
		overrideModel = ogreNode.elements[ './/overrideGraphicsModel' ]
		defaultModel = $modelsGamePath + '/' + ogreNode.attributes[ 'name' ] + '.model'
		
		if ( overrideModel != nil ) then
		
			if ( overrideModel.text != nil ) then
		
				@model = $modelsGamePath + '/' + overrideModel.text
				
			else
			
				@model = defaultModel
			
			end
		
		else
		
			@model = defaultModel
			
		end
		
		graphicsType = ogreNode.elements[ './/graphicsType' ]
		
		if ( graphicsType != nil ) then
		
			@type = graphicsType.text
		
		end
		
    end
    
    def model
        @model
    end

end

class AnimationComponent < Component

	def initialize( ogreNode )
	
		super( 'animation' )
		
		@animations = Hash.new
		
		animationsPath = ogreNode.elements[ './/animationPath' ]
		
		if ( animationsPath.text != nil )
		
			fileExtension = '.hkx'
			fullAnimationPath = File.join( $sharedPath, animationsPath.text )
			searchPath = File.join( fullAnimationPath, '*' + fileExtension )
			
			Dir.glob( searchPath ).each { | file |
			
				fileName = File.basename( file )
				seperatorIndex = fileName.index( '_' )
				extensionIndex = fileName.index( fileExtension )
				animationName = fileName[ seperatorIndex + 1, extensionIndex - seperatorIndex - 1 ].downcase
				animationPath = File.join( $baseGamePath, animationsPath.text, fileName )
			
				@animations[ animationName ] = animationPath
			
			}
			
			bindPosePath = ogreNode.elements[ './/bindPose' ]
		
			if ( bindPosePath.text != nil )
			
				@bindPose = File.join( $baseGamePath, animationsPath.text, bindPosePath.text )
			
			end
			
			defaultAnimation = ogreNode.elements[ './/defaultAnimation' ]
		
			if ( defaultAnimation.text != nil )
			
				fileName = File.basename( defaultAnimation.text )
				seperatorIndex = fileName.index( '_' )
				extensionIndex = fileName.index( fileExtension )
				@defaultAnimation = fileName[ seperatorIndex + 1, extensionIndex - seperatorIndex - 1 ].downcase
			
			end
			
	
		end
	
	end

end

class ScriptComponent < Component

	def initialize( ogreNode )
	
		super( 'script' )
		
		scriptName = ogreNode.elements[ './/scriptName' ]
		
		@scriptPath = $scriptsGamePath + '/' + scriptName.text
	
	end

end

class AIComponent < Component

	def initialize( ogreNode )
	
		super( 'ai' )
		
		filePath = ogreNode.elements[ './/aifilePath' ]
		
		if ( filePath.text != nil )
		
			@filePath = $baseGamePath + '/' + filePath.text
		
		end
		
		aiType = ogreNode.elements[ './/aiType' ]
		
		if ( aiType.text != nil ) then
		
			@type = aiType.text
		
		end
	
	end

end

class GeometryComponent < Component

     def initialize( ogreNode )
	 
        super( 'geometry' )
    
        @scale = Vector.new( 0, 0, 0 )
        @position = Vector.new( 0, 0, 0 )
        @orientation = Quaternion.new( 0, 0, 0, 0 )
 
        @scale.x = ogreNode.elements[ 'scale' ].attributes[ 'x' ]
        @scale.y = ogreNode.elements[ 'scale' ].attributes[ 'y' ]
        @scale.z = ogreNode.elements[ 'scale' ].attributes[ 'z' ]
        
        @position.x = ogreNode.elements[ 'position' ].attributes[ 'x' ]
        @position.y = ogreNode.elements[ 'position' ].attributes[ 'y' ]
        @position.z = ogreNode.elements[ 'position' ].attributes[ 'z' ]
        
        @orientation.w = ogreNode.elements[ 'rotation' ].attributes[ 'qw' ]
        @orientation.x = ogreNode.elements[ 'rotation' ].attributes[ 'qx' ]
        @orientation.y = ogreNode.elements[ 'rotation' ].attributes[ 'qy' ]
        @orientation.z = ogreNode.elements[ 'rotation' ].attributes[ 'qz' ]
    end
    
    def position
        @position
    end
    
    def scale
        @scale
    end
    
    def orientation
        @orientation
    end

end

class PhysicsComponent < Component

	def initialize( ogreNode, name )
	
		super( 'physics' )
		
		@body = '/data/entities/bodies/' + name + '.hkx' 
		@type = ogreNode.elements[ './/physicsType' ].text
	
	end
	
	def body
        @body
    end

end

class InputComponent < Component

	def initialize( ogreNode )
	
		super( 'input' )
		
	end

end

class SoundComponent < Component

	def initialize( ogreNode )
	
		super( 'sound' )
		
	end

end

class SkyBox < Serializable

	def initialize( xmlNode )
	
		super( )
	
		@enabled = xmlNode.attributes[ 'enable' ]
		@material = xmlNode.attributes[ 'material' ]
		@distance = Float( xmlNode.attributes[ 'distance' ] ) * Float( xmlNode.elements[ '//clipping' ].attributes[ 'far' ] )
		@drawFirst = xmlNode.attributes[ 'drawFirst' ]
		@type = 'skybox'
	
	end
	
	def type
		@type
	end
	
	def enabled
		@enabled
	end
	
	def material
		@material
	end
	
	def distance
		@distance
	end
	
	def drawFirst
		@drawFirst
	end

end

class EntityLink < Serializable

	def initialize( subjectName, subjectSystem, observerName, observerSystem )
	
		super( )
	
		@subjectName = subjectName
		@subjectSystem = subjectSystem
		@observerName = observerName
		@observerSystem = observerSystem
		@type = 'entityLink'

	end
	
	def subjectName
		@subjectName
	end
	
	def subjectSystem
		@subjectSystem
	end
	
	def observerName
		@observerName
	end
	
	def observerSystem
		@observerSystem
	end
	
	def type
		@type
	end
	
end

class GameEntity < Serializable

    def initialize( ogreNode )
	
		super( )
    
		@name = ogreNode.attributes[ 'name' ]
		@type = 'entity'
		
		@components = Array.new
		
		attachGraphics = ogreNode.elements[ './/userData/components/attachGraphics' ]
		
		if ( attachGraphics != nil && attachGraphics.text == 'true')
			
			@components.push( GraphicsComponent.new( ogreNode ) )
		
		end
		
		attachPhysics = ogreNode.elements[ './/userData/components/attachPhysics' ]
		
		if ( attachPhysics != nil && attachPhysics.text == 'true')
			
			physicsNode = ogreNode.elements[ './/userData' ]
			@components.push( PhysicsComponent.new( physicsNode, @name ) )
		
		end
		
		attachAnimation = ogreNode.elements[ './/userData/components/attachAnimation' ]
		
		if ( attachAnimation != nil && attachAnimation.text == 'true')
			
			@components.push( AnimationComponent.new( ogreNode ) )
		
		end
		
		attachInput = ogreNode.elements[ './/userData/components/attachInput' ]
		
		if ( attachInput != nil && attachInput.text == 'true' )
		
			inputNode = ogreNode.elements[ './/userData' ]
			@components.push( InputComponent.new( inputNode ) )
		
		end
		
		attachSound = ogreNode.elements[ './/userData/components/attachSound' ]
		
		if ( attachSound != nil && attachSound.text == 'true' )
		
			soundNode = ogreNode.elements[ './/userData' ]
			@components.push( SoundComponent.new( soundNode ) )
		
		end
		
		attachScript = ogreNode.elements[ './/userData/components/attachScript' ]
		
		if ( attachScript != nil && attachScript.text == 'true' )
		
			scriptNode = ogreNode.elements[ './/userData' ]
			@components.push( ScriptComponent.new( scriptNode ) )
		
		end
		
		attachAI = ogreNode.elements[ './/userData/components/attachAI' ]
		
		if ( attachAI != nil && attachAI.text == 'true' )
		
			aiNode = ogreNode.elements[ './/userData' ]
			@components.push( AIComponent.new( aiNode ) )
			
			$entityLinks.push( EntityLink.new( 'player', 'geometry', @name, 'ai' ) );
		
		end
		
		#Must come last so the game can send the geometry information to all previously registered components
		@components.push( GeometryComponent.new( ogreNode ) )
        
    end
    
    def name
        @name
    end
    
    def components
        @components
    end
	
	def type
		@type
	end

end

def createEntities( xmlRoot )

	entities = Array.new
	
	#replaceMeshPaths( xmlRoot )

	xmlRoot.each_element( "/scene/nodes/node" ) { | node |

		entity = GameEntity.new( node )	
		
		entities.each { | savedEntity |
		
			if ( savedEntity.name.eql? entity.name ) then
			
				puts '#### ERROR: Duplicate entity name found for: ' + entity.name
				Process.exit
			
			end
		}
		
		entities.push( entity )
		
		modelFilePath = $modelsFullPath + '/' + entity.name + '.model'
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
		
		puts 'Processed GameEntity: ' + entity.name
	}
	
	return entities

end

def createEnvironment( xmlRoot )

	environment = Array.new
	
	ambientColor = Color.new( 'ambient', xmlRoot.elements[ '//colourAmbient' ] )
	environment.push( ambientColor )
	
	backgroundColor = Color.new( 'background', xmlRoot.elements[ '//colourBackground' ] )
	environment.push( backgroundColor )
	
	skyBoxElement = xmlRoot.elements[ '//skyBox' ]
	
	if ( skyBoxElement != nil ) then
	
		skyBox = SkyBox.new( skyBoxElement )
		environment.push( skyBox )
	
	end
	
	fogElement = xmlRoot.elements[ '//fog' ];
	
	if ( fogElement != nil ) then
		
		fog = Fog.new( fogElement )
		environment.push( fog )
	
	end
	
	return environment

end

def createLinks( xmlRoot )

	return $entityLinks

end

def createBaseStructure( )

	FileUtils.mkdir_p( $bodiesFullPath )
	FileUtils.mkdir_p( $texturesFullPath )
	FileUtils.mkdir_p( $meshesFullPath )
	FileUtils.mkdir_p( $modelsFullPath )

end

levelFilePath = File.join( $basePath, $levelName + '.yaml' )

outputFile = File.new( levelFilePath,  "w+" )
outputFile.write( createEnvironment( root ).to_yaml )
outputFile.write( "...\n" )
outputFile.write( createEntities( root ).to_yaml )
outputFile.write( "...\n" )
outputFile.write( createLinks( root ).to_yaml )
outputFile.write( "...\n" )
outputFile.write( "---\n" )
outputFile.close( )

puts ''
puts 'Deleting Old Scene File'
puts ''

	file.close( )

	#FileUtils.rm( sourcePath )

puts ''
puts 'Finished Deleting Old Scene File'
puts ''

#temporary until we figure out a better export pipeline
FileUtils.mv( levelFilePath, '../../levels/levels' )


