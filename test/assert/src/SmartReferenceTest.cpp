#include "Core/SmartReference.h"
#include "Core/Object.h"
#include <gtest/gtest.h>
using namespace fragcore;

TEST(SmartReference, SmartReference_Init_State_One_Reference_Count) {
	SmartReference smartReference;
	ASSERT_EQ(smartReference.getIndexCount(), 1);
}

TEST(SmartReference, SmartReference_Increment_Success) {
	SmartReference smartReference;
	ASSERT_TRUE(smartReference.increment());
}

TEST(SmartReference, SmartReference_Deincreemnt_Success) {
	SmartReference smartReference;
	ASSERT_TRUE(smartReference.deincreemnt());
}

TEST(SmartReference, SmartReference_On_DeConstructor) { SmartReference *smart = new SmartReference(); }


TEST(SmartReference, Create_Default_No_Throw) {

	ASSERT_NO_THROW(SmartReference smartReference);

	/*	*/
	// ASSERT_TRUE(smartReference.increment());
	// ASSERT_EQ(smartReference.getIndexCount(), 2);
	// ASSERT_EQ(smartReference.deincreemnt(), false);
	// ASSERT_TRUE(smartReference.deincreemnt());
	// ASSERT_EQ(smartReference.getIndexCount(), 1);
	// ASSERT_FALSE(smartReference.deincreemnt());
	// ASSERT_EQ(smartReference.getIndexCount(), 0);
}