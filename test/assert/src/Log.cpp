#include<gtest/gtest.h>
#include<Core/Log.h>
#include<Exception/InvalidArgumentException.h>
using namespace fragcore;

TEST(Log, Log_Set_Invalid_Verbosity_Throw_Exception){
	ASSERT_THROW(Log::setVerbosity((Log::VERBOSITY)-1), InvalidArgumentException);
}

TEST(Log, Log_Set_Debug_Verbosity_Correct){
	Log::setVerbosity(Log::Debug);
	ASSERT_EQ(Log::getVerbosity(), Log::Debug);
}

TEST(Log, Log_log){
//	Log::clearIO();
//	Log::removeIOOutPut();
//	Log::addIOOutput();
}