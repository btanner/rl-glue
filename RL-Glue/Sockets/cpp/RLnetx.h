#ifndef RLnetx_h
#define RLnetx_h

#include <cassert>
#include <algorithm>
#include "RLnet.h"

#define RLnet_BigEndian 0
#define RLnet_LittleEndian 1

template <class T> 
T& rlxToNetworkByteOrder(T& theData)
{
  if (rlGetSystemByteOrder() == RLnet_LittleEndian)
  {
    unsigned char* c = reinterpret_cast<unsigned char*>(&theData);
    std::reverse(c, c + sizeof(theData));
  }
  return theData;
}

template <class T> 
T& rlxToSystemByteOrder(T& theData)
{
  if (rlGetSystemByteOrder() == RLnet_LittleEndian)
  {
    unsigned char* c = reinterpret_cast<unsigned char*>(&theData);
    std::reverse(c, c + sizeof(theData));
  }
  return theData;
}

template <class T> 
int rlxSendType(rlSocket theSocket, T& theData, int theSize = 1) 
{
  /* Preconditions */
  assert(theSize == 1);
  rlxToNetworkByteOrder(theData);
  return rlSendData(theSocket, (char*)&theData, sizeof(T));
}

template <class T> 
int rlxSendType(rlSocket theSocket, T* theData, int theSize)
{
  for (int i = 0; i < theSize; ++i)
    rlxToNetworkByteOrder(theData[i]);

  return rlSendData(theSocket, (char*)theData, sizeof(T) * theSize);
}

template <class T> 
int rlxRecvType(rlSocket theSocket, T& theData, int theSize = 1)
{
  assert(theSize == 1);

  int theResult = rlRecvData(theSocket, (char*)&theData, sizeof(T));
  rlxToSystemByteOrder(theData);

  return theResult;
}
  
template <class T> 
int rlxRecvType(rlSocket theSocket, T* theData, int theSize)
{
  int theResult = rlRecvData(theSocket, (char*)theData, sizeof(T) * theSize);

  for (int i = 0; i < theSize; ++i)
    rlxToSystemByteOrder(theData[i]);  

  return theResult;
}

#endif
