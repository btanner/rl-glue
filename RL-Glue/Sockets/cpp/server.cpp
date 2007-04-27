#include <iostream>
#include <cassert>
#include "RLnet.h"
#include "RLnetx.h"

int main(int argc, char ** argv)
{
  const char* theMessage = "Hello World!";
  int theMessageLength = 13;
  int theIntValues[3] = { 0, 1, 2 };
  float theFloatValues[3] = { 3.4f, 4.5f, 5.6f };
  double theDoubleValues[3] = { 6.7, 8.9, 10.11 };

  rlSocket theClient;
  rlSocket theSocket = rlOpen(4096);
  
  assert(rlIsValidSocket(theSocket));
  assert(rlListen(theSocket) >= 0);

  theClient = rlAcceptConnection(theSocket);
  assert(rlIsValidSocket(theSocket));

  std::cerr << "Server: " << theMessage << std::endl;

  for (int i = 0; i < 3; ++i)
    std::cerr << "Server: " << theIntValues[i] << std::endl;

  for (int i = 0; i < 3; ++i)
    std::cerr << "Server: " << theFloatValues[i] << std::endl;

  for (int i = 0; i < 3; ++i)
    std::cerr << "Server: " << theDoubleValues[i] << std::endl;

  rlSendData(theClient, theMessage, theMessageLength);
  rlxSendType(theClient, theIntValues, 3);
  rlxSendType(theClient, theFloatValues, 3);
  rlxSendType(theClient, theDoubleValues, 3);

  rlClose(theClient);
  rlClose(theSocket);

  return 0;
}
