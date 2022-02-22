#include"NetModbusSocket.h"
#include <Core/RefPtr.h>
#include <gtest/gtest.h>

using namespace fragcore;

class ModBusAddressTest : public testing::Test {
  protected:
	virtual void SetUp() override {

		const char *ipAddress = "127.0.0.1";
	//	IP4Address = IPAddress(ipAddress, IPAddress::IPAddressType::IPAddress_Type_IPV4);
		////IP6Address = IPAddress(ipAddress, IPAddress::IPAddressType::IPAddress_Type_IPV6);
	}
	//IPAddress IP4Address;
	//IPAddress IP6Address;
};

// TEST_F(ModBusAddressTest, Create_Local_Address_IPV4_Throw_Exception) {
// 	ASSERT_NO_THROW(TCPUDPAddress(IP4Address, 54231));
// }

// TEST_F(ModBusAddressTest, Create_Local_Address_IPV6_Throw_Exception) {
// 	ASSERT_NO_THROW(TCPUDPAddress(IP6Address, 54231));
// }