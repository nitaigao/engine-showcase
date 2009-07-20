
task :clean do
	filesToPurge = Array.new;
	filesToPurge.push( '../../doc/api/*' );
	
	filesToPurge.push( '../../src/**/*.obj' );
	filesToPurge.push( '../../src/**/*.cache' );
	filesToPurge.push( '../../src/**/*.ncb' );
	filesToPurge.push( '../../src/**/*.user' );
	filesToPurge.push( '../../src/**/*.lnt' );
	filesToPurge.push( '../../src/**/*.exp' );
	filesToPurge.push( '../../src/**/*.ilk' );
	filesToPurge.push( '../../src/**/*.manifest' );
	filesToPurge.push( '../../src/**/*.log' );
	filesToPurge.push( '../../src/**/*.dep' );
	filesToPurge.push( '../../src/**/*.idb' );
	
	filesToPurge.push( '../../etc/vendor/**/*.obj' );
	filesToPurge.push( '../../etc/vendor/**/*.cache' );
	filesToPurge.push( '../../etc/vendor/**/*.ncb' );
	filesToPurge.push( '../../etc/vendor/**/*.user' );
	filesToPurge.push( '../../etc/vendor/**/*.lnt' );
	filesToPurge.push( '../../etc/vendor/**/*.exp' );
	filesToPurge.push( '../../etc/vendor/**/*.ilk' );
	filesToPurge.push( '../../etc/vendor/**/*.manifest' );
	filesToPurge.push( '../../etc/vendor/**/*.log' );
	filesToPurge.push( '../../etc/vendor/**/*.dep' );
	filesToPurge.push( '../../etc/vendor/**/*.idb' );

	results = FileList.new( filesToPurge );

	FileUtils.rm_r results
end

task :default => ["clean"]
