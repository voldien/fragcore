#include<gtest/gtest.h>
#include<Core/Object.h>

using namespace fragcore;

TEST(Object, Object_DefaultConstructor_NoThrow){
	ASSERT_NO_THROW(Object obj);
}

TEST(Object, Object_DefaultConstructor_Name) {
    Object obj;
    ASSERT_STREQ(obj.getName().c_str(), "");
}

TEST(Object, Object_Set_Name_As_Copy){
	Object obj;
	obj.setName("Hello World");
}

TEST(UIObject, UIDObject_Defaul_Constructor){
	UIDObject obj;
}