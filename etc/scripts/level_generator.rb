require 'yaml'
require 'rexml/document'
include REXML

file = File.new( ARGV[ 0 ].to_s );

contents = file.read.gsub( '<![CDATA[', '' ).gsub( ']]>', '' )

doc = Document.new( contents );
root = doc.root;

class Color

	def initialize( name, xmlNode )
	
		@name = name;
		@r = xmlNode.attributes[ 'r' ];
		@g = xmlNode.attributes[ 'g' ];
		@b = xmlNode.attributes[ 'b' ];
	
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

end

class Environment

	def initialize( )
	
		@colors = Array.new;
	
	end

	def colors
        @colors
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
        @model = '/data/entities/models/' + ogreNode.attributes[ 'name' ] + '.model'
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
		
		@body = ogreNode.elements[ '//body' ].text;
		@type = ogreNode.elements[ '//type' ].text;
	
	end
	
	def body
        @body
    end
    
    def type
        @type
    end

end

class Entity

    def initialize( ogreNode )
    
		@name = ogreNode.attributes[ 'name' ];
		
		@components = Array.new
		
		if @name != 'player_spawn' then
		
			@components.push( GraphicsComponent.new( ogreNode ) );        
		
		end
	
        @components.push( GeometryComponent.new( ogreNode ) );
		
		physicsNode = ogreNode.elements[ 'entity//Physics' ];
		
		if ( physicsNode != nil )
			
			@components.push( PhysicsComponent.new( physicsNode ) );
		
		end
        
    end
    
    def name
        @name
    end
    
    def components
        @components
    end

end

def createEntities( xmlRoot )

	entities = Array.new;

	xmlRoot.each_element( "//node" ) { | node |

		entity = Entity.new( node );	
		entities.push( entity );
		
		puts 'Processed Entity: ' + entity.name;
	}
	
	return entities;

end

def createEnvironment( xmlRoot )

	environment = Environment.new;
	
	ambientColor = Color.new( 'ambient', xmlRoot.elements[ '//colourAmbient' ] );
	environment.colors.push( ambientColor );
	
	backgroundColor = Color.new( 'background', xmlRoot.elements[ '//colourBackground' ] );
	environment.colors.push( backgroundColor );

end

puts ''
puts 'Processing Scene File: ' + ARGV[ 0 ].to_s
puts ''

levelName = ARGV[ 0 ]
levelFilePath = levelName.to_s.gsub( '.scene', '.yaml' );

outputFile = File.new( levelFilePath,  "w+" );
outputFile.write( createEnvironment( root ).to_yaml );
outputFile.write( "...\n" );
outputFile.write( createEntities( root ).to_yaml );
outputFile.close( );

puts ''
puts 'Processing Finished'
puts ''