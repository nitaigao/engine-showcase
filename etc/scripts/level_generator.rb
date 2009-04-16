require 'yaml'
require 'fileutils'
require 'rexml/document'
include REXML

sourcePath = ARGV[ 0 ].to_s;
file = File.new( sourcePath );

$workingDirectory = File.dirname( sourcePath );
$levelName = File.basename( sourcePath, '.scene' )
$basePath = File.join( $workingDirectory, $levelName );

$bodiesFullPath = $basePath + '/bodies'
$texturesFullPath = $basePath + '/textures'
$meshesFullPath = $basePath + '/meshes'
$modelsFullPath = $basePath + '/models'

$baseGamePath = '/data/levels/' + $levelName
$bodiesGamePath = $baseGamePath + '/bodies'
$texturesGamePath = $baseGamePath + '/textures'
$meshesGamePath = $baseGamePath + '/meshes'
$modelsGamePath = $baseGamePath + '/models'

contents = file.read.gsub( '<![CDATA[', '' ).gsub( ']]>', '' )

doc = Document.new( contents );
root = doc.root;

class Color

	def initialize( name, xmlNode )
	
		@name = name;
		@r = xmlNode.attributes[ 'r' ];
		@g = xmlNode.attributes[ 'g' ];
		@b = xmlNode.attributes[ 'b' ];
		@type = 'color';
	
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

class Vector

    def initialize( x, y, z )
        @x = x;
        @y = y;
        @z = z;
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

class Quaternion

    def initialize( w, x, y, z )
        @w = w;
        @x = x;
        @y = y;
        @z = z;
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

class Component

    def initialize( system )
    
        @system = system
    
    end
    
    def system
        @system
    end

end

class GraphicsComponent < Component

    def initialize( ogreNode )
        @system = 'graphics';
        @model = $modelsGamePath + '/' + ogreNode.attributes[ 'name' ] + '.model'
    end
    
    def model
        @model
    end

end

class GeometryComponent < Component

     def initialize( ogreNode )
        @system = 'geometry';
    
        @scale = Vector.new( 0, 0, 0 );
        @position = Vector.new( 0, 0, 0 );
        @orientation = Quaternion.new( 0, 0, 0, 0 );
 
        @scale.x = ogreNode.elements[ 'scale' ].attributes[ 'x' ];
        @scale.y = ogreNode.elements[ 'scale' ].attributes[ 'y' ];
        @scale.z = ogreNode.elements[ 'scale' ].attributes[ 'z' ];
        
        @position.x = ogreNode.elements[ 'position' ].attributes[ 'x' ];
        @position.y = ogreNode.elements[ 'position' ].attributes[ 'y' ];
        @position.z = ogreNode.elements[ 'position' ].attributes[ 'z' ];
        
        @orientation.w = ogreNode.elements[ 'rotation' ].attributes[ 'qw' ];
        @orientation.x = ogreNode.elements[ 'rotation' ].attributes[ 'qx' ];
        @orientation.y = ogreNode.elements[ 'rotation' ].attributes[ 'qy' ];
        @orientation.z = ogreNode.elements[ 'rotation' ].attributes[ 'qz' ];
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

	def initialize( ogreNode )
	
		@system = 'physics'
		
		@body = File.join( $bodiesGamePath, ogreNode.elements[ './/physicsBody' ].text );
		@type = ogreNode.elements[ './/physicsType' ].text;
	
	end
	
	def body
        @body
    end
    
    def type
        @type
    end

end

class InputComponent < Component

	def initialize( ogreNode )
	
		@system = 'input'
		
	end

end

class SkyBox

	def initialize( ogreNode )
	
		@enabled = ogreNode.attributes[ 'enable' ];
		@material = ogreNode.attributes[ 'material' ];
		@distance = ogreNode.attributes[ 'distance' ];
		@drawFirst = ogreNode.attributes[ 'drawFirst' ];
		@type = 'skybox';
	
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

class Entity

    def initialize( ogreNode )
    
		@name = ogreNode.attributes[ 'name' ];
		@type = 'entity'
		
		@components = Array.new
		
		attachGraphics = ogreNode.elements[ './/userData/components/attachGraphics' ];
		
		if ( attachGraphics == nil || attachGraphics.text == 'true')
			
			@components.push( GraphicsComponent.new( ogreNode ) );
		
		end
	
        @components.push( GeometryComponent.new( ogreNode ) );
		
		attachPhysics = ogreNode.elements[ './/userData/components/attachPhysics' ];
		
		if ( attachPhysics != nil && attachPhysics.text == 'true')
			
			physicsNode = ogreNode.elements[ './/userData' ];
			@components.push( PhysicsComponent.new( physicsNode ) );
		
		end
		
		attachInput = ogreNode.elements[ './/userData/components/attachInput' ];
		
		if ( attachInput != nil && attachInput.text == 'true' )
		
			inputNode = ogreNode.elements[ './/userData' ];
			@components.push( InputComponent.new( inputNode ) );
		
		end
        
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

	entities = Array.new;

	xmlRoot.each_element( "/scene/nodes/node" ) { | node |

		entity = Entity.new( node );	
		
		entities.each { | savedEntity |
		
			if ( savedEntity.name.eql? entity.name ) then
			
				puts '#### ERROR: Duplicate entity name found for: ' + entity.name;
				Process.exit;
			
			end
		}
		
		entities.push( entity );
		
		modelFilePath = $modelsFullPath + '/' + entity.name + '.model';
		modelFile = File.new( modelFilePath, "w+" );
		
		node.elements.delete( 'entity//userData' );
		
		node.each_element( 'entity' ) { | entityNode |

			meshFile = entityNode.attributes[ 'meshFile' ];
			fullMeshFile = $meshesGamePath + '/' + meshFile.to_s;
			entityNode.attributes[ 'meshFile' ] = fullMeshFile;
			
			entityNode.each_element( './/entity' ) { | subEntityNode |
			
				meshFile = subEntityNode.attributes[ 'meshFile' ];
				fullMeshFile = $meshesGamePath + '/' + meshFile.to_s;
				subEntityNode.attributes[ 'meshFile' ] = fullMeshFile;
			
			}
			
		}
		
		formatter = REXML::Formatters::Pretty.new
		modelNode = String.new
		formatter.write( node, modelNode );
	
		modelFile.write( modelNode );
		modelFile.close( );
		
		puts 'Processed Entity: ' + entity.name;
	}
	
	return entities;

end

def createEnvironment( xmlRoot )

	environment = Array.new;
	
	ambientColor = Color.new( 'ambient', xmlRoot.elements[ '//colourAmbient' ] );
	environment.push( ambientColor );
	
	backgroundColor = Color.new( 'background', xmlRoot.elements[ '//colourBackground' ] );
	environment.push( backgroundColor );
	
	skyBoxElement = xmlRoot.elements[ '//skyBox' ];
	
	if ( skyBoxElement != nil ) then
	
		skyBox = SkyBox.new( skyBoxElement );
		environment.push( skyBox );
	
	end
	
	return environment;

end

def processMaterials( materialsPath )

	materialsDir = Dir.new( materialsPath );
	materialsDir.each { | entry |
	
		if ( entry.to_s.include? ".material" ) then
		
			materialFilePath = File.join( materialsPath, entry );
		
			materialFile = File.new( materialFilePath, 'r+' );
			materialContents = materialFile.read( );
			
			if ( !materialContents.to_s.include? $texturesGamePath )
			
				materialContents = materialContents.gsub( "\ttexture ", "\ttexture " + $texturesGamePath + '/' );
				materialFile.close( );
				
				materialFile = File.new( materialFilePath, 'w' );
				materialFile.write( materialContents );
				materialFile.close( );
				
				puts 'Processed Material: ' + entry;
				
			end
			
		end
	}

end

def processMeshes( meshesPath )

	meshesDir = Dir.new( meshesPath );
	meshesDir.each { | entry |
	
		if ( entry.to_s.include? ".mesh" ) then
		
			meshFilePath = File.join( meshesPath, entry );
		
			meshFile = File.new( meshFilePath, 'r+' );
			meshContents = meshFile.read( );
			
			skeletonFilename = File.basename( entry, '.mesh' ) + '.skeleton'
			
			if ( !meshContents.to_s.include? $meshesGamePath )
			
				meshContents = meshContents.gsub( skeletonFilename, $meshesGamePath + '/' + skeletonFilename );
				meshFile.close( );
				
				meshFile = File.new( meshFilePath, 'w' );
				meshFile.write( meshContents );
				meshFile.close( );
				
				puts 'Processed Mesh: ' + entry;
			
			end
			
		end
		
	}

end

def createBaseStructure( )

	FileUtils.mkdir_p( $bodiesFullPath );
	FileUtils.mkdir_p( $texturesFullPath );
	FileUtils.mkdir_p( $meshesFullPath );
	FileUtils.mkdir_p( $modelsFullPath );

end

puts ''
puts 'Processing Scene Files'
puts ''

puts ''
puts 'Processing Scene File: ' + ARGV[ 0 ].to_s
puts ''

createBaseStructure( );

levelFilePath = File.join( $basePath, $levelName + '.yaml' );

outputFile = File.new( levelFilePath,  "w+" );
outputFile.write( createEnvironment( root ).to_yaml );
outputFile.write( "...\n" );
outputFile.write( createEntities( root ).to_yaml );
outputFile.close( );

puts ''
puts 'Finished Processing Scene Files'
puts ''

puts ''
puts 'Processing Materials'
puts ''

materialPath = File.join( $workingDirectory, 'materials' );
processMaterials( materialPath );
FileUtils.cp_r( materialPath, $basePath );

puts ''
puts 'Processing Meshes'
puts ''

meshesPath = File.join( $workingDirectory, 'meshes' );
processMeshes( meshesPath );
FileUtils.cp_r( meshesPath, $basePath );

puts ''
puts 'Finished Processing Materials'
puts ''