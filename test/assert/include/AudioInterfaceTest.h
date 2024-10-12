#ifndef _TEST_AUDIOINTERFACE_TEST_H_
#define _TEST_AUDIOINTERFACE_TEST_H_ 1
#include "common.h"
using namespace fragcore;

class AudioInterfaceTest : public CommonBaseTest {
  protected:
	void TearDown() override;

	void SetUp() override;
	std::vector<AudioFactory::AudioAPI> apis;

	/*	TODO add all the common test that only need the interface as a parameter.*/
};

#endif
