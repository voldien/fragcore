#include "rpcPhysicInterface.h"
#include "ProtocolHeaders.h"
#include "rpcCollision.h"
#include "rpcConstraints.h"
#include "rpcRigidBody.h"

using namespace fragcore;

// void RPCPhysicInterface::initAllocate(InitAllocateTableInfo *table) {
// 	table->initphyic.collisionsize = 10;
// 	table->initphyic.constraintssize = 10;
// 	table->initphyic.rigidbodysize = 10;
// }

RPCPhysicInterface::RPCPhysicInterface(IConfig *config) {

	this->dispatcher = new RPCPhysicDispatcher();
	this->interpreter = new RPCPhysicInterpreter();
}
RPCPhysicInterface::RPCPhysicInterface(const RPCPhysicInterface &other) {

	this->dispatcher = new RPCPhysicDispatcher();
	this->interpreter = new RPCPhysicInterpreter();
}
RPCPhysicInterface::~RPCPhysicInterface() {

	/*	*/
	delete this->dispatcher;
	delete this->interpreter;
}

void RPCPhysicInterface::simulate(float timeStep, int maxSubSteps, float fixedTimeStep) {

	PacketSimulate simulate;
	simulate.timeStep = timeStep;
	simulate.maxSubSteps = maxSubSteps;
	simulate.fixedTimeStep = fixedTimeStep;

	this->dispatcher->send(RPCProtocolCommand::eSimulate, sizeof(simulate), &simulate.header, nullptr);
}

void RPCPhysicInterface::sync() {

	PacketSync sync;
	sync.num = 0;

	this->dispatcher->send(RPCProtocolCommand::eSync, sizeof(sync), &sync.header, nullptr);
}

void RPCPhysicInterface::setGravity(const Vector3 &gravity) {

	PacketGravity pgravity;
	pgravity.gravity = gravity;

	this->dispatcher->send(RPCProtocolCommand::eGravity, sizeof(pgravity), &pgravity.header, nullptr);
}

Vector3 RPCPhysicInterface::getGravity() const {

	PacketRequest request;
	request.request = RPCProtocolCommand::eGravity;

	this->dispatcher->send(RPCProtocolCommand::eRequest, sizeof(request), &request.header, nullptr);

	/*	TODO resolve for the connection.	*/
	PacketHeader header;
	PacketGravity gravity;
	/*	TODO	TMP*/

	return gravity.gravity;
}

void RPCPhysicInterface::addRigidBody(RigidBody *body) {
	// this->dispatcher->send(eAddRigidBody, sizeof(RigidBody), body, nullptr);
}
void RPCPhysicInterface::removeRigidBody(RigidBody *body) {
	// this->dispatcher->send(eRemoveRidigBody, sizeof(RigidBody), body, nullptr);
}

void RPCPhysicInterface::addConstraints(Constraints *constraints) {}
void RPCPhysicInterface::removeConstraints(Constraints *constraints) {}

Collision *RPCPhysicInterface::createCollision(const CollisionDesc *desc) {

	// this->dispatcher->send(eCreateCollision, sizeof(*desc), desc, desc);

	// return this->obtainMappedObject<Collision>(nullptr);
}
void RPCPhysicInterface::deleteCollision(Collision *collision) {

	// this->dispatcher->send(eDeleteCollision, sizeof(Collision), collision, nullptr);
}

Constraints *RPCPhysicInterface::createConstraints(const ConstraintsDesc *desc) {
	RPCConstraints *constrain;

	return constrain;
}
void RPCPhysicInterface::deleteConstraints(Constraints *constraints) {}

RigidBody *RPCPhysicInterface::createRigibody(const RigidBodyDesc *desc) {
	RigidBody *rigbody;
	this->dispatcher->send(RPCProtocolCommand::eCreateRigidbody, 0, nullptr);
	return rigbody;
}
void RPCPhysicInterface::deleteRigibody(RigidBody *collision) {
	this->dispatcher->send(RPCProtocolCommand::eDeleteRigidbody, sizeof(*collision), nullptr);
}

bool RPCPhysicInterface::rayTest(const Ray &ray, RayCastHit *hit) {
	this->dispatcher->send(RPCProtocolCommand::eRayTest, 0, nullptr);
}
bool RPCPhysicInterface::raySphereTest(const Ray &ray, RayCastHit *hit) {
	this->dispatcher->send(RPCProtocolCommand::eRayTest, 0, nullptr);
}

void *RPCPhysicInterface::getState(unsigned int *len) { return nullptr; }

const char *RPCPhysicInterface::getVersion() const { return ""; }

extern "C" PhysicInterface *createInternalPhysicInterface(IConfig *config) { return new RPCPhysicInterface(config); }
