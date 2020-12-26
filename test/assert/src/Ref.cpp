#include"Core/Ref.h"
#include"Core/Object.h"
#include<gtest/gtest.h>
#include<Renderer/Texture.h>
using namespace fragcore;


TEST(References, SmartReference)
{

	SmartReference smartReference;

	/*	*/
	ASSERT_TRUE(smartReference.increment());
	ASSERT_EQ(smartReference.getIndexCount(), 2);
	ASSERT_EQ(smartReference.deincreemnt(), false);
	ASSERT_TRUE(smartReference.deincreemnt());
	ASSERT_EQ(smartReference.getIndexCount(), 1);
	ASSERT_FALSE(smartReference.deincreemnt());
	ASSERT_EQ(smartReference.getIndexCount(), 0);
}


TEST(Ref, Ref_Added_First_Object_Increment2_1){
	Ref<SmartReference> ref = Ref<SmartReference>(new SmartReference());
	ASSERT_EQ(ref->getIndexCount(), 1);
}

TEST(Ref, Ref_On_No_Reference)
{
	Ref<SmartReference> ref = Ref<SmartReference>(new SmartReference());
	ref->increment();
	ref->deincreemnt();
	ASSERT_NO_THROW(ref->deincreemnt());
}

TEST(Ref, Ref_Operator_Pointer){
	SmartReference* obj = new SmartReference();
	Ref<SmartReference> ref = Ref<SmartReference>(obj);
	ASSERT_EQ(ref.ptr(), obj);
}

TEST(Ref, Ref_OnAllReferenceRemoved_ReleaseResources_No_Throw){
	
}

TEST(References, RefCounter)
{

	//	Ref<Object> ref;

	/*  */
	//	ASSERT_EQ(ref.ptr(), NULL);
	//	ASSERT_EQ(ref.operator->(), NULL);
	//	Shader* = new Object();
	//	Ref<Object> ref(obj);

	//	ASSERT_EQ(ref.ptr(), obj);
	//	ASSERT_EQ(ref->)
}