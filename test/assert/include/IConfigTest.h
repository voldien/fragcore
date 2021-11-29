#ifndef FRAG_CORE_ICONFIGTEST_H_
#define FRAG_CORE_ICONFIGTEST_H_ 1
#include"common.h"


class IConfigTest : public CommonBaseTest {
protected:
	void TearDown() override;

	void SetUp() override;

	fragcore::IConfig* config;
};

#endif
