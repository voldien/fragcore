/*
 *	FragCore - Core Framework Functionalities for Game Engines
 *	Copyright (C) 2018  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _FRAG_CORE_RPC_PROTOCOL_HEADERS_H_
#define _FRAG_CORE_RPC_PROTOCOL_HEADERS_H_ 1
#include <Core/Math3D.h>
#include <FragDef.h>

/**
 *	Connection initilization.
 *
 *  Client			Server
 * 	  |				  |
 * 	  | -> connect -> |	Socket connect
 *    |				  |
 *    | <-	init   <- | Server replay with init packet.
 *    |				  |
 *    | -> cliopt ->  | Client send options to use.
 *    |				  |
 *	 [|				  |] Optional section for encryption.
 *	 [|	<- certic  < -|] Send X509 certificate to client
 *	 [|				  |]
 *   [| -> sym-key -> |] Client sends encrypted key from certificate.
 *   [| 			  |]
 *    .				  .
 *    .				  .
 *    .				  .
 *    .				  .
 *    | ->    EOI  -> | End of init process.
 *	  ^^^^^^^^^^^^^^^^^
 *
 *	The end of EOI has to be called before send
 *	packets about what physic interface.
 */
namespace fragcore {
	/**
	 *	Server client protocols.
	 */
	enum class RPCProtocolCommand {
		eInit = 0x1,		/*	Init packet sent to client after TCP connect.	*/
		eCliOpt = 0x2,		/*	Client option selected. */
		eCertificate = 0x3, /*	certificate.	*/
		eSymmetric = 0x4,	/*	Symmetric key exchange.	*/
		eEndOfInit = 0x5,	/*	Ending initialization session.	*/

		/*	Physic dynamicInterface factory.	*/
		ePhysicAPIRequest = 0x7, /*	*/

		/*	*/
		eRequest = 0x8, /*	Request packet.	*/

		/*	Physic dynamicInterface.	*/
		eGravity,  /*	Get and set gravity.	*/
		eSimulate, /*	Simluate the scene.	*/
		eSync,	   /*	Syncronize.	*/
		eRayTest,  /*	Perform ray test.	*/

		/*  */
		eAddRigidBody,	  /*	Add rigidbody to scene.	*/
		eRemoveRidigBody, /*	Remove Rigidibody.	*/

		/*  */
		eCreateCollision, /*	Create collision object.	*/
		eDeleteCollision, /*	Delete collision object.	*/
		eCreateRigidbody, /*	Create ridigbody object.	*/
		eDeleteRigidbody, /*	*/
		eCreateCharacterController,
		eDeleteCharacterController,

		/*	PhysicInterface object mass.	*/
		eGetMass, /*	Get mass of object.	*/
		eSetMass, /*	Set mass of object.	*/
		eSetPosition,
		eGetPosition,
		eSetOritention,
		eGetOriention,
		eSetScale,
		eGetScale,
		eSetDamp,
		eGetDamp,
		eSetAngularDamp,
		eGetAngularDamp,

		/*	*/
		ePacketBroadcastInfo,
	};

	/**
	 *	Debug symbol for the application
	 *	protocol command.
	 */
	extern const char *gc_proto_symbol[];

	/**
	 *	Protocol header.
	 */
	/*	TODO resolve the packet name in order to add additional packets other than physic related communication.	*/
	/*	TODO make sure that the packet header gets included in the actual packet instead of two different one.*/
	typedef struct packet_header_t {
		uint16_t version; /*	version.	*/
		uint16_t sType;	  /*	Packet type.	*/
		uint8_t offset;	  /*	packet header offset, in order to add additional attributes without causing segmentation
							 violation.	*/
		uint32_t len;	  /*	Size of the whole packet. that includes the size of the header. */
		uint16_t uid;	  /*	UID to identify what client sent the packet.	*/
	} PacketHeader;

	/**
	 *	Initialization packet.
	 */
	typedef struct packet_init_info_t {
		PacketHeader header; /*	Header.	*/
		uint32_t version;	 /*	version of the server.	*/
		uint32_t sym;		 /*	Symmetric cipher supported on the server.	*/
		uint32_t asym;		 /*	Asymmetric cipher supported on the server.	*/
		uint32_t comp;		 /*	Compression supported on the server.	*/
		uint32_t exts;		 /*	Extension supported on the server.	*/
	} PacketInitInfo;

	/**
	 *
	 */
	typedef struct packet_client_option_t {
		PacketHeader header; /*	Header.	*/
		uint32_t sym;		 /*	Symmetric cipher decided by the client.	*/
		uint32_t asym;		 /*	Asymmetric cipher decided by the client in the certifcate.	*/
		uint32_t comp;		 /*	Compressed decided by the client.	*/
		uint32_t exts;		 /*	Extension selected.	*/
	} PacketClientOption;

	/**
	 *
	 */
	typedef struct packet_certificate_t {
		PacketHeader header;
		uint32_t type;
		uint8_t certifcate[1000];
	} PacketCertificate;

	/**
	 *
	 */
	typedef struct packet_symmetric_key_t {
		PacketHeader header;
		int32_t keylen;
		uint32_t key[1024]; /*	asymmetric encrypted symmetric key.	*/
	} PacketSymmetricKey;

	/**
	 *	Packet sent for requesting specific information
	 */
	typedef struct packet_request_t {
		PacketHeader header; /*	header.	*/
		RPCProtocolCommand request;	 /*	request	information enum.	*/
	} PacketRequest;		 /*	Packet for requesting particular data.	*/

	/**
	 *	Packet for requesting what physic interface to be
	 *	used.
	 */
	typedef struct packet_physic_engine_type_t {
		PacketHeader header; /*	header.	*/
		uint32_t type;		 /*	enum type from PhysicFactory::PhysicAPI. if equal -1 its a custom library name.*/
		uint8_t offset;		 /*	Packet offset */
		uint32_t pathlen;	 /*	physic engine type library name character length.*/
	} PacketPhysicAPIRequest;

	/**
	 *	Packet with gravity information
	 *	associate with the physic interface.
	 */
	typedef struct packet_gravity_t {
		PacketHeader header; /*	Header.	*/
		Vector3 gravity;	 /*	Gravity.	*/
	} PacketGravity;

	/**
	 *
	 */
	typedef struct packet_simulate_t {
		PacketHeader header;  /*	Header.	*/
		float timeStep;		  /*	*/
		uint32_t maxSubSteps; /*	*/
		float fixedTimeStep;  /*	*/
	} PacketSimulate;

	/**
	 *
	 */
	typedef struct packet_sync_t {
		PacketHeader header; /*	*/
		uint32_t num;		 /*	number of indivual specific objects.	*/
		uint32_t typeSize;	 /*	*/
	} PacketSync;

	/**
	 *
	 */
	typedef struct packet_sync_objects_t {
		PacketHeader header; /*	*/
		uint32_t num;
	} PacketSyncObjects;

	/**
	 *	Broadcast information packet.
	 *	Used by the server for broadcasting
	 *	about the server on the network.
	 */
	typedef struct packet_broadcast_info_t {
		PacketHeader header; /*	Protocol header.	*/
		uint32_t listenport; /*	Port the server is listening too.	*/
		uint32_t ip[128];	 /*	PI address of the server.	*/
		uint32_t status;	 /*	status of the server.	*/

	} PacketBroadCastInfo;

	/**
	 *
	 */
	typedef struct packet_uniform_t {
		union {
			struct {
				PacketHeader header;
				char buf[1500 - sizeof(PacketHeader)];
			};
			char packbuf[1500];
		};

	} PacketUniform;
} // namespace fragcore

#endif
