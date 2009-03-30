class ExecCmd 
	attr_reader :output,:cmd,:exec_time 
	#When a block is given, the command runs before yielding 
	def initialize cmd 
	@cmd=cmd 
	@cmd_run=cmd+" 2>&1" unless cmd=~/2>&1/ 
	if block_given? 
			run 
			yield self 
	end 
	end 
	#Runs the command 
	def run 
			t1=Time.now 
			IO.popen(@cmd_run){|f| 
			@output=f.read 
			@process=Process.waitpid2(f.pid)[1] 
			} 
			@exec_time=Time.now-t1 
	end 
	#Returns false if the command hasn't been executed yet 
	def run? 
			return false unless @process 
			return true 
	end 
	#Returns the exit code for the command. Runs the command if it hasn't run yet. 
	def exitcode 
			run unless @process 
			@process.exitstatus 
	end 
	#Returns true if the command was succesfull. 
	#Will return false if the command hasn't been executed 
	def success? 
			return @process.success? if @process 
			return false 
	end 
end 

class VersionInfo
	def getVersion( )
		cmd = ExecCmd.new( 'git show --abbrev-commit | grep "^commit"' )
		cmd.run
		output = cmd.output.to_s.gsub( 'commit ', '' )
		output = output.gsub( "\n", '' );
		return output;
	end
end

#versionInfo = VersionInfo.new
#output = versionInfo.getVersion( )
