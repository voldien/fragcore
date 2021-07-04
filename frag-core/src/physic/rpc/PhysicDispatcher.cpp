#include"physic/rpc/PhysicDispatcher.h"
#include"physic/PhysicInterface.h"
#include<stdarg.h>
using namespace fragcore;

PhysicDispatcher::PhysicDispatcher(void){
	this->connection = nullptr;
}
PhysicDispatcher::PhysicDispatcher(const PhysicDispatcher& other){
	*this = other;
}


void PhysicDispatcher::send(unsigned int sType, unsigned int len, PacketHeader* packet,...){

	va_list vl;
	void* pva;
	packet->uid = connection->getUID();	/*	from connection.	*/

	va_start(vl, packet);


	switch(sType){
	case eGravity:
		break;
	case eSimulate:
		break;
	default:
		break;
	}

	/*	send.	*/
	this->connection->send(packet, len);

	while((pva = va_arg(vl, void*)) != nullptr){
		this->connection->send(pva, len);
	}

	va_end(vl);
}
