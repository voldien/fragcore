#define USE_PHYSX

#undef None
#undef Success;
#include"Def.h"
#include"physic/physx/internal_object_type.h"
#include"Exception/RuntimeException.h"
#include <physx/PxPhysicsAPI.h>
#include <physx/cooking/PxCooking.h>
#include <physx/extensions/PxExtensionsAPI.h>
#include <physx/foundation/PxFoundation.h>
#include <physx/pxtask/PxCudaContextManager.h>
#include <physx/vehicle/PxVehicleSDK.h>

#ifdef FV_DEBUG
	#pragma comment(lib, "PhysX3CHECKED_x86.lib")
	#pragma comment(lib, "PhysX3CommonCHECKED_x86.lib")
	#pragma comment(lib, "PhysX3ExtensionsCHECKED.lib")
	#pragma comment(lib, "PhysX3DEBUG_x86.lib")
	#pragma comment(lib, "PhysX3CookingCHECKED_x86.lib")
	#pragma comment(lib, "PhysXProfileSDKCHECKED.lib")
	#pragma comment(lib, "PhysXVisualDebuggerSDKCHECKED.lib")
	#pragma comment(lib, "PxTaskCHECKED.lib")
	#pragma comment(lib, "PhysX3CharacterKinematicCHECKED_x86.lib")
	#pragma comment(lib, "PhysX3VehicleCHECKED.lib")
	#pragma comment(lib, "PhysX3VehiclePROFILE.lib")
	#pragma comment(lib, "PxTaskPROFILE.lib")
#elif FV_RELEASE
	#pragma comment(lib, "PhysX3_x86.lib")
	#pragma comment(lib, "PhysX3Common_x86.lib")
	#pragma comment(lib, "PhysX3Extensions.lib")
	#pragma comment(lib, "PhysX3DEBUG_x86.lib")
	#pragma comment(lib, "PhysX3Cooking_x86.lib")
	#pragma comment(lib, "PhysXProfileSDK.lib")
	//#pragma comment(lib,"PhysXVisualDebuggerSDK.lib")
	#pragma comment(lib, "PxTask.lib")
	#pragma comment(lib, "PhysX3CharacterKinematic_x86.lib")
	#pragma comment(lib, "PhysX3Vehicle.lib")
	//#pragma comment(lib, "PhysX3VehiclePROFILE.lib")
	#pragma comment(lib, "PxTaskPROFILE.lib")
#else
#endif

#if defined(USE_PHYSX)
	#ifdef FV_WINDOWS
	#define PX_WINDOWS
	#endif
	#define NVIDIA_PHYSX
	#define physx physx
	#undef None
	#undef Success;
	#include <physx/PxPhysicsAPI.h>
#endif

using namespace physx;
using namespace physx::profile;
using namespace physx::debugger::comm;
using namespace physx::debugger;
using namespace std;
using namespace fragcore;

PhysicInterface::PhysicInterface(IConfig *config) {
	this->setName("PhysX");
	/*	*/
	PhysicPhysxCore *physicore = new PhysicPhysxCore();
	this->pdata = physicore;
	assert(physicore);

	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;

	physicore->gFoundation = PxCreateFoundation(
		PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	if (!physicore->gFoundation)
	{
		throw RuntimeException("Failed to Create Foundation");
	}

	engine.physic.gProfileManager =
		&PxProfileZoneManager::createProfileZoneManager(
			engine.physic.gFoundation);

	if (!engine.physic.gProfileManager)
	{
		throw RuntimeException("Failed to Create Profile Manager");
	}

	if (!(engine.physic.gPhysic = PxCreatePhysics(
			  PX_PHYSICS_VERSION, *engine.physic.gFoundation,
			  PxTolerancesScale(), false, engine.physic.gProfileManager)))
	{
		throw RuntimeException("Failed to Create VDPhysic | \n");
	}

	if (!PxInitExtensions(*engine.physic.gPhysic))
	{
		throw RuntimeException("Failed to initialize Nvidia PhysX extensions.\n");
	}

	engine.physic.gCooking =
		PxCreateCooking(PX_PHYSICS_VERSION, *engine.physic.gFoundation,
						PxCookingParams(PxTolerancesScale()));
	if (!engine.physic.gCooking)
	{
		throw RuntimeException("Failed to create PhysX Cooking |\n");
	}

	/**/
	PxCudaContextManagerDesc cudaDesc;
	cudaDesc.interopMode = PxCudaInteropMode::OGL_INTEROP;

	cudaDesc.graphicsDevice = ExGetCurrentOpenGLContext();
	cudaDesc.appGUID = "OpenGLEngineCuda";
	int getCudeSugge = PxGetSuggestedCudaDeviceOrdinal(gDefaultErrorCallback);
	engine.physic.gCudaContextManager = PxCreateCudaContextManager(
		*engine.physic.gFoundation, cudaDesc, engine.physic.gProfileManager);
	if (!engine.physic.gCudaContextManager)
	{
		throw RuntimeException(
			"Failed to create Nvidia Cuda Context from OpenGL Context.\n");
	}

	if (initialzeScene())
	{
		VDPhysic::createMaterial(VDPhysicMaterial::eDefault);
		engine.physic.gControlManager =
			PxCreateControllerManager(*engine.physic.gPhysxScene);
		if (engine.physic.gControlManager)
		{
			engine.physic.gPhysiccontrollerreport =
				new PhysicUserControllerHitReport();
		}
		else
		{
			throw RuntimeException("Failed to Create PHYSX Controller Manager\n");
		}
	}

	VDPhysic::initVehicleSDK();
	initDebug();
}

PhysicInterface::PhysicInterface(const PhysicInterface &other)
{
	//	*this = other;
}
PhysicInterface::~PhysicInterface(void)
{
	engine.physic.gPhysxScene->fetchResults(true);
	engine.physic.gPhysxScene->flushQueryUpdates();

	if (VDPhysic::getPhysicHandle())
	{
		delete VDPhysic::getPhysicHandle();
		engine.physic.gPhysic = nullptr;
	}

	if (engine.physic.gControlManager)
	{
		engine.physic.gControlManager->release();
		engine.physic.gControlManager = nullptr;
	}
	if (engine.physic.gPhysxScene)
	{
		engine.physic.gPhysxScene->release();
		engine.physic.gPhysxScene = nullptr;
	}
	if (engine.physic.gCooking)
	{
		engine.physic.gCooking->release();
		engine.physic.gCooking = nullptr;
	}
	if (engine.physic.gFoundation)
	{
		engine.physic.gFoundation->release();
		engine.physic.gFoundation = nullptr;
	}
	if (engine.physic.gCudaContextManager)
	{
		engine.physic.gCudaContextManager->releaseContext();
	}

	PxCloseVehicleSDK();
	PxCloseExtensions();
}

void PhysicInterface::OnInitialization(void)
{
}
void PhysicInterface::OnDestruction(void)
{
}

void PhysicInterface::simulate(float timeStep, int maxSubSteps, float fixedTimeStep)
{
	PhysicCore *physicore = (PhysicCore *)this->pdata;

	engine.physic.gPhysxScene->simulate(
		(VDTime::deltaTime() + 0.0000000001f));  //(_time * 0.000001f));

	if (engine.physic.gPhysxScene->fetchResults()) {
		/*
while(!gPhysxScene->fetchResults()){
		continue;
}
		 */

		/* send feedback;	*/
		m_simulationEventcallBack->EventonContact();
		m_simulationEventcallBack->EventonTrigger();
		m_simulationEventcallBack->EventonConstraintBreak();

		update();
	}
}

void PhysicInterface::sync(void)
{
	PhysicCore *physicore = (PhysicCore *)this->pdata;
	assert(physicore);

	physicore->dynamicsWorld->synchronizeMotionStates();
}

void PhysicInterface::setGravity(const Vector3 &gravity)
{
	PhysicCore *physicore = (PhysicCore *)this->pdata;
	assert(physicore);

	btVector3 grav = btVector3(gravity.x(), gravity.y(), gravity.z());
	physicore->dynamicsWorld->setGravity(*(btVector3 *)&gravity);
}

Vector3 PhysicInterface::getGravity(void) const
{
	PhysicCore *physicore = (PhysicCore *)this->pdata;

	btVector3 gr = physicore->dynamicsWorld->getGravity();
	return Vector3(gr.x(), gr.y(), gr.z());
}

physx::PxFilterFlags PxCustomFilterShader(
	physx::PxFilterObjectAttributes attributes0,
	physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1,
	physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags,
	const void* constantBlock, physx::PxU32 constantBlockSize);

/* Get User Controller hit Report  */
class PhysicUserControllerHitReport : public physx::PxUserControllerHitReport {
   public:
	virtual void onShapeHit(const physx::PxControllerShapeHit& hit);
	virtual void onControllerHit(const physx::PxControllersHit& hit);
	virtual void onObstacleHit(const physx::PxControllerObstacleHit& hit);
};

/* Simulation Event CallBack  */
class SimulationEventCallback : public physx::PxSimulationEventCallback {
   public:
	SimulationEventCallback(void) {
		memset(&this->dataBreak, 0, sizeof(ConstraintBreak));
		memset(&this->dataContact, 0, sizeof(Contact));
		memset(&this->dataTrigger, 0, sizeof(Trigger));
	}
	virtual void onConstraintBreak(physx::PxConstraintInfo* constraints,
								   PxU32 count);
	virtual void onWake(physx::PxActor** actors, PxU32 count);
	virtual void onSleep(physx::PxActor** actors, PxU32 count);
	virtual void onContact(const physx::PxContactPairHeader& pairHeader,
						   const physx::PxContactPair* pairs, PxU32 nbPairs);
	virtual void onTrigger(physx::PxTriggerPair* pairs, PxU32 count);
	struct ConstraintBreak {
		physx::PxConstraintInfo* constraints;
		physx::PxU32 count;

	} dataBreak;
	struct Contact {
		// const physx::PxContactPairHeader& pairHeader;
		physx::PxContactPair* pairs;
		physx::PxU32 nbPair;
	} dataContact;
	struct Trigger {
		physx::PxTriggerPair* pairs;
		physx::PxU32 count;
	} dataTrigger;

	void VDAPIENTRY EventonConstraintBreak(void);
	void VDAPIENTRY EventonWake(void);
	void VDAPIENTRY EventonSleep(void);
	void VDAPIENTRY EventonContact(void);
	void VDAPIENTRY EventonTrigger(void);
};
// extern VDDECLSPEC SimulationEventCallback* m_simulationEventcallBack;
SimulationEventCallback* m_simulationEventcallBack = nullptr;

std::vector<VDPhysicMaterial*> gPhysicMaterialCollection;

void VDPhysic::initialize(void) {
	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;

	engine.physic.gFoundation = PxCreateFoundation(
		PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	if (!engine.physic.gFoundation) {
		throw RuntimeException("Failed to Create VDPhysic Foundation");
	}

	engine.physic.gProfileManager =
		&PxProfileZoneManager::createProfileZoneManager(
			engine.physic.gFoundation);

	if (!engine.physic.gProfileManager) {
	}

	if (!(engine.physic.gPhysic = PxCreatePhysics(
			  PX_PHYSICS_VERSION, *engine.physic.gFoundation,
			  PxTolerancesScale(), false, engine.physic.gProfileManager))) {
		throw RuntimeException("Failed to Create VDPhysic | \n");
	}

	if (!PxInitExtensions(*engine.physic.gPhysic)) {
		throw RuntimeException("Failed to initialize Nvidia PhysX extensions.\n");
	}

	engine.physic.gCooking =
		PxCreateCooking(PX_PHYSICS_VERSION, *engine.physic.gFoundation,
						PxCookingParams(PxTolerancesScale()));
	if (!engine.physic.gCooking) {
		throw RuntimeException("Failed to create PhysX Cooking |\n");
	}

	/**/
	PxCudaContextManagerDesc cudaDesc;
	cudaDesc.interopMode = PxCudaInteropMode::OGL_INTEROP;

	cudaDesc.graphicsDevice = ExGetCurrentOpenGLContext();
	cudaDesc.appGUID = "OpenGLEngineCuda";
	int getCudeSugge = PxGetSuggestedCudaDeviceOrdinal(gDefaultErrorCallback);
	engine.physic.gCudaContextManager = PxCreateCudaContextManager(
		*engine.physic.gFoundation, cudaDesc, engine.physic.gProfileManager);
	if (!engine.physic.gCudaContextManager) {
		throw RuntimeException(
			"Failed to create Nvidia Cuda Context from OpenGL Context.\n");
	}

	if (initialzeScene()) {
		VDPhysic::createMaterial(VDPhysicMaterial::eDefault);
		engine.physic.gControlManager =
			PxCreateControllerManager(*engine.physic.gPhysxScene);
		if (engine.physic.gControlManager) {
			engine.physic.gPhysiccontrollerreport =
				new PhysicUserControllerHitReport();
		} else {
			throw RuntimeException("Failed to Create PHYSX Controller Manager\n");
		}
	}

	VDPhysic::initVehicleSDK();
	initDebug();
}

void VDPhysic::release(void) {
	engine.physic.gPhysxScene->fetchResults(true);
	engine.physic.gPhysxScene->flushQueryUpdates();

	if (VDPhysic::getPhysicHandle()) {
		delete VDPhysic::getPhysicHandle();
		engine.physic.gPhysic = nullptr;
	}

	if (engine.physic.gControlManager) {
		engine.physic.gControlManager->release();
		engine.physic.gControlManager = nullptr;
	}
	if (engine.physic.gPhysxScene) {
		engine.physic.gPhysxScene->release();
		engine.physic.gPhysxScene = nullptr;
	}
	if (engine.physic.gCooking) {
		engine.physic.gCooking->release();
		engine.physic.gCooking = nullptr;
	}
	if (engine.physic.gFoundation) {
		engine.physic.gFoundation->release();
		engine.physic.gFoundation = nullptr;
	}
	if (engine.physic.gCudaContextManager) {
		engine.physic.gCudaContextManager->releaseContext();
	}

	PxCloseVehicleSDK();
	PxCloseExtensions();
}

void VDPhysic::initDebug(const char* pvd_host_ip) {
	// local host
	int port = 5425;
	unsigned int timeOut = 100;

	if (!VDPhysic::getPhysicHandle()->getPvdConnectionManager())
		return; /*	failure	*/

	PxVisualDebuggerConnectionFlags connectionFlag =
		PxVisualDebuggerExt::getAllConnectionFlags();

	PvdConnection* theConnection = PxVisualDebuggerExt::createConnection(
		VDPhysic::getPhysicHandle()->getPvdConnectionManager(), pvd_host_ip,
		port, timeOut, connectionFlag);
	if (theConnection) {
		engine.physic.gPhysic->getVisualDebugger()->setVisualizeConstraints(
			true);
		engine.physic.gPhysic->getVisualDebugger()->setVisualDebuggerFlag(
			PxVisualDebuggerFlag::eTRANSMIT_CONTACTS, true);
		engine.physic.gPhysic->getVisualDebugger()->setVisualDebuggerFlag(
			PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
		engine.physic.gPhysic->getVisualDebugger()->setVisualDebuggerFlag(
			PxVisualDebuggerFlag::eTRANSMIT_CONSTRAINTS, true);
	}

	// if No PVD, Disable all Stream for performance
	if (!theConnection) {
		engine.physic.gPhysic->getVisualDebugger()->disconnect();
	}
}

int VDPhysic::initVehicleSDK(void) {
	if (PxInitVehicleSDK(*engine.physic.gPhysic)) {
		// success!
		PxVehicleSetBasisVectors(PxVec3(0, 1, 0), PxVec3(0, 0, 1));
		PxVehicleSetUpdateMode(PxVehicleUpdateMode::eVELOCITY_CHANGE);
		return SDL_TRUE;
	}
	throw RuntimeException("Failed to Create PhysX Vehicle SDK");
	return SDL_FALSE;
}

int VDPhysic::initialzeScene(void) {
	PxSceneDesc sceneDesc(engine.physic.gPhysic->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0, -9.82f, 0);

	/**/
	if (!sceneDesc.cpuDispatcher) {
		engine.physic.PhysicThreadCount = 2;
		engine.physic.PhysicThreadCount = VDSystemInfo::getCPUCoreCount();
		PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(
			engine.physic.PhysicThreadCount);  // conenction with cpu.
		sceneDesc.cpuDispatcher = mCpuDispatcher;

		/*	*/
		if (!mCpuDispatcher) {
			throw RuntimeException("Failed to Create CPU Dispatcher\n");
		}

		/*	add support for gpu dispatching if supported	*/
		if (mCpuDispatcher && engine.physic.gCudaContextManager) {
			sceneDesc.gpuDispatcher =
				engine.physic.gCudaContextManager->getGpuDispatcher();
		}
	}

	sceneDesc.filterShader = *PxCustomFilterShader;
	m_simulationEventcallBack = new SimulationEventCallback();
	sceneDesc.simulationEventCallback = m_simulationEventcallBack;
	sceneDesc.simulationOrder = PxSimulationOrder::eCOLLIDE_SOLVE;
	sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;
	engine.physic.gPhysxScene =
		VDPhysic::getPhysicHandle()->createScene(sceneDesc);

	if (!engine.physic.gPhysxScene) {
		throw RuntimeException("Failed to create NVidia PhysX Scene!\n");
	}
	if (!(engine.physic.gPhysxScene->getSimulationEventCallback() ==
		  m_simulationEventcallBack)) {
		throw RuntimeException("PhysX simulation callback error.\n");
	}

	// m_physxScene->createClient();
	/*	m_physxScene->setClientBehaviorFlags(0,
					PxClientBehaviorFlag::eREPORT_FOREIGN_OBJECTS_TO_CONTACT_NOTIFY
					|
	   PxClientBehaviorFlag::eREPORT_FOREIGN_OBJECTS_TO_CONSTRAINT_BREAK_NOTIFY
					|
	   PxClientBehaviorFlag::eREPORT_FOREIGN_OBJECTS_TO_TRIGGER_NOTIFY);
			*/

	// Initialize simulation Data
	// This will enable basic visualization of PhysX objects like- actors
	// collision shapes and their axes. The function PxScene::getRenderBuffer()
	// is used to render any active visualization for scene.
	if ((VDEngine::getFlag() &
		 VDEngine::Debug)) { /*	TODO fix! and add VD_DEBUG CHECK	*/

		VDPhysic::getPhysicScene()->setVisualizationParameter(
			PxVisualizationParameter::eSCALE,
			1.0);  // Global visualization scale which gets multiplied with the
				   // individual scales
		VDPhysic::getPhysicScene()->setVisualizationParameter(
			PxVisualizationParameter::eCOLLISION_SHAPES,
			1.0f);  // Enable visualization of actor's shape
		VDPhysic::getPhysicScene()->setVisualizationParameter(
			PxVisualizationParameter::eACTOR_AXES,
			1.0f);  // Enable visualization of actor's axis

		VDPhysic::getPhysicScene()->setVisualizationParameter(
			PxVisualizationParameter::eBODY_LIN_VELOCITY,
			1.0f);  // Enable visualization of actor's axis
		VDPhysic::getPhysicScene()->setVisualizationParameter(
			PxVisualizationParameter::eCOLLISION_COMPOUNDS,
			1.0f);  // Enable visualization of actor's axis

		VDPhysic::getPhysicScene()->setVisualizationParameter(
			PxVisualizationParameter::eCULL_BOX, 1.0f);
		VDPhysic::getPhysicScene()->setVisualizationParameter(
			PxVisualizationParameter::eJOINT_LIMITS, 1.0f);
		VDPhysic::getPhysicScene()->setVisualizationParameter(
			PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
		VDPhysic::getPhysicScene()->setVisualizationCullingBox(
			PxBounds3(PxVec3(-1000), PxVec3(1000)));
	}

	VDPhysic::getPhysicScene()->simulate(0.000000001f);
	while (!VDPhysic::getPhysicScene()->fetchResults()) {
		continue;
	}
	return SDL_TRUE;
}

void VDPhysic::simulation(void) {
	engine.physic.gPhysxScene->simulate(
		(VDTime::deltaTime() + 0.0000000001f));  //(_time * 0.000001f));

	if (engine.physic.gPhysxScene->fetchResults()) {
		/*
while(!gPhysxScene->fetchResults()){
		continue;
}
		 */

		/* send feedback;	*/
		m_simulationEventcallBack->EventonContact();
		m_simulationEventcallBack->EventonTrigger();
		m_simulationEventcallBack->EventonConstraintBreak();

		update();
	}
}

void VDPhysic::update(void) {
	unsigned int x;

	for (x = 0; x < rigidBodyCollection.size(); x++) {
		const PxTransform trans =
			rigidBodyCollection[x]->actor->getGlobalPose();
		rigidBodyCollection[x]->transform()->SetRigidBodyPosition(
			*VDCASTP(const VDVector3*, &trans.p));
		rigidBodyCollection[x]->transform()->SetRigidBodyRotation(
			*(const VDQuaternion*)&trans.q);
	}
}

void VDPhysic::setGravity(const VDVector3& gravity) {
	return VDPhysic::getPhysicScene()->setGravity(*(PxVec3*)&gravity);
}

VDVector3 VDPhysic::getGravity(void) {
	return *(VDVector3*)&VDPhysic::getPhysicScene()->getGravity();
}

bool VDPhysic::isPhysicEnabled(void) {
	return (VDEngine::getFlag() & VDEngine::ePhysic);
}

bool VDPhysic::rayCast(const VDVector3& origin, const VDVector3& direction,
					   float distance) {
	PxRaycastBuffer hit;

	return engine.physic.gPhysxScene->raycast(
		*VDCASTP(const PxVec3*, &origin), *VDCASTP(const PxVec3*, &direction),
		distance, hit);
}

bool VDPhysic::rayCast(const VDVector3& origin, const VDVector3& direction,
					   float distance, VDRaycastHit& hit) {
	PxRaycastBuffer hitpointer;

	engine.physic.gPhysxScene->raycast(*VDCASTP(const PxVec3*, &origin),
									   *VDCASTP(const PxVec3*, &direction),
									   distance, hitpointer);
	hit.hitdata = hitpointer.block;
	return (hitpointer.hasBlock);
}

bool VDPhysic::rayCast(const VDVector3& origin, const VDVector3& direction,
					   VDRaycastHit& hit) {
	PxRaycastBuffer hitpointer;

	engine.physic.gPhysxScene->raycast(*VDCASTP(const PxVec3*, &origin),
									   *VDCASTP(const PxVec3*, &direction),
									   10000, hitpointer);
	hit.hitdata = hitpointer.block;
	return hitpointer.hasBlock;
}

bool VDPhysic::lineCast(const VDVector3& start, const VDVector3& end) {
	PxQueryFilterData fd;
	fd.flags |= PxQueryFlag::eANY_HIT;
	PxRaycastBuffer hit;
	engine.physic.gPhysxScene->raycast(
		*VDCASTP(const PxVec3*, &start),
		*VDCASTP(const PxVec3*, &(start - end).normalize()),
		(start - end).length(), hit);
	return hit.hasBlock;
}

bool VDPhysic::lineCast(const VDVector3& start, const VDVector3& end,
						VDRaycastHit& hit) {
	PxQueryFilterData fd;
	fd.flags |= PxQueryFlag::eANY_HIT;
	PxRaycastBuffer hitpointer;
	engine.physic.gPhysxScene->raycast(
		*VDCASTP(const PxVec3*, &start),
		*VDCASTP(const PxVec3*, &(start - end).normalize()),
		(start - end).length(), hitpointer);
	hit.hitdata = hitpointer.block;
	return hitpointer.hasBlock;
}

int VDPhysic::overlapSphere(const VDVector3& position, float radius,
							VDRaycastHit& hit) {
	return 1;
}

VDPhysicMaterial* VDPhysic::createMaterial(unsigned int materialtemplate) {
	if (!VDPhysic::getPhysicHandle() || !materialtemplate) return nullptr;

	// does the requested Template Material exist?
	if (VDPhysic::findMaterial(materialtemplate)) {  // exists current
		return VDPhysic::findMaterial(materialtemplate);
	} else {
		VDPhysicMaterial* material = new VDPhysicMaterial();
		switch (materialtemplate) {
			case VDPhysicMaterial::eDefault:
				material->m_material =
					VDPhysic::getPhysicHandle()->createMaterial(
						PX_MATERIAL_DEFAULT_VALUE);
				break;
			case VDPhysicMaterial::eIce:
				material->m_material =
					VDPhysic::getPhysicHandle()->createMaterial(
						PX_MATERIAL_ICE_VALUE);
				break;
			case VDPhysicMaterial::eStone:
				material->m_material =
					VDPhysic::getPhysicHandle()->createMaterial(
						PX_MATERIAL_STONE_VALUE);
				break;
			case VDPhysicMaterial::eWood:
				material->m_material =
					VDPhysic::getPhysicHandle()->createMaterial(
						PX_MATERIAL_WOOD_VALUE);
				break;
			default:
				break;
		}
		return material;
	}
}

VDPhysicMaterial* VDPhysic::findMaterial(int materialtemplate) {
	switch (materialtemplate) {
		case VDPhysicMaterial::eDefault:
			return VDPhysic::findMaterial(PX_MATERIAL_DEFAULT_VALUE);
		case VDPhysicMaterial::eIce:
			return VDPhysic::findMaterial(PX_MATERIAL_ICE_VALUE);
		case VDPhysicMaterial::eStone:
			return VDPhysic::findMaterial(PX_MATERIAL_STONE_VALUE);
		case VDPhysicMaterial::eWood:
			return VDPhysic::findMaterial(PX_MATERIAL_WOOD_VALUE);
		default:
			return VDPhysic::findMaterial(PX_MATERIAL_DEFAULT_VALUE);
	}
}

VDPhysicMaterial* VDPhysic::findMaterial(double staticFriction,
										 double dynamicFriction,
										 double restitution) {
	for (int x = 0; x < VDPhysicMaterial::getCollectionSize(); x++) {
		// if properties match equally
		if (restitution ==
				((PxMaterial*)gPhysicMaterialCollection[x]->getMaterial())
					->getRestitution() &&
			staticFriction ==
				((PxMaterial*)gPhysicMaterialCollection[x]->getMaterial())
					->getStaticFriction() &&
			dynamicFriction ==
				((PxMaterial*)gPhysicMaterialCollection[x]->getMaterial())
					->getDynamicFriction()) {
			return gPhysicMaterialCollection
				[x];  // find a material with same properties. IS that enough to
					  // consider as a same Object ??
		}
	}
	return nullptr;
}

PxRigidStatic* VDPhysic::createRigidStatic(VDCollider* collider) {
	if (!VDPhysic::getPhysicHandle()) return nullptr;

	if (collider->getActor()) {
		// convert it to requesed Rigidid
		if (collider->isStaticRigid()) {
			return (PxRigidStatic*)collider->getActor();
		} else {
			// Convert to Static RigidBody
		}
	} else {
		PxRigidStatic* staticActor = engine.physic.gPhysic->createRigidStatic(
			VDPhysic::createPxTransformInstance(collider->transform()));
		if (staticActor) staticActor->userData = collider->gameObject();
		return staticActor;
	}
}

PxRigidDynamic* VDPhysic::createRigidDynamic(VDRigidBody* rigidBody) {
	if (!engine.physic.gPhysic) return nullptr;

	if (rigidBody->getActor()) {
		// convert it to requesed Rigidid
		if (rigidBody->isDynamicRigid())
			return rigidBody->GetDynamicActor();
		else {
			rigidBody->actor =
				VDPhysic::convertDynamicActor(rigidBody->GetStaticActor());
			// Convert to PxRigidDynamic RigidBody
		}
	} else {
		PxRigidDynamic* dynamicActor =
			VDPhysic::getPhysicHandle()->createRigidDynamic(
				VDPhysic::createPxTransformInstance(rigidBody->transform()));
		dynamicActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
		dynamicActor->userData = rigidBody->gameObject();
		dynamicActor->setContactReportThreshold(0.01f);
		// dynamicActor->setOwnerClient(1);
		// dynamicActor->setClientBehaviorFlags(PxActorClientBehaviorFlag::eREPORT_TO_FOREIGN_CLIENTS_CONTACT_NOTIFY);
		return dynamicActor;
	}
}

PxRigidDynamic* VDPhysic::convertDynamicActor(PxRigidStatic* dynamicActor) {
	return 0;
}

PxRigidStatic* VDPhysic::convertStaticActor(PxRigidDynamic* staticActor) {
	return 0;
}

PxRigidActor* VDPhysic::copyInstanceActor(VDPhysicActor* origonalActor,
										  VDPhysicActor* cloneActor,
										  bool shareShape) {
	PxRigidActor* cloneactor;               // new actor instance
	if (origonalActor->isDynamicRigid()) {  // rigidbody actor
		cloneactor = PxCloneDynamic(
			*VDPhysic::getPhysicHandle(),
			VDPhysic::createPxTransformInstance(origonalActor->transform()),
			*origonalActor->GetDynamicActor());
	} else if (origonalActor->isStaticRigid()) {  // static actor
		cloneactor = PxCloneStatic(
			*VDPhysic::getPhysicHandle(),
			VDPhysic::createPxTransformInstance(origonalActor->transform()),
			*origonalActor->GetStaticActor());
	}
	cloneActor->actor = cloneactor;
	cloneActor->actor->userData =
		cloneActor->gameObject();  // reference to GameObject In the Engine.
	PxShape* shape;
	cloneActor->actor->getShapes(&shape, sizeof(PxShape*));
	if (shape) shape->userData = cloneActor->collider();
	return cloneactor;
}

PxCloth* VDPhysic::createClothActor(VDCloth* cloth,
									const HandleMeshEx* meshHandler) {
	if (!cloth || !meshHandler) return nullptr;

	PxClothFabric* fabric = genClothFabric(*meshHandler);

	PxClothParticle vertices[] = {
		PxClothParticle(PxVec3(0.0f, 0.0f, 0.0f), 0.0f),
		PxClothParticle(PxVec3(0.0f, 1.0f, 0.0f), 1.0f),
		PxClothParticle(PxVec3(1.0f, 0.0f, 0.0f), 1.0f),
		PxClothParticle(PxVec3(1.0f, 1.0f, 0.0f), 1.0f)};

	cloth->actor = (PxRigidActor*)engine.physic.gPhysic->createCloth(
		VDPhysic::createPxTransformInstance(cloth->transform()), *fabric,
		vertices, PxClothFlags());
	return VDCASTP(PxCloth*, cloth->actor);
}

void VDPhysic::serializeActor(PxActor* actor, VDBehavior* behavior) {
	// check if there is acollection, or create one with the behavior
	// PxCollection* collection = m_physic->createCollection();
	// collection.setObjectRef(
}

int VDPhysic::initlizeRigidBody(VDRigidBody* rigidBody) {
	if (rigidBody->collider()) {  // has a collider
		if (rigidBody->collider()->isStaticRigid()) {
		} else if (rigidBody->collider()->isDynamicRigid()) {
		}
	} else {
	}
	return SDL_TRUE;
}

PxShape* VDPhysic::createShape(VDCollider* collider, unsigned int collisionFlag,
							   HandleMeshEx* handleMesh) {
	if (engine.physic.gPhysic == nullptr || collider == nullptr ||
		collisionFlag == VDCollider::eUnknown) {
		return nullptr;
	}

	// return out of function. if some of data is useless.
	PxShape* pxShape = collider->getShape();

	PxRigidActor* colliderActor = (PxRigidActor*)collider->GetRigidActor();
	VDPhysicMaterial* material =
		VDPhysic::createMaterial(VDPhysicMaterial::eDefault);
	VDRigidBody* lrigidbody = collider->rigidBody();

	switch (collisionFlag) {
		case VDCollider::eBox: {
			PxBoxGeometry boxGeomtry;
			if (handleMesh != nullptr)  // has if
				boxGeomtry =
					PxBoxGeometry(*(PxVec3*)&(handleMesh->bounds.getSize()));
			if (!boxGeomtry.isValid()) boxGeomtry.halfExtents = PxVec3(1, 1, 1);
			pxShape = colliderActor->createShape(
				boxGeomtry, *(PxMaterial*)material->getMaterial(),
				VDPhysic::createPxTransformInstance(handleMesh->bounds));
			break;
		}
		case VDCollider::eSphere: {
			float Radius = handleMesh->bounds.getSize().x();
			PxSphereGeometry sphereCollider;
			sphereCollider.radius = Radius;
			if (!sphereCollider.isValid()) sphereCollider.radius += 0.01f;
			pxShape = colliderActor->createShape(
				sphereCollider, *(PxMaterial*)material->getMaterial(),
				VDPhysic::createPxTransformInstance(handleMesh->bounds));
		} break;
		case VDCollider::eTerrain: {
			PxHeightField* heightmap = VDPhysic::genHeightCollision(
				collider->getComponent<VDTerrain>());
			if (heightmap != nullptr) {
				PxHeightFieldGeometry hfGem(heightmap, PxMeshGeometryFlags(),
											1.0f, 1.0f, 1.0f);
				if (!hfGem.isValid()) {
					// try to fix it.
				}
				pxShape = colliderActor->createShape(
					hfGem, *(PxMaterial*)material->getMaterial());
			}
		} break;
		case VDCollider::eMesh: {
			PxTriangleMesh* meshCollision =
				VDPhysic::genMeshCollision(*handleMesh);
			if (meshCollision) {
				PxTriangleMeshGeometry meshgeometry(meshCollision);
				meshgeometry.scale.scale =
					*(PxVec3*)&collider->transform()->getScale();
				if (lrigidbody != nullptr) {
					lrigidbody->kinect(true);
					pxShape = lrigidbody->GetDynamicActor()->createShape(
						meshgeometry,
						*(PxMaterial*)material
							 ->getMaterial());  // PxTransform(*(PxVec3*)&this->transform()->postion,*(PxQuat*)&this->transform()->Rotation));
					VDPhysic::addActor(lrigidbody->getActor());
					lrigidbody->PxShapeSimulate(false);
					lrigidbody->kinect(false);
					lrigidbody->useGraivity(true);
				} else {
					pxShape = colliderActor->createShape(
						meshgeometry,
						*(PxMaterial*)material
							 ->getMaterial());  // PxTransform(*(PxVec3*)&this->transform()->postion,*(PxQuat*)&this->transform()->Rotation));
				}
			}
			// meshCollision->set
		} break;
		case VDCollider::eConvex: {
			PxConvexMesh* convex = VDPhysic::genConvexCollision(*handleMesh);
			if (convex) {
				PxConvexMeshGeometry convexG(convex);
				convexG.scale.scale =
					*(PxVec3*)&collider->transform()->getScale();
				pxShape = colliderActor->createShape(
					convexG, *(PxMaterial*)material->getMaterial());
			}
		} break;
		case VDCollider::ePlane: {
			PxPlaneGeometry planeCollider = PxPlaneGeometry();
			pxShape = colliderActor->createShape(
				planeCollider, *(PxMaterial*)material->getMaterial(),
				PxTransformFromPlaneEquation(PxPlane(
					*(PxVec3*)&collider->transform()->getPosition(),
					*(PxVec3*)&collider->transform()->transformDirection(
						-VDVector3::up()))));

		} break;
		case VDCollider::eCapsule: {
			PxCapsuleGeometry capsgem(handleMesh->bounds.getSize().x() *
										  handleMesh->bounds.getSize().z(),
									  handleMesh->bounds.getSize().y());

			pxShape = colliderActor->createShape(
				capsgem, *(PxMaterial*)material->getMaterial(), PxTransform());
		} break;
		case VDCollider::eWheel: {
		} break;
		default:
			return nullptr;
	}
	if (!pxShape) {
		return nullptr;
		// failed to crate some shape.
	}
	if (lrigidbody != nullptr) {
		// lrigidbody->useGraivity(true);
	}

	pxShape->userData = collider;  // collider information.
	pxShape->getActor()->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
	pxShape->getActor()->setActorFlag(PxActorFlag::eSEND_SLEEP_NOTIFIES, true);
	pxShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	pxShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	VDPhysic::setupFiltering(pxShape, 1, 2);

	return pxShape;
}

void VDPhysic::setupFiltering(PxShape* shape, unsigned int filterGroup,
							  unsigned int filterMask) {
	PxFilterData filterData;
	// PxFilterObjectIsTrigger(
	filterData.word0 = filterGroup;  // word0 = own ID
	filterData.word1 = filterMask;   // word1 = ID mask to filter pairs that
									 // trigger a contact callback;
	shape->setSimulationFilterData(filterData);
}

PxShape* VDPhysic::createShape(VDCollider* collider,
							   unsigned int collisionFlag) {
	if (engine.physic.gPhysic == nullptr || collider == nullptr ||
		collisionFlag == VDCollider::eUnknown)
		return nullptr;  // return out of function. if some of data is useless.

	PxShape* shape = nullptr;
	HandleMeshEx meshhandle;

	switch (collisionFlag) {
		case VDCollider::eUnknown:
		case VDCollider::eBox:
		case VDCollider::eSphere:
		case VDCollider::eCapsule:
		case VDCollider::ePlane:
		case VDCollider::eWheel:
		case VDCollider::eCharacterController:
			memset(&meshhandle, 0, sizeof(HandleMeshEx));
			if (collider->renderer()) {
				if (collider->renderer()->getMesh()) {
					meshhandle.bounds =
						collider->renderer()->getMesh()->bounds.aabb;
				}
			}
			return VDPhysic::createShape(collider, collisionFlag, &meshhandle);
		case VDCollider::eMesh:
		case VDCollider::eConvex: {
			if (!collider->renderer()) return nullptr;  // check render

			if (!collider->renderer()->getMesh())
				return nullptr;  // check mesh filter

			if (collider->renderer()->getMesh()->getVertexCount() > (1 << 2))
				return nullptr;

			meshhandle =
				collider->renderer()->getMesh()->createMeshDescription();
			shape = VDPhysic::createShape(collider, collisionFlag, &meshhandle);
			ExFreeMeshHandler(&meshhandle);
		} break;
		case VDCollider::eTerrain: {
			if (collider->getComponent<VDTerrain>()) {
				shape = VDPhysic::createShape(collider, collisionFlag, nullptr);
			}
			break;
		}
	}

	return shape;
}

PxController* VDPhysic::createCharacterController(VDTransform* transform) {
	if (engine.physic.gPhysic == nullptr || transform == nullptr) return nullptr;

	PxCapsuleControllerDesc chDesc;
	chDesc.height = 1.0f;
	chDesc.radius = 0.5f;
	chDesc.material =
		(PxMaterial*)VDPhysic::createMaterial(VDPhysicMaterial::eWood)
			->getMaterial();
	chDesc.position.x = (double)transform->getPosition().x();
	chDesc.position.y = (double)transform->getPosition().y();
	chDesc.position.z = (double)transform->getPosition().z();
	chDesc.reportCallback = engine.physic.gPhysiccontrollerreport;

	PxController* characterController =
		engine.physic.gControlManager->createController(chDesc);
	assert(characterController != nullptr);
	// assigne the gameObject
	PxShape* shape;
	characterController->getActor()->getShapes(&shape, sizeof(PxShape));
	// assign collider data to collider

	if (shape) shape->userData = transform->gameObject()->collider();

	characterController->getActor()->userData = transform->gameObject();
	characterController->getActor()->setOwnerClient(1);
	return characterController;
}

PxTriangleMesh* VDPhysic::genMeshCollision(VDMesh* mesh) {
	if (!mesh) return nullptr;

	HandleMeshEx lmesh = mesh->createMeshDescription();
	PxTriangleMesh* phMesh = VDPhysic::genMeshCollision(lmesh);
	ExFreeMeshHandler(&lmesh);
	return phMesh;
}

PxTriangleMesh* VDPhysic::genMeshCollision(const HandleMeshEx& meshHandler) {
	if (!meshHandler.indicesData || !meshHandler.vertexData) return nullptr;

	if (meshHandler.vertexCount > (1 << 12)) return nullptr;

	PxTriangleMeshDesc meshDesc;
	// vertex Point
	meshDesc.points.count = meshHandler.vertexCount;
	meshDesc.points.stride = meshHandler.meshDescription.vertexSize();
	meshDesc.points.data = meshHandler.vertexData;
	void* indicesData = nullptr;

	// indices Data
	meshDesc.flags = PxMeshFlag::e16_BIT_INDICES;
	if (meshHandler.vertexCount > UCHAR_MAX &&
		meshHandler.vertexCount < UINT_MAX) {
		if (meshHandler.vertexCount > USHRT_MAX)
			meshDesc.flags = (~PxMeshFlag::e16_BIT_INDICES & meshDesc.flags);
		meshDesc.triangles.count = (meshHandler.indicesCount / 3);
		meshDesc.triangles.stride =
			meshHandler.meshDescription.indicesSize() * 3;
		meshDesc.triangles.data = meshHandler.indicesData;
	} else {
		indicesData = callocStrideData(meshHandler.indicesData, 1,
									   meshHandler.indicesCount, 2);
		meshDesc.triangles.count = (meshHandler.indicesCount / 3);
		meshDesc.triangles.stride = 6;
		meshDesc.triangles.data = indicesData;
	}

	PxDefaultMemoryOutputStream writebuffer;
	if (!meshDesc.isValid()) {
		throw RuntimeException("Mesh collider Description was not valid!\n");
	}

	bool cookSucess =
		engine.physic.gCooking->cookTriangleMesh(meshDesc, writebuffer);
	if (!cookSucess) {
		free(indicesData);
		throw RuntimeException("Failed to Cook Mesh Collision\n");
		return nullptr;
	}

	PxDefaultMemoryInputData readBuffer(writebuffer.getData(),
										writebuffer.getSize());
	PxTriangleMesh* meshCollider =
		engine.physic.gPhysic->createTriangleMesh(readBuffer);
	free(indicesData);
	return meshCollider;
}

PxConvexMesh* VDPhysic::genConvexCollision(const HandleMeshEx& meshHandler) {
	PxConvexMeshDesc convexDesc;

	convexDesc.setToDefault();
	convexDesc.points.count = meshHandler.vertexCount;
	convexDesc.points.stride = meshHandler.meshDescription.vertexSize();
	convexDesc.points.data = meshHandler.vertexData;
	convexDesc.flags =
		PxConvexFlag::eCOMPUTE_CONVEX | PxConvexFlag::e16_BIT_INDICES;
	convexDesc.vertexLimit = 256;

	// initilize structor of mesh data
	unsigned short* indicesData = nullptr;
	if (meshHandler.vertexCount > UCHAR_MAX &&
		meshHandler.vertexCount < USHRT_MAX) {
		convexDesc.indices.count = meshHandler.indicesCount / 3;
		convexDesc.indices.data = meshHandler.indicesData;
		convexDesc.indices.stride =
			meshHandler.meshDescription.indicesSize() * 3;
	} else {  // Mesh Geometry use a 8 Bits Indices Type.[PhysX don't support by
			  // it default]
		indicesData = (unsigned short*)callocStrideData(
			VDCASTP(void*, meshHandler.indicesData), 1,
			meshHandler.indicesCount, 2);
		convexDesc.indices.count = meshHandler.indicesCount / 3;
		convexDesc.indices.data = indicesData;
		convexDesc.indices.stride = 6;
	}
	if (!convexDesc.isValid()) {
		throw RuntimeException("InValid ConvexMesh Description");
		convexDesc.flags |= PxConvexFlag::eINFLATE_CONVEX;
	}
	PxDefaultMemoryOutputStream writebuffer;
	if (!engine.physic.gCooking->cookConvexMesh(convexDesc, writebuffer)) {
		convexDesc.flags |= PxConvexFlag::eINFLATE_CONVEX;
		if (!engine.physic.gCooking->cookConvexMesh(convexDesc, writebuffer)) {
			throw RuntimeException("Failed to CookMesh Description\n");
			free(indicesData);
			return nullptr;
		}
	}
	PxDefaultMemoryInputData readBuffer(writebuffer.getData(),
										writebuffer.getSize());
	PxConvexMesh* convecMesh =
		engine.physic.gPhysic->createConvexMesh(readBuffer);
	free(indicesData);
	return convecMesh;
}

PxConvexMesh* VDPhysic::genConvexCollision(VDMesh* mesh) {
	if (mesh == nullptr) return nullptr;

	HandleMeshEx exhandle = mesh->createMeshDescription();
	PxConvexMesh* convex = VDPhysic::genConvexCollision(exhandle);
	ExFreeMeshHandler(&exhandle);
	return convex;
}

PxClothParticle* VDPhysic::genClothParticle(const HandleMeshEx& meshHandler) {
	PxClothParticle* particle = (PxClothParticle*)malloc(
		sizeof(PxClothParticle) * meshHandler.vertexCount);

	// PxVec3 center = *CASTP(PxVec3*,&meshHandler.bounds.center);
	PxVec3 delta =
		1.0f / (meshHandler.vertexCount - 1) * PxVec3(15.0f, 15.0f, 15.0f);
	/*	for(PxU32 i=0; i<meshHandler.vertexCount; ++i)
			{
					for(PxU32 j=0; j<meshHandler.vertexCount; ++j, ++pIt)
					{
							pIt->invWeight = j+1<meshHandler.vertexCount ? 1.0f
	   : 0.0f; pIt->pos = delta.multiply(PxVec3(PxReal(i), PxReal(j),
	   -PxReal(j))) - center;
					}
			}*/
	return particle;
}

PxClothFabric* VDPhysic::genClothFabric(const HandleMeshEx& meshHandler) {
	PxClothMeshDesc Clothdesc;

	void* indicesData = nullptr;
	Clothdesc.points.count = meshHandler.vertexCount;
	Clothdesc.points.stride = meshHandler.meshDescription.vertexSize();
	Clothdesc.points.data = meshHandler.vertexData;
	Clothdesc.flags |= PxMeshFlag::e16_BIT_INDICES;

	if (meshHandler.vertexCount > UCHAR_MAX) {
		if (meshHandler.vertexCount > UINT_MAX)
			Clothdesc.flags |= (~PxMeshFlag::e16_BIT_INDICES & Clothdesc.flags);
		Clothdesc.triangles.data = meshHandler.indicesData;
		Clothdesc.triangles.count = (meshHandler.indicesCount / 3);
		Clothdesc.triangles.stride =
			meshHandler.meshDescription.indicesSize() * 3;
	} else {
		indicesData = callocStrideData(
			meshHandler.indicesData, meshHandler.meshDescription.indicesSize(),
			meshHandler.indicesCount, sizeof(unsigned short));
		Clothdesc.triangles.data = indicesData;
		Clothdesc.triangles.count = (meshHandler.indicesCount / 3);
		Clothdesc.triangles.stride = 6;
	}

	// Clothdesc.invMasses.

	if (!Clothdesc.isValid()) {
		VDDebug::errorLog("Faild");
	}

	PxClothFabric* fabric =
		PxClothFabricCreate(*engine.physic.gPhysic, Clothdesc,
							engine.physic.gPhysxScene->getGravity());

	free(indicesData);
	return fabric;
}

PxClothFabric* VDPhysic::genClothFabric(VDMesh* mesh) {
	if (!mesh) return nullptr;
	HandleMeshEx meshHandler = mesh->createMeshDescription();
	PxClothFabric* fabric = VDPhysic::genClothFabric(meshHandler);
	ExFreeMeshHandler(&meshHandler);
	return fabric;
}

PxParticleSystem* VDPhysic::genParticleSystem(unsigned int maxParticleCount) {
	if (!engine.physic.gPhysic) return nullptr;

	PxParticleSystem* p_particleSystem =
		engine.physic.gPhysic->createParticleSystem(maxParticleCount);
	return p_particleSystem;
}

PxParticleFluid* VDPhysic::genParticleFluid(unsigned int maxParticleCount) {
	if (!engine.physic.gPhysic) return nullptr;

	PxParticleFluid* p_particleFluid =
		engine.physic.gPhysic->createParticleFluid(maxParticleCount);
	assert(p_particleFluid == 0);
	return p_particleFluid;
}

PxHeightFieldSample* VDPhysic::createHeightFieldSample(VDTerrain* terrain) {
	PxHeightFieldSample* sample = (PxHeightFieldSample*)malloc(
		sizeof(PxHeightFieldSample) *
		(terrain->getWidth() * terrain->getHeight()));
	memset(sample, 0,
		   sizeof(PxHeightFieldSample) *
			   (terrain->getWidth() * terrain->getHeight()));
	VDVector3* vertex = (VDVector3*)terrain->getMesh()->getVertexBundle();

	for (unsigned int y = 0; y < terrain->getHeight(); y++) {
		for (unsigned int x = 0; x < terrain->getWidth(); x++) {
			sample->materialIndex0 = 0;
			sample->materialIndex1 = 0;
			sample->clearTessFlag();
			sample->height = -vertex[y * terrain->getHeight() + x].y();
		} /**/
	}     /**/
	free(vertex);
	return sample;
}

PxHeightField* VDPhysic::genHeightCollision(VDTerrain* terrain) {
	if (!terrain) return nullptr;

	PxHeightFieldSample* heightField =
		VDPhysic::createHeightFieldSample(terrain);
	PxHeightFieldDesc hfDesc;
	hfDesc.setToDefault();
	hfDesc.format = PxHeightFieldFormat::eS16_TM;
	hfDesc.nbColumns = terrain->getWidth();
	hfDesc.nbRows = terrain->getHeight();
	hfDesc.samples.data = (PxU32*)heightField;
	hfDesc.samples.stride = sizeof(PxHeightFieldSample);
	hfDesc.thickness = -1.0f;
	PxDefaultMemoryOutputStream writebuffer;
	// if(!m_cooking->cookHeightField(hfDesc,writebuffer)){
	//	Debug::errorLog("Failed to Cook Height Field map");
	// failed to cook the heightfield.
	//}

	// PxDefaultMemoryInputData readBuffer(writebuffer.getData(),
	// writebuffer.getSize());
	PxHeightField* p_HeightField =
		engine.physic.gPhysic->createHeightField(hfDesc);
	free(heightField);
	return p_HeightField;
}

void VDPhysic::addActor(void* actor) {
	if (!actor)
		return;

	else
		engine.physic.gPhysxScene->addActor(*VDCASTP(PxActor*, actor));
}

void VDPhysic::removeActor(void* actor) {
	engine.physic.gPhysxScene->removeActor(*VDCASTP(PxActor*, actor));
}

PxPhysics* VDPhysic::getPhysicHandle(void) { return engine.physic.gPhysic; }

PxScene* VDPhysic::getPhysicScene(void) { return engine.physic.gPhysxScene; }

PxFoundation* VDPhysic::getFoundationHandle(void) {
	return engine.physic.gFoundation;
}

PxControllerManager* VDPhysic::getControllerManager(void) {
	return engine.physic.gControlManager;
}

PxProfileZoneManager* VDPhysic::getProfileZoneManager(void) {
	return engine.physic.gProfileManager;
}

PxVisualDebugger* VDPhysic::getVisualDebugger(void) {
	if (engine.physic.gPhysic)
		return engine.physic.gPhysic->getVisualDebugger();
	else
		return nullptr;
}

PxCudaContextManager* VDPhysic::getCudaContextManager(void) {
	return engine.physic.gCudaContextManager;
}

PxTransform VDPhysic::createPxTransformInstance(const VDTransform* transform) {
	if (transform)
		return PxTransform(*(const PxVec3*)&transform->getPosition(),
						   *(const PxQuat*)&transform->getRotation());
	else
		return PxTransform();
}

PxTransform VDPhysic::createPxTransformInstance(
	const VDAABB& offset, const VDQuaternion& quaternion) {
	return PxTransform(*(const PxVec3*)&offset.getCenter(),
					   *(const PxQuat*)&quaternion);
}

PxFilterFlags PxCustomFilterShader(PxFilterObjectAttributes attributes0,
								   PxFilterData filterData0,
								   PxFilterObjectAttributes attributes1,
								   PxFilterData filterData1,
								   PxPairFlags& pairFlags,
								   const void* constantBlock,
								   PxU32 constantBlockSize) {
	if (PxFilterObjectIsTrigger(attributes0) ||
		PxFilterObjectIsTrigger(attributes1)) {
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}
	// generate contacts for all that were not filtered above
	pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND;

	PxU32 ShapeGroup0 = filterData0.word0 & 31;
	PxU32 ShapeGroup1 = filterData1.word0 & 31;
	PxU32* groupCollisionFlags = (PxU32*)constantBlock;

	// if ((groupCollisionFlags[ShapeGroup0] & (1 << ShapeGroup1)) == 0)
	//		return PxFilterFlag::eSUPPRESS;
	// else
	return PxFilterFlag::eDEFAULT;
}

void SimulationEventCallback::onConstraintBreak(PxConstraintInfo* constraints,
												PxU32 count) {
	this->dataBreak.count = count;
	this->dataBreak.constraints = constraints;
}

void SimulationEventCallback::onWake(PxActor** actors, PxU32 count) {}
void SimulationEventCallback::onSleep(PxActor** actors, PxU32 count) {}

void SimulationEventCallback::onContact(const PxContactPairHeader& pairHeader,
										const PxContactPair* pairs,
										PxU32 nbPairs) {
	this->dataContact.pairs = (PxContactPair*)pairs;
	this->dataContact.nbPair = nbPairs;
}

void SimulationEventCallback::onTrigger(PxTriggerPair* pairs, PxU32 count) {
	this->dataTrigger.pairs = pairs;
	this->dataTrigger.count = count;
}

void SimulationEventCallback::EventonConstraintBreak(void) {
	PxRigidActor *a1, *a2;
	VDGameObject *g1, *g2;
	for (unsigned int ncount = 0; ncount < dataBreak.count; ncount++) {
		// getting actor
		dataBreak.constraints[ncount].constraint->getActors(a1, a2);
		// getting gameobject based on the actor userdata.
		g1 = VDCASTP(VDGameObject*, a1->userData);
		g2 = VDCASTP(VDGameObject*, a2->userData);

		for (unsigned int x = 0; x < g1->getComponentCollection().size(); x++) {
			if (g1->getComponentCollection()[x]->isBroodBehavior())
				VDCASTP(VDBroodBehavior*, g1->getComponentCollection()[x])
					->onJointBreak(0.0f);
		}
		for (unsigned int x = 0; x < g2->getComponentCollection().size(); x++) {
			if (g2->getComponentCollection()[x]->isBroodBehavior())
				VDCASTP(VDBroodBehavior*, g2->getComponentCollection()[x])
					->onJointBreak(0.0f);
		}
	}
}

void SimulationEventCallback::EventonContact(void) {
	unsigned int nCount;
	VDCollision collision;

	for (nCount = 0; nCount < this->dataContact.nbPair; nCount++) {
		VDGameObject* g1 = VDCASTP(
			VDGameObject*,
			this->dataContact.pairs[nCount].shapes[0]->getActor()->userData);
		VDGameObject* g2 = VDCASTP(
			VDGameObject*,
			this->dataContact.pairs[nCount].shapes[1]->getActor()->userData);

		if (this->dataContact.pairs[nCount].events &
			PxPairFlag::eNOTIFY_TOUCH_LOST) {
			for (int x = 0; x < g1->getComponentCollection().size(); x++) {
				// if(g1->componets[x]->isBroodBehavior())
				// CASTP(BroodBehavior*,g1->componets[x])->onCollisionExit(Collision(&this->dataContact.pairs[nCount],this->dataContact.pairs[nCount].shapes[1]));
			}
			for (int x = 0; x < g2->getComponentCollection().size(); x++) {
				// if(g2->componets[x]->isBroodBehavior())
				// CASTP(BroodBehavior*,g2->componets[x])->onCollisionExit(Collision(&this->dataContact.pairs[nCount],this->dataContact.pairs[nCount].shapes[0]));
			}
		}
		if (this->dataContact.pairs[nCount].events &
			PxPairFlag::eNOTIFY_TOUCH_FOUND) {
			for (int x = 0; x < g1->getComponentCollection().size(); x++) {
				// if(g1->componets[x]->isBroodBehavior())
				// CASTP(BroodBehavior*,g1->componets[x])->onCollisionEnter(Collision(&this->dataContact.pairs[nCount],this->dataContact.pairs[nCount].shapes[1]));
			}
			for (int x = 0; x < g2->getComponentCollection().size(); x++) {
				// if(g2->componets[x]->isBroodBehavior())
				// CASTP(BroodBehavior*,g2->componets[x])->onCollisionEnter(Collision(&this->dataContact.pairs[nCount],this->dataContact.pairs[nCount].shapes[0]));
			}
		}
		if (this->dataContact.pairs[nCount].events &
			PxPairFlag::eNOTIFY_TOUCH_PERSISTS) {
			for (int x = 0; x < g1->getComponentCollection().size(); x++) {
				// if(g1->componets[x]->isBroodBehavior())
				//	CASTP(BroodBehavior*,g1->componets[x])->onCollisionStay(Collision(&this->dataContact.pairs[nCount],this->dataContact.pairs[nCount].shapes[1]));
			}
			for (int x = 0; x < g2->getComponentCollection().size(); x++) {
				// if(g2->componets[x]->isBroodBehavior())
				// CASTP(BroodBehavior*,g2->componets[x])->onCollisionStay(Collision(&this->dataContact.pairs[nCount],this->dataContact.pairs[nCount].shapes[0]));
			}
		}
		continue;
	}  // for each pair
	this->dataContact.nbPair = 0;
}

void SimulationEventCallback::EventonTrigger(void) {
	for (unsigned int nCount = 0; nCount < this->dataTrigger.count; nCount++) {
		VDGameObject* g =
			VDCASTP(VDGameObject*,
					this->dataTrigger.pairs[nCount].triggerActor->userData);
		switch (this->dataTrigger.pairs[nCount].status) {
				// On Trigger Exit
			case PxPairFlag::eNOTIFY_TOUCH_LOST:
				for (int x = 0; x < g->getComponentCollection().size(); x++) {
					if (g->getComponentCollection()[x]->isBroodBehavior())
						VDCASTP(VDBroodBehavior*,
								g->getComponentCollection()[x])
							->onTriggerExit(VDCASTP(
								VDCollider*, this->dataTrigger.pairs[nCount]
												 .otherShape->userData));
				}
				break;
				// On Trigger Enter
			case PxPairFlag::eNOTIFY_TOUCH_FOUND:
				for (int x = 0; x < g->getComponentCollection().size(); x++) {
					if (g->getComponentCollection()[x]->isBroodBehavior())
						VDCASTP(VDBroodBehavior*,
								g->getComponentCollection()[x])
							->onTriggerEnter(VDCASTP(
								VDCollider*, this->dataTrigger.pairs[nCount]
												 .otherShape->userData));
				}
				break;
			case PxPairFlag::eNOTIFY_TOUCH_PERSISTS:
				for (int x = 0; x < g->getComponentCollection().size(); x++) {
					if (g->getComponentCollection()[x]->isBroodBehavior())
						VDCASTP(VDBroodBehavior*,
								g->getComponentCollection()[x])
							->onTriggerStay(VDCASTP(
								VDCollider*, this->dataTrigger.pairs[nCount]
												 .otherShape->userData));
				}
				break;
			default:
				continue;
		}  // event
	}      // number of trigger
	this->dataTrigger.count = 0;
}

void PhysicUserControllerHitReport::onShapeHit(
	const physx::PxControllerShapeHit& hit) {
	VDGameObject* g =
		VDCASTP(VDGameObject*, hit.controller->getActor()->userData);
	VDCharacterControllerHit t;
	for (unsigned int x = 0; x < g->getComponentCollection().size(); x++) {
		if (g->getComponentCollection()[x]->isBroodBehavior())
			VDCASTP(VDBroodBehavior*, g->getComponentCollection()[x])
				->onCharacterController(VDCharacterControllerHit(
					(const physx::PxControllerShapeHit*)&hit));
	}
}

void PhysicUserControllerHitReport::onControllerHit(
	const PxControllersHit& hit) {
	VDGameObject* g =
		VDCASTP(VDGameObject*, hit.controller->getActor()->userData);
	VDCharacterControllerHit t;

	for (unsigned int x = 0; x < g->getComponentCollection().size(); x++) {
		if (g->getComponentCollection()[x]->isBroodBehavior())
			VDCASTP(VDBroodBehavior*, g->getComponentCollection()[x])
				->onCharacterController(t);
	}
}

void PhysicUserControllerHitReport::onObstacleHit(
	const PxControllerObstacleHit& hit) {}

#undef USE_PHYSX
