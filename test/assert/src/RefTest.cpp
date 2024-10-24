#include "Core/Ref.h"
#include "Core/Object.h"
#include "Core/SmartReference.h"
#include <gtest/gtest.h>
using namespace fragcore;

TEST(Ref, Added_First_Object_Increment2_1) {
	Ref<SmartReference> ref = Ref<SmartReference>(new SmartReference());

	ASSERT_EQ(ref->getIndexCount(), 1);
}

TEST(Ref, _On_No_Reference) {
	Ref<SmartReference> ref = Ref<SmartReference>(new SmartReference());
	ref->increment();
	ref->deincreemnt();
	ASSERT_NO_THROW(ref->deincreemnt());
}

TEST(Ref, Ref_Operator_Pointer) {
	SmartReference *obj = new SmartReference();
	Ref<SmartReference> ref = Ref<SmartReference>(obj);
	ASSERT_EQ(ref.ptr(), obj);
}

TEST(Ref, Ref_OnAllReferenceRemoved_ReleaseResources_No_Throw) {}

TEST(Ref, RefCounter) {

	//	Ref<Object> ref;

	/*  */
	//	ASSERT_EQ(ref.ptr(), NULL);
	//	ASSERT_EQ(ref.operator->(), NULL);
	//	Shader* = new Object();
	//	Ref<Object> ref(obj);

	//	ASSERT_EQ(ref.ptr(), obj);
	//	ASSERT_EQ(ref->)
}