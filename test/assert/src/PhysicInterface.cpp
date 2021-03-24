#include <gtest/gtest.h>
#include <Physic/Contact.h>
#include <Physic/PhysicFactory.h>
#include <Physic/PhysicInterface.h>
#include <Physic/RayCastHit.h>

#include "common.h"
using namespace fragcore;

class PhysicInterfaceTest : public CommonBaseTest {
   protected:
	void TearDown() override {}

	void SetUp() override {}

	// TODO add method that will be translated to all the interfaces to allow
	// code reuse between the tests
	void CreateClothNoThrow(void) {}
};

// TODO resolve the naming convention for the test functions.
TEST(PhysicInterface, PhysicInteface_Create_ENUM_Bullet_No_Throw) {
	ASSERT_NO_THROW(PhysicFactory::createPhysic(PhysicFactory::Bullet, NULL));
}

TEST(PhysicInterface, PhysicInteface_Delete_ENUM_Bullet_No_Throw) {
	PhysicInterface *interface =
		PhysicFactory::createPhysic(PhysicFactory::Bullet, NULL);
	delete interface;
}
TEST(PhysicInterface, PhysicInterface_Enum_Bullet_Valid_Version_String) {
	PhysicInterface *interface =
		PhysicFactory::createPhysic(PhysicFactory::Bullet, NULL);
	//ASSERT_NE(interface->getVersion(), NULL);
}

TEST(PhysicInterface, PhysicInteface_Create_ENUM_PhysX_No_Throw) {
	ASSERT_NO_THROW(PhysicFactory::createPhysic(PhysicFactory::PhysX, NULL));
}

// TODO determine if needed.
TEST(PhysicInterface, PhysicInteface_Create_ENUM_Bullet3_No_Throw) {
	ASSERT_NO_THROW(PhysicFactory::createPhysic(PhysicFactory::Bullet3, NULL));
}

TEST(PhysicInterface, PhysicInterface_Enum_Bullet_Set_Gravity_Has_Affect) {
	const Vector3 gravity(0.0f, 9.82f, 0.0f);
	PhysicInterface *interface =
		PhysicFactory::createPhysic(PhysicFactory::Bullet, NULL);
	interface->setGravity(gravity);
	ASSERT_EQ(interface->getGravity(), gravity);
}
TEST(PhysicInterface,
	 PhysicInterface_Enum_Bullet_Create_CharacterController_No_Throw) {
	PhysicInterface *interface =
		PhysicFactory::createPhysic(PhysicFactory::Bullet, NULL);

	CharacterControllerDesc desc;
	ASSERT_NO_THROW(interface->createCharacterController(&desc));
}

TEST(PhysicInterface, PhysicInterface_Enum_Bullet_Create_Cloth_No_Throw) {
	PhysicInterface *interface =
		PhysicFactory::createPhysic(PhysicFactory::Bullet, NULL);
	ClothDesc desc;
	ASSERT_NO_THROW(interface->createCloth(&desc));
}

TEST(PhysicInterface, PhysicInterface_Enum_Bullet_RayTest_No_Throw) {
	PhysicInterface *interface =
		PhysicFactory::createPhysic(PhysicFactory::Bullet, NULL);
	RayCastHit hit;
	ASSERT_NO_THROW(interface->rayTest(
		PVRay(Vector3::zero(), Vector3::forward()), &hit));
}

TEST(PhysicInterface, PhysicInterface_Enum_Bullet_RayTest_Fetch_Hit) {
	PhysicInterface *interface =
		PhysicFactory::createPhysic(PhysicFactory::Bullet, NULL);
	RayCastHit hit;
	interface->rayTest(PVRay(Vector3::zero(), Vector3::forward()), &hit);
	// TODO add testing.
}

TEST(PhysicInterface, PhysicInterface_Enum_Bullet_RaySphereTest_No_Throw) {
	PhysicInterface *interface =
		PhysicFactory::createPhysic(PhysicFactory::Bullet, NULL);
	RayCastHit hit;
	ASSERT_NO_THROW(interface->raySphereTest(
		PVRay(Vector3::zero(), Vector3::forward()), &hit));
}

TEST(PhysicInterface,
	 PhysicInterface_Enum_Bullet_Create_Box_Collision_No_Throw) {
	PhysicInterface *interface =
		PhysicFactory::createPhysic(PhysicFactory::Bullet, NULL);

	CollisionDesc desc;
	desc.Primitive = CollisionDesc::ShapePrimitive::eBox;
	ASSERT_NO_THROW(interface->createCollision(&desc));
}

TEST(PhysicInterface, PhysicInterface_Bullet_Simulate_No_Error) {
	PhysicInterface *interface =
		PhysicFactory::createPhysic(PhysicFactory::Bullet, NULL);
	ASSERT_NO_FATAL_FAILURE(interface->simulate(0.16f));
}