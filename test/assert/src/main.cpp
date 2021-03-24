#include<gtest/gtest.h>
//#include<HpmCpp.h>
int main(int argc, char **argv) {
//    LIBHPM::Hpm::init(Hpm::eHPM_NOSIMD);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}