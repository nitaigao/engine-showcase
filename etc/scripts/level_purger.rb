require 'fileutils'

sourcePath = Dir.glob( File.join( ARGV[ 0 ].to_s.gsub( "\\", '/' ), '**' ) )
FileUtils.rm_rf( sourcePath )