#include <iostream>
#include <cassert>
#include <stdexcept>
#include "RLnet.h"
#include "RLnetx.h"

int main(int argc, char** argv)
{
  rlSocket theClient;
  char theMessage[256] = {0};
  int theIntValues[3] = {0};
  float theFloatValues[3] = {0};
  double theDoubleValues[3] = {0};

  if (argc != 2)
    throw std::runtime_error("Usage: client <ip address>");

  theClient = rlOpen(4096);
  assert(rlIsValidSocket(theClient));

  assert(rlConnect(theClient, argv[1]) >= 0);
  assert(rlRecvData(theClient, theMessage, 13) >= 0);
  assert(rlxRecvType(theClient, theIntValues, 3) >= 0);
  assert(rlxRecvType(theClient, theFloatValues, 3) >= 0);
  assert(rlxRecvType(theClient, theDoubleValues, 3) >= 0);
  assert(rlClose(theClient) >= 0);

  std::cerr << "Client: " << theMessage << std::endl;

  for (int i = 0; i < 3; ++i)
    std::cerr << "Client: " << theIntValues[i] << std::endl;

  for (int i = 0; i < 3; ++i)
    std::cerr << "Client: " << theFloatValues[i] << std::endl;

  for (int i = 0; i < 3; ++i)
    std::cerr << "Client: " << theDoubleValues[i] << std::endl;

  return 0;
}
