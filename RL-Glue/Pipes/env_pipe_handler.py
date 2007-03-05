import sys
import string
import types

env = None
pipe_in_stream = None
pipe_out_stream = None

def main():
	global env
	global pipe_in_stream
	global pipe_out_stream
	pipe_in = ""
	pipe_out = ""
	env_name = ''

	if(len(sys.argv) == 4):
		print "\nUsing user pipe names:\n"
		print "\t...%s\n\t...%s\n\n" % (sys.argv[1],sys.argv[2])
		pipe_in = sys.argv[1];
		pipe_out = sys.argv[2];
		env_name = sys.argv[3];
	elif len(sys.argv) == 2:
		print "\nUsing default pipe names:\n"
		print "\t.../tmp/RL_pipe_env_in\n\t.../tmp/RL_pipe_env_out\n\n"
		pipe_in = "/tmp/RL_pipe_env_in"
		pipe_out= "/tmp/RL_pipe_env_out"
		env_name = sys.argv[1]
	else:
		print "\nError: incorrect input arguments!\n\nExample of valid invokations:\n\t./RLenv /tmp/RL_pipe_env_in /tmp/RL_pipe_env_out\n...or...\n\t./RLenv\n\nExiting...\n\n"
		sys.exit(0)
	
	env = __import__(env_name,globals(),locals(),[])
	while 1:
		try:
			pipe_in_stream = open(pipe_in,"r")
			pipe_out_stream = open(pipe_out,"w")
		except IOError:
			continue
		break

	pipe_env_buf = pipe_in_stream.readline()
	while 1:
		if (pipe_env_buf == "init\n"):
			pipe_env_init()
		elif (pipe_env_buf == "start\n"):   
			pipe_env_start()
		elif (pipe_env_buf == "step\n"):   
			pipe_env_step()
		elif (pipe_env_buf == "end\n"):  
			pipe_env_end()
		elif (pipe_env_buf == "cleanup\n"):
			env.cleanup()
			pipe_in_stream.close()
			pipe_out_stream.close()
			sys.exit(1)
		pipe_env_buf = pipe_in_stream.readline()
		print pipe_env_buf

  
def pipe_env_init():
	task_spec = env.init()
	pipe_out_stream.write(task_spec + '\n')
	pipe_out_stream.flush()

def read_action():
	pipe_env_buf = pipe_in_stream.readline()
	while pipe_env_buf == '\n':
		pipe_env_buf = pipe_in_stream.readline()
		
	l = string.split(pipe_env_buf)
	action = None
	if len(l) == 1:
		if pipe_env_buf.find('.') != -1:
			action = float(pipe_env_buf)
		else:
			action = int(pipe_env_buf)
	else:
		if l[0].find('.') != -1:
			action = map(float,l)
		else:
			action = map(int,l)
	return action
		

def pipe_env_start():
	env_state = env.start()
	if isinstance(env_state,types.ListType):
		env_state = string.join(env_state," ")
	pipe_out_stream.write(str(env_state) + '\n')
	pipe_out_stream.flush()

def pipe_env_step():
	env_action = read_action()
	terminal,reward,env_state = env.step(env_action)
	if isinstance(env_state,types.ListType):
		env_state = string.join(env_state," ")
	pipe_out_stream.write(str(env_state) + '\n')
	pipe_out_stream.write(str(reward) + '\n')
	pipe_out_stream.write(str(terminal) + '\n')
	pipe_out_stream.flush()

if __name__ == '__main__':
	main()

