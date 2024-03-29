#include "PhysicFactory.h"
#include "Core/Library.h"

using namespace fragcore;

/**
 *	Internal function data type for creating physic
 *	interface.
 */
typedef PhysicInterface *(*pcreateInternalPhysicInterface)(IConfig *overrideOption);

/*	TODO update with the new naming.	*/
const char *bulletlibpath = "libfragcore-pbu.so"; /*	Default bullet library.	*/
const char *physxlibpath = "libfragcore-pbu.so";  /*	Nvidia's physic library. ( Not supported ) */

PhysicInterface *PhysicFactory::createPhysic(PhysicAPI api, IConfig *overrideOption) {
	return PhysicFactory::createPhysic(PhysicFactory::getInterfaceLibraryPath(api), overrideOption);
}

PhysicInterface *PhysicFactory::createPhysic(const char *libpath, IConfig *config) {

	const char *funcsymbol = "createInternalPhysicInterface";
	pcreateInternalPhysicInterface pfunc = nullptr;
	PhysicInterface *interface = nullptr;
	Library library;

	/*	Validate parameters.	*/
	if (libpath == nullptr)
		throw InvalidArgumentException("Invalid filepath do dynamic library: {}", libpath);

	/*	Open dynamicInterface library and create dynamicInterface object.	*/
	library.open(libpath);
	if (library.isValid()) {
		pfunc = (pcreateInternalPhysicInterface)library.getfunc(funcsymbol);

		if (pfunc) {
			interface = pfunc(config);
		} else {
			/*	Error	*/
		//	Log::log("Couldn't find symbol %s in %s.\n", funcsymbol, libpath);
		}
	} else {
		//Log::error("Failed loading %s library for creating physic dynamicInterface.\n", libpath);
	}

	//	if(connection != nullptr){
	//
	//		/*	Create RPC physic dynamicInterface.	*/
	//		/*	Assign connection.	*/
	//		PacketPhysicAPIRequest apirequest;
	//		apirequest.offset = sizeof(apirequest);
	//		apirequest.type = -1;
	//		apirequest.pathlen = strlen(libpath);
	//		//connection->sendPacket(RPCProtocolCommand::ePhysicAPIRequest, &apirequest,
	//sizeof(PacketPhysicAPIRequest)); 		connection->send(libpath, apirequest.pathlen);
	//
	//	}else

	/*	Check.	*/
	//	if(interface != nullptr){
	//		interface->setResource(resources);
	//		if(interface->dispatcher != nullptr){
	//			interface->dispatcher->connection = connection;
	//		}
	//	}else{
	//		Debug::log("Failed to create physic dynamicInterface.\n");
	//	}

	return interface;
}

const char *PhysicFactory::getInterfaceLibraryPath(PhysicAPI api) {
#ifdef FV_UNIX
	switch (api) {
	case PhysicFactory::PhysicAPI::Bullet:
		return bulletlibpath;
	case PhysicFactory::PhysicAPI::PhysX:
		return physxlibpath;
	default:
		throw InvalidArgumentException("");
	}
#else
#endif
}
