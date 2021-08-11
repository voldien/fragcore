#include"physic/PhysicInterface.h"
#include"network/Network.h"
#include"Resources.h"

PhysicDispatcher* dispatcher;		/*	*/
PhysicInterpreter* interpreter;		/*	*/

void PhysicInterface::initAllocate(InitAllocateTableInfo* table){
	table->initphyic.collisionsize = 10;
	table->initphyic.constraintssize = 10;
	table->initphyic.rigidbodysize = 10;
}

PhysicInterface::PhysicInterface(Config* config){
	this->resources = nullptr;
	this->dispatcher = new PhysicDispatcher();
	this->interpreter = new PhysicInterpreter();
}
PhysicInterface::PhysicInterface(const PhysicInterface& other){
	this->resources = nullptr;
	this->dispatcher = new PhysicDispatcher();
	this->interpreter = new PhysicInterpreter();
}
PhysicInterface::~PhysicInterface(void){

	/*	*/
	delete this->dispatcher;
	delete this->interpreter;
}

void PhysicInterface::simulate(float timeStep, int maxSubSteps, float fixedTimeStep){

	PacketSimulate simulate;
	simulate.timeStep = timeStep;
	simulate.maxSubSteps = maxSubSteps;
	simulate.fixedTimeStep = fixedTimeStep;

	this->dispatcher->send(eSimulate, sizeof(simulate), &simulate.header, nullptr);
}

void PhysicInterface::sync(void){

	PacketSync sync;
	sync.num = 0;

	this->dispatcher->send(eSync, sizeof(sync), &sync.header, nullptr);
}

void PhysicInterface::setGravity(const Vector3& gravity){

	PacketGravity pgravity;
	pgravity.gravity = gravity;

	this->dispatcher->send(eGravity, sizeof(pgravity), &pgravity.header, nullptr);
}

Vector3 PhysicInterface::getGravity(void)const{

	PacketRequest request;
	request.request = eGravity;

	this->dispatcher->send(eRequest, sizeof(request), &request.header, nullptr);

	/*	TODO resolve for the connection.	*/
	PacketHeader header;
	PacketGravity gravity;
	/*	TODO	TMP*/

	return gravity.gravity;
}

void PhysicInterface::addRigidBody(RigidBody* body){
	//this->dispatcher->send(eAddRigidBody, sizeof(RigidBody), body, nullptr);
}
void PhysicInterface::removeRigidBody(RigidBody* body){
	//this->dispatcher->send(eRemoveRidigBody, sizeof(RigidBody), body, nullptr);
}

void PhysicInterface::addConstraints(Constraints* constraints){}
void PhysicInterface::removeConstraints(Constraints* constraints){}


Collision* PhysicInterface::createCollision(const CollisionDesc* desc){

	//this->dispatcher->send(eCreateCollision, sizeof(*desc), desc, desc);


	return this->obtainMappedObject<Collision>(nullptr);
}
void PhysicInterface::deleteCollision(Collision* collision){

	//this->dispatcher->send(eDeleteCollision, sizeof(Collision), collision, nullptr);
}


Constraints* PhysicInterface::createConstraints(const ConstraintsDesc* desc){
	Constraints* constrain;

	return constrain;
}
void PhysicInterface::deleteConstraints(Constraints* constraints){

}


RigidBody* PhysicInterface::createRigibody(const RigidBodyDesc* desc){
	RigidBody* rigbody;
	this->dispatcher->send(eCreateRigidbody, 0, nullptr);
	return rigbody;
}
void PhysicInterface::deleteRigibody(RigidBody* collision){
	this->dispatcher->send(eDeleteRigidbody, sizeof(*collision), nullptr);
}


bool PhysicInterface::rayTest(const Ray& ray, RayCastHit* hit){
	this->dispatcher->send(eRayTest, 0, nullptr);
}
bool PhysicInterface::raySphereTest(const Ray& ray, RayCastHit* hit){
	this->dispatcher->send(eRayTest, 0, nullptr);
}






void* PhysicInterface::getState(unsigned int* len){
	return nullptr;
}


const char* PhysicInterface::getVersion(void)const{
	return PV_STR_VERSION(1,0,0);
}
const char* PhysicInterface::getName(void)const{
	return "RPC Physic";
}



extern "C" PhysicInterface* createInternalPhysicInterface(Config* config){
	return new PhysicInterface(config);
}
