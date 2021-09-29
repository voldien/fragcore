#include "Core/Network/NetSocket.h"
using namespace fragcore;

NetSocket::NetSocket(const IPInterface &ip) {}

NetSocket::~NetSocket(void) {
	/*Make sure it gets disconnected.   */
	close();
}
