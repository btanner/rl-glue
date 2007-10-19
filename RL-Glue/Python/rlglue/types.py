# 
# Copyright (C) 2007, Mark Lee
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.


class Action:
	intArray = []
	doubleArray = []
	def __init__(self,numInts=None,numDoubles=None):
		if numInts != None:
			intArray = [0]*numInts
		if numDoubles != None:
			doubleArray = [0.0]*numDoubles

class Observation:
	intArray = []
	doubleArray = []
	def __init__(self,numInts=None,numDoubles=None):
		if numInts != None:
			intArray = [0]*numInts
		if numDoubles != None:
			doubleArray = [0.0]*numDoubles

class Random_seed_key:
	intArray = []
	doubleArray = []
	def __init__(self,numInts=None,numDoubles=None):
		if numInts != None:
			intArray = [0]*numInts
		if numDoubles != None:
			doubleArray = [0.0]*numDoubles

class State_key:
	intArray = []
	doubleArray = []
	def __init__(self,numInts=None,numDoubles=None):
		if numInts != None:
			intArray = [0]*numInts
		if numDoubles != None:
			doubleArray = [0.0]*numDoubles

class Observation_action:
	o = Observation()
	a = Action()
	def __init__(self,theObservation=None,theAction=None):
		if theObservation != None:
			this.o = theObservation
		if theAction != None:
			this.a = theAction

class Reward_observation:
	r = 0.0
	o = Observation()
	terminal = False
	def __init__(self,reward=None, theObservation=None, terminal=None):
		if reward != None:
			r = reward
		if theObservation != None:
			this.o = theObservation
		if terminal != None:
			this.terminal = terminal

class Reward_observation_action_terminal:
	r = 0.0
	o = Observation()
	a = Action()
	terminal = False
	def __init__(self,reward=None, theObservation=None, theAction=None, terminal=None):
		if reward != None:
			r = reward
		if theObservation != None:
			this.o = theObservation
		if theAction != None:
			this.a = theAction
		if terminal != None:
			this.terminal = terminal