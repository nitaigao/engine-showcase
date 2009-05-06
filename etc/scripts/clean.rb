
task :clean do
	filesToPurge = Array.new;
	filesToPurge.push( '../../doc/api/*' );
	filesToPurge.push( '../../src/**/*.obj' );
	filesToPurge.push( '../../src/**/*.cache' );
	filesToPurge.push( '../../src/**/*.ncb' );
	filesToPurge.push( '../../src/**/*.user' );
	filesToPurge.push( '../../src/**/*.lnt' );
	filesToPurge.push( '../../**/*.exp' );
	filesToPurge.push( '../../**/*.ilk' );
	filesToPurge.push( '../../**/*.manifest' );
	filesToPurge.push( '../../**/*.log' );
	filesToPurge.push( '../../**/*.dep' );
	filesToPurge.push( '../../**/*.idb' );
	filesToPurge.push( '../../**/*.dep' );

	results = FileList.new( filesToPurge );

	FileUtils.rm_r results
end

task :default => ["clean"]
