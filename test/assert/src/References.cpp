#include "Core/Ref.h"
#include "Core/Object.h"
#include <gtest/gtest.h>
#include <Renderer/Texture.h>
using namespace fragcore;

TEST(SmartReference, SmartReference_Init_State_One_Reference_Count)
{
	SmartReference smartReference;
	ASSERT_EQ(smartReference.getIndexCount(), 1);
}

TEST(SmartReference, SmartReference_Increment_Success)
{
	SmartReference smartReference;
	ASSERT_TRUE(smartReference.increment());
}

TEST(SmartReference, SmartReference_Deincreemnt_Success)
{
	SmartReference smartReference;
	ASSERT_TRUE(smartReference.deincreemnt());
}

TEST(SmartReference, SmartReference_On_DeConstructor)
{
	SmartReference *smart = new SmartReference();
}
