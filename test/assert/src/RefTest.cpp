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
	ASSERT_EQ(ref, nullptr);
	ref = nullptr;
	ASSERT_EQ(ref, nullptr);
}

TEST(Ref, Ref_Operator_Pointer) {
	SmartReference *obj = new SmartReference();
	Ref<SmartReference> ref = Ref<SmartReference>(obj);
	ASSERT_EQ(ref.get(), obj);
}

TEST(Ref, Ref_OnAllReferenceRemoved_ReleaseResources_No_Throw) {
	Ref<SmartReference> ref = Ref<SmartReference>(new SmartReference());
}