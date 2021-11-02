#include <Core/UIDObject.h>
#include <gtest/gtest.h>
using namespace fragcore;

TEST(UIDGenerator, Create_Default_Constructor_No_Throw) { ASSERT_NO_THROW(UIDGenerator<unsigned int> uid); }

TEST(UIDGenerator, Next_UID_Is_Bigger_No_Throw) {
	UIDGenerator<unsigned int> uid;
	unsigned int uid1 = uid.getNextUID();
	unsigned int uid2 = uid.getNextUID();
	ASSERT_GE(uid2, uid1);
}
