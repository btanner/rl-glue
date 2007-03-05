import sys
import string
import types

agent = None
pipe_in_stream = None
pipe_out_stream = None

def main():
	global pipe_in_stream
	global pipe_out_stream
	global agent

	pipe_in = ""
	pipe_out = ""
	agent_name = ''

	if(len(sys.argv) == 4):
		print "\nUsing user pipe names:\n"
		print "\t...%s\n\t...%s\n\n" % (sys.argv[1],sys.argv[2])
		pipe_in = sys.argv[1];
		pipe_out = sys.argv[2];
		agent_name = sys.argv[3];
	elif len(sys.argv) == 2:
		print "\nUsing default pipe names:\n"
		print "\t.../tmp/RL_pipe_agent_in\n\t.../tmp/RL_pipe_agent_out\n\n"
		pipe_in = "/tmp/RL_pipe_agent_in"
		pipe_out= "/tmp/RL_pipe_agent_out"
		agent_name = sys.argv[1]
	else:
		print "\nError: incorrect input arguments!\n\nExample of valid invokations:\n\t./RLagent /tmp/RL_pipe_agent_in /tmp/RL_pipe_agent_out\n...or...\n\t./RLagent\n\nExiting...\n\n"
		sys.exit(0)
	
	agent = __import__(agent_name,globals(),locals(),[])
	while 1:
		try:
			pipe_in_stream = open(pipe_in,"r")
			pipe_out_stream = open(pipe_out,"w")
		except IOError:
			continue
		break

	pipe_agent_buf = pipe_in_stream.readline()
	while 1:
		if (pipe_agent_buf == "init\n"):
			pipe_agent_init()
		elif (pipe_agent_buf == "start\n"):   
			pipe_agent_start()
		elif (pipe_agent_buf == "step\n"):   
			pipe_agent_step()
		elif (pipe_agent_buf == "end\n"):  
			pipe_agent_end()
		elif (pipe_agent_buf == "cleanup\n"):
			agent.cleanup()
			pipe_in_stream.close()
			pipe_out_stream.close()
			sys.exit(1)
		pipe_agent_buf = pipe_in_stream.readline()

  
def pipe_agent_init():
	pipe_agent_buf = pipe_in_stream.readline()
	#parse_task_spec2(pipe_agent_buf,&tss)
	#my_alloc(agent_state)
	agent.init(pipe_agent_buf)

def read_state():
	pipe_agent_buf = pipe_in_stream.readline()
	l = string.split(pipe_agent_buf)
	state = None
	if len(l) == 1:
		if pipe_agent_buf.find('.') != -1:
			state = float(pipe_agent_buf)
		else:
			state = int(pipe_agent_buf)
	else:
		if l[0].find('.') != -1:
			state = map(float,l)
		else:
			state = map(int,l)
	return state
		

def pipe_agent_start():
	agent_state = read_state()
	agent_action = agent.start(agent_state)
	if isinstance(agent_action,types.ListType):
		agent_action = string.join(agent_action," ")
	pipe_out_stream.write(str(agent_action) + '\n')
	pipe_out_stream.flush()

def pipe_agent_step():
	agent_state = read_state()
	r_str = pipe_in_stream.readline()
	while r_str == '\n':
		r_str = pipe_in_stream.readline()
	reward = float(r_str)
	agent_action = agent.step(reward,agent_state)
	if isinstance(agent_action,types.ListType):
		agent_action = string.join(agent_action," ")
	pipe_out_stream.write(str(agent_action) + '\n')
	pipe_out_stream.flush()

def pipe_agent_end():
	reward = float(pipe_in_stream.readline())
	agent.end(reward)

if __name__ == '__main__':
	main()

