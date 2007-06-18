/* Standard Headers */
#include <assert.h> /* assert */
#include <stdlib.h> /* calloc */
#include <string.h> /* memset */

/*#include <stdio.h>*/ /* fprintf : Debug only */

/* Network Headers */
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

/* RL_netlib Library Header */
#include <Network/RL_netlib.h>

extern void rlSetAgentConnection(int);
extern void rlSetEnvironmentConnection(int);
extern void rlSetExperimentConnection(int);

rlSocket rlOpen(short thePort) {
  int flag = 1;
  rlSocket theSocket = 0;

  theSocket = socket(PF_INET, SOCK_STREAM, 0);
  setsockopt(theSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));

  return theSocket;
}

rlSocket rlAcceptConnection(rlSocket theSocket) {
  rlSocket theClient = 0;
  struct sockaddr_in theClientAddress = {0};
  unsigned int theSocketSize = sizeof(struct sockaddr_in);
  theClient = accept(theSocket, (struct sockaddr*)&theClientAddress, &theSocketSize);
  return theClient;
}

int rlConnect(rlSocket theSocket, const char* theAddress, short thePort) {
  int theStatus = 0;
  struct sockaddr_in theDestination;
  theDestination.sin_family = AF_INET;
  theDestination.sin_port = htons(thePort);
  theDestination.sin_addr.s_addr = inet_addr(theAddress);
  memset(&theDestination.sin_zero, '\0', 8);

  theStatus = connect(theSocket, 
		      (struct sockaddr*)&theDestination, 
		      sizeof(struct sockaddr));

  return theStatus;
}

int rlListen(rlSocket theSocket, short thePort) {
  struct sockaddr_in theServer;
  int theStatus = 0;
  int yes = 1;
  
  theServer.sin_family = AF_INET;
  theServer.sin_port = htons(thePort);
  theServer.sin_addr.s_addr = INADDR_ANY;
  memset(&theServer.sin_zero, '\0', 8);
  
  /* We don't really care if this fails... */
  setsockopt(theSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

  theStatus = bind(theSocket, 
		   (struct sockaddr*)&theServer, 
		   sizeof(struct sockaddr));

  if (theStatus == -1) return -1;
  
  theStatus = listen(theSocket, 10);
  if (theStatus == -1) return -1;

  return theStatus;
}

int rlClose(rlSocket theSocket) {
  return close(theSocket);
}

int rlIsValidSocket(rlSocket theSocket) {
  return theSocket != -1;
}

int rlSendData(rlSocket theSocket, const void* theData, int theLength) {
  int theBytesSent = 0;
  int theMsgError = 0;
  const char* theDataBuffer = (const char*)theData;
  
  while (theBytesSent < theLength) {
    theMsgError = send(theSocket, theDataBuffer + theBytesSent, theLength - theBytesSent, 0);
    if (theMsgError == -1) break;
    else theBytesSent += theMsgError;
  }

  return theBytesSent;
}

int rlRecvData(rlSocket theSocket, void* theData, int theLength) {
  int theBytesRecv = 0;
  int theMsgError = 0;
  char* theDataBuffer = (char*)theData;
  
  while (theBytesRecv < theLength) {
    theMsgError = recv(theSocket, theDataBuffer + theBytesRecv, theLength - theBytesRecv, 0);
    if (theMsgError <= 0) break;
    else theBytesRecv += theMsgError;
  }

  return theBytesRecv;
}

/* rlBuffer API */
void rlBufferCreate(rlBuffer *buffer, unsigned int capacity) {
  buffer->size     = 0;
  buffer->capacity = 0;
  
  if (capacity > 0) {
    rlBufferReserve(buffer, capacity);
  }
}

void rlBufferDestroy(rlBuffer *buffer) {
  free(buffer->data);
}

void rlBufferClear(rlBuffer *buffer) {
  buffer->size = 0;
}

void rlBufferReserve(rlBuffer *buffer, unsigned int capacity) {
  unsigned char* new_data = 0;

  /* Ensure the buffer can hold the new data */
  if (capacity > buffer->capacity) {

    /* Allocate enough memory for the additional data */
    new_data = (unsigned char*)malloc(capacity + (capacity - buffer->capacity) * 2);
    assert(new_data != 0);

    /* Copy the existing data into the the larger memory allocation */
    if (buffer->size > 0) {
      memcpy(new_data, buffer->data, buffer->size);
    }

    /* Free the original data */
    free(buffer->data);

    /* Set the buffers data to the new data pointer */
    buffer->data = new_data;

    /* Set the new capacity */
    buffer->capacity = capacity + (capacity - buffer->capacity) * 2;
  }
}

unsigned int rlBufferWrite(rlBuffer *buffer, unsigned int offset, const void* sendData, unsigned int count, unsigned int size) {
  const unsigned char* data = (const unsigned char*)sendData;
  unsigned char* data_ptr = 0;
  unsigned int i = 0;

  if (buffer->capacity < offset + count * size ) {
    rlBufferReserve(buffer, offset + count * size);
  }

  /* Get the offset to the place in the buffer we want to start inserting */
  data_ptr = buffer->data + offset;

  /* For each of the new data items, swap the endianness and add them to the buffer */
  for (i = 0; i < count; ++i) {
    if (rlGetSystemByteOrder() == 1) {
      rlSwapData(&data_ptr[i * size], &data[i * size], size);
    }
    else {
      memcpy(&data_ptr[i * size], &data[i * size], size);
    }
  }

  buffer->size += count * size;
  return offset + count * size;
}

unsigned int rlBufferRead(const rlBuffer *buffer, unsigned int offset, void* recvData, unsigned int count, unsigned int size) {
  unsigned char* data = (unsigned char*)recvData;
  unsigned int i = 0;

  /* For each of the new data items, swap the endianness and read them from the buffer */
  for (i = 0; i < count; ++i) {
    if (rlGetSystemByteOrder() == 1) {
      rlSwapData(&data[i * size], &buffer->data[(i * size) + offset], size);
    }
    else {
      memcpy(&data[i * size], &buffer->data[(i * size) + offset], size);
    }
  }

  return offset + (count * size);
}

unsigned int rlSendBufferData(rlSocket theSocket, const rlBuffer* buffer) {
  int sendSize = buffer->size;
  
  /* sendSize needs to go across in network byte order, swap it if we're little endian */
  if (rlGetSystemByteOrder() == 1) {
    rlSwapData(&sendSize, &buffer->size, sizeof(int));
  }
  
  rlSendData(theSocket, &sendSize, sizeof(int));
  rlSendData(theSocket, buffer->data, buffer->size);

  return buffer->size; /* Returns payload size, not actual data sent ! */
}

unsigned int rlRecvBufferData(rlSocket theSocket, rlBuffer* buffer) {
  int recvSize = 0;
  rlRecvData(theSocket, &recvSize, sizeof(int));
  /* recvSize came across in network byte order, swap it if we're little endian */
  if (rlGetSystemByteOrder() == 1) {
    rlSwapData(&buffer->size, &recvSize, sizeof(int));
  }
  else {
    buffer->size = recvSize;
  }

  rlBufferReserve(buffer, buffer->size);
  rlRecvData(theSocket, buffer->data, buffer->size);

  return buffer->size; /* Returns payload size, not actual data received ! */
}

/* Utilities */
int rlGetSystemByteOrder() {
  /*
    Endian will be 1 when we are on a little endian machine,
    and not 1 on a big endian machine.
  */

  const int one = 1;
  const char endian = *(char*)&one;

  return endian;
}

void rlSwapData(void* out, const void* in, const unsigned int size) {
  const unsigned char *src = (const unsigned char *)in;
  unsigned char *dst = (unsigned char *)out;
  unsigned int i = 0;

  for (i = 0; i < size; ++i) {
    dst[i] = src[size-i];
  }
}

rlSocket rlWaitForConnection(const char *address, const short port, const int retryTimeout) {
  rlSocket theConnection = 0;
  int isConnected = -1;

  while(isConnected == -1) {
    theConnection = rlOpen(port);
    assert(rlIsValidSocket(theConnection));
    isConnected = rlConnect(theConnection, address, port);
    if (isConnected == -1) { 
      rlClose(theConnection);
      sleep(retryTimeout);
    }
  }

  return theConnection;
}

void rlCopyADTToBuffer(const RL_abstract_type* src, rlBuffer* dst) {
  const int headerSize = sizeof(unsigned int) * 2;
  const int intSize    = src->numInts * sizeof(int);
  const int doubleSize = src->numDoubles * sizeof(double);
  int offset = 0;

  rlBufferReserve(dst, headerSize + intSize + doubleSize);

  /*  fprintf(stderr, "send 1 offset = %d\n", offset); */
  offset = rlBufferWrite(dst, offset, &src->numInts, 1, sizeof(unsigned int));
  /*  fprintf(stderr, "send 2 offset = %d\n", offset); */
  offset = rlBufferWrite(dst, offset, &src->numDoubles, 1, sizeof(unsigned int));
  /*  fprintf(stderr, "send 3 offset = %d\n", offset); */

  if (src->numInts > 0) {
    offset = rlBufferWrite(dst, offset, src->intArray, src->numInts, sizeof(int));
  }

  if (src->numDoubles > 0) {
    offset = rlBufferWrite(dst, offset, src->doubleArray, src->numDoubles, sizeof(double));  
  }
}

void rlCopyBufferToADT(const rlBuffer* src, RL_abstract_type* dst) {
  int numInts    = 0;
  int numDoubles = 0;

  int* intArray = 0;
  double * doubleArray = 0;

  int offset = 0;

  /*  fprintf(stderr, "recv 1 offset = %d\n", offset); */
  offset = rlBufferRead(src, offset, &numInts, 1, sizeof(unsigned int));
  /*  fprintf(stderr, "recv 2 offset = %d\n", offset); */
  offset = rlBufferRead(src, offset, &numDoubles, 1, sizeof(unsigned int));
  /*  fprintf(stderr, "recv 3 offset = %d\n", offset); */

  if (numInts > dst->numInts) {
    intArray = (int*)calloc(dst->numInts, sizeof(int));
    memcpy(intArray, dst->intArray, dst->numInts * sizeof(int));
    free(dst->intArray);
    dst->intArray = intArray;
  }
  dst->numInts = numInts;

  if (numDoubles > dst->numDoubles) {
    doubleArray = (double*)calloc(dst->numDoubles, sizeof(double));
    memcpy(doubleArray, dst->doubleArray, dst->numDoubles * sizeof(double));
    free(dst->doubleArray);
    dst->doubleArray = doubleArray;
  }
  dst->numDoubles = numDoubles;

  if (dst->numInts > 0) {
    offset = rlBufferRead(src, offset, dst->intArray, dst->numInts, sizeof(int));
  }

  if (dst->numDoubles > 0) {
    offset = rlBufferRead(src, offset, dst->doubleArray, dst->numDoubles, sizeof(double));
  }
}
