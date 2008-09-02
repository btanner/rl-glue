/* 
* Copyright (C) 2007, Adam White
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA. */

#ifndef RL_network_h
#define RL_network_h

/* Defines types for RL-Glue */
#include <RL_common.h>

/* RL-Glue needs to know what type of object is trying to connect. */
#define kExperimentConnection  1
#define kAgentConnection       2
#define kEnvironmentConnection 3

#define kAgentInit    4 /* agent_* start by sending one of these values */
#define kAgentStart   5 /* to the client to let it know what type of    */
#define kAgentStep    6 /* event to respond to                          */
#define kAgentEnd     7
#define kAgentCleanup 8
#define kAgentFreeze  9
#define kAgentMessage 10

#define kEnvInit          11
#define kEnvStart         12
#define kEnvStep          13
#define kEnvCleanup       14
#define kEnvSetState      15
#define kEnvSetRandomSeed 16
#define kEnvGetState      17
#define kEnvGetRandomSeed 18
#define kEnvMessage       19

#define kRLInit           20
#define kRLStart          21
#define kRLStep           22
#define kRLCleanup        23
#define kRLReturn         24
#define kRLNumSteps       25
#define kRLNumEpisodes    26
#define kRLEpisode        27
#define kRLSetState       28
#define kRLSetRandomSeed  29
#define kRLGetState       30
#define kRLGetRandomSeed  31
#define kRLFreeze         32
#define kRLAgentMessage   33
#define kRLEnvMessage     34

#define kRLTerm           35

#define kLocalHost "127.0.0.1"
#define kDefaultPort 4096
#define kRetryTimeout 10

/* Data types */
typedef struct rlBuffer_t {
  unsigned int size;
  unsigned int capacity;
  unsigned char *data;
} rlBuffer;

/* Basic network functionality */
int rlOpen(short thePort);
int rlAcceptConnection(int theSocket);

int rlConnect(int theSocket, const char* theAddress, short thePort);
int rlListen(int theSocket, short thePort);
int rlClose(int theSocket);
int rlIsValidSocket(int theSocket);

int rlSendData(int socket, const void* data, int length);
int rlRecvData(int socket, void* data, int length);

/* rlBuffer API */
void rlBufferCreate(rlBuffer *buffer, unsigned int capacity);
void rlBufferDestroy(rlBuffer *buffer);
void rlBufferClear(rlBuffer *buffer);
void rlBufferReserve(rlBuffer *buffer, unsigned int capacity);
unsigned int rlBufferWrite(rlBuffer *buffer, unsigned int offset, const void* sendData, unsigned int count, unsigned int size);
unsigned int rlBufferRead(const rlBuffer *buffer, unsigned int offset, void* recvData, unsigned int count, unsigned int size);

/* Utilities */
unsigned int rlSendBufferData(int theSocket, const rlBuffer* buffer, const int target);
unsigned int rlRecvBufferData(int theSocket, rlBuffer* buffer, int* target);

int rlGetSystemByteOrder();
void rlSwapData(void* out, const void* in, const unsigned int size);
int rlWaitForConnection(const char *address, const short port, const int retryTimeout);
unsigned int rlCopyADTToBuffer(const RL_abstract_type* src, rlBuffer* dst, unsigned int offset);
unsigned int rlCopyBufferToADT(const rlBuffer* src, unsigned int offset, RL_abstract_type* dst);

#endif
