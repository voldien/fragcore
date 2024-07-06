#include "rpcPhysicDispatcher.h"
#include "ProtocolHeaders.h"
#include <cstdarg>
using namespace fragcore;

RPCPhysicDispatcher::RPCPhysicDispatcher() {}
RPCPhysicDispatcher::RPCPhysicDispatcher(const RPCPhysicDispatcher &other) { *this = other; }

void RPCPhysicDispatcher::send(RPCProtocolCommand sType, unsigned int len, PacketHeader *packet, ...) {

	va_list vl;
	void *pva;
	// packet->uid = connection->getUID(); /*	from connection.	*/

	va_start(vl, packet);

	switch (sType) {
	case RPCProtocolCommand::eGravity:
		break;
	case RPCProtocolCommand::eSimulate:
		break;
	default:
		break;
	}

	/*	send.	*/
	// this->connection->send(packet, len);

	while ((pva = va_arg(vl, void *)) != nullptr) {
		// this->connection->send(pva, len);
	}

	va_end(vl);
}
