#include "Core/Object.h"
#include "Core/Ref.h"
#include <Renderer/Texture.h>
#include <gtest/gtest.h>
using namespace fragcore;

TEST(SmartReference, Create_Default_No_Throw) {

	ASSERT_NO_THROW(SmartReference smartReference);

	/*	*/
	ASSERT_TRUE(smartReference.increment());
	ASSERT_EQ(smartReference.getIndexCount(), 2);
	ASSERT_EQ(smartReference.deincreemnt(), false);
	ASSERT_TRUE(smartReference.deincreemnt());
	ASSERT_EQ(smartReference.getIndexCount(), 1);
	ASSERT_FALSE(smartReference.deincreemnt());
	ASSERT_EQ(smartReference.getIndexCount(), 0);
}