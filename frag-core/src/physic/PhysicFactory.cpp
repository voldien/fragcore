#include <Core/Log.h>
#include <Exception/InvalidArgumentException.h>
#include"Physic/PhysicFactory.h"
#include"Core/Library.h"
#include"Utils/StringUtil.h"
using namespace fragcore;

/**
 *	Internal function data type for creating physic
 *	interface.
 */
typedef PhysicInterface* (*pcreateInternalPhysicInterface)(IConfig* overrideOption);

/*	TODO update with the new naming.	*/
const char *bulletlibpath = "libfragcore-pbu.so";             /*	Default bullet library.	*/
const char *bullet3libpath = "libfragcore-pbu.so";			  /*	Bullet library with OpenCL support.	*/
const char *physxlibpath = "libfragcore-pbu.so";			  /*	Nvidia's physic library. ( Not supported ) */

PhysicInterface* PhysicFactory::createPhysic(PhysicAPI api,  IConfig* overrideOption) {
	return PhysicFactory::createPhysic(PhysicFactory::getInterfaceLibraryPath(api), overrideOption);
}

PhysicInterface* PhysicFactory::createPhysic(const char* libpath, IConfig* config) {

	const char* funcsymbol = "createInternalPhysicInterface";
	pcreateInternalPhysicInterface pfunc = NULL;
	PhysicInterface* interface = NULL;
	Library library;

	/*	Validate parameters.	*/
	if(libpath == NULL)
		throw InvalidArgumentException(fvformatf("Invalid filepath do dynamic library: %s", libpath));

	/*	Open dynamicInterface library and create dynamicInterface object.	*/
	library.open(libpath);
	if(library.isValid()){
		pfunc = (pcreateInternalPhysicInterface)library.getfunc(funcsymbol);

		if(pfunc){
			interface = pfunc(config);
		}else{
			/*	Error	*/
			Log::log("Couldn't find symbol %s in %s.\n", funcsymbol, libpath);
		}
	}else{
		Log::error("Failed loading %s library for creating physic dynamicInterface.\n", libpath);
	}

//	if(connection != NULL){
//
//		/*	Create RPC physic dynamicInterface.	*/
//		/*	Assign connection.	*/
//		PacketPhysicAPIRequest apirequest;
//		apirequest.offset = sizeof(apirequest);
//		apirequest.type = -1;
//		apirequest.pathlen = strlen(libpath);
//		//connection->sendPacket(ProtocolCommand::ePhysicAPIRequest, &apirequest, sizeof(PacketPhysicAPIRequest));
//		connection->send(libpath, apirequest.pathlen);
//
//	}else

	/*	Check.	*/
//	if(interface != NULL){
//		interface->setResource(resources);
//		if(interface->dispatcher != NULL){
//			interface->dispatcher->connection = connection;
//		}
//	}else{
//		Debug::log("Failed to create physic dynamicInterface.\n");
//	}

	return interface;
}

const char* PhysicFactory::getInterfaceLibraryPath(PhysicAPI api){
#ifdef FV_UNIX
	switch(api){
	case PhysicFactory::Bullet:
		return bulletlibpath;
	case PhysicFactory::Bullet3:
		return bullet3libpath;
	case PhysicFactory::PhysX:
		return physxlibpath;
	default:
		throw InvalidArgumentException("");
	}
#else
#endif
}
