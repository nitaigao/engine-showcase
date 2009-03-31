
task :clean do
	filesToPurge = Array.new;
	filesToPurge.push( '../../**/*.obj' );
	filesToPurge.push( '../../**/*.cache' );
	filesToPurge.push( '../../**/*.ncb' );
	filesToPurge.push( '../../**/*.user' );
	filesToPurge.push( '../../**/*.lnt' );
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