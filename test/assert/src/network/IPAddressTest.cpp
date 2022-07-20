#include <Core/Network/IPAddress.h>
#include <Core/Network/IPInterface.h>
#include <Core/RefPtr.h>
#include <gtest/gtest.h>

using namespace fragcore;

class IPAddressTest : public testing::Test {
  public:
	const char *localIP = "127.0.0.1";
};

TEST_F(IPAddressTest, Create_Default_Constructor_No_Exception) { ASSERT_NO_THROW(IPAddress localHost); }

TEST_F(IPAddressTest, Create_Local_Address_IPV4_No_Exception) {
	ASSERT_NO_THROW(IPAddress localHost(this->localIP, IPAddress::IPAddressType::IPAddress_Type_IPV4));
}

TEST_F(IPAddressTest, Create_Local_Address_IPV4_Valid) {
	IPAddress localHost(this->localIP, IPAddress::IPAddressType::IPAddress_Type_IPV4);
	ASSERT_TRUE(localHost.isValid());
}

TEST_F(IPAddressTest, Create_Local_Address_IPV6_No_Exception) {
	ASSERT_NO_THROW(IPAddress localHost(this->localIP, IPAddress::IPAddressType::IPAddress_Type_IPV6));
}

TEST_F(IPAddressTest, Create_Local_Address_IPV6_Valid) {
	IPAddress localHost(this->localIP, IPAddress::IPAddressType::IPAddress_Type_IPV6);
	ASSERT_TRUE(localHost.isValid());
}

TEST_F(IPAddressTest, Create_Local_Adddress_FDQN_No_Exception) {
	ASSERT_NO_THROW(IPAddress localHost("localhost", IPAddress::IPAddressType::IPAddress_Type_IPV4));
}

TEST_F(IPAddressTest, Create_InCorrect_FDQN_Local_Adddress_Throw_Exception) {
	ASSERT_ANY_THROW(IPAddress localHost("incorrect_fqdn"));
}