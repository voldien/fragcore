#include "BZ2Compression.h"
#include "GZCompression.h"
#include "LZ4Compression.h"
#include <Core/RefPtr.h>
#include <FragCore.h>
#include <Math/NormalDistribution.h>
#include <array>
#include <cstddef>
#include <gtest/gtest.h>

using namespace fragcore;

template <class T> class CompressionTest : public ::testing::Test {};
TYPED_TEST_SUITE_P(CompressionTest);

TYPED_TEST_P(CompressionTest, CreateInstance) { ASSERT_NO_THROW(TypeParam()); }

TYPED_TEST_P(CompressionTest, WriteDeflate) {
	std::array<uint8_t, 8192> deflated_buffer{0};
	std::array<uint8_t, 8192> inflated_buffer{0};

	RandomUniform<float> uniform(0, 1, ::time(nullptr));

	std::vector<float> randomData(512, 0xF0);
	const size_t original_data_size = randomData.size() * sizeof(randomData[0]);
	// uniform.random(randomData);

	TypeParam *compressor = nullptr;
	ASSERT_NO_THROW(compressor = new TypeParam());

	ssize_t deflated_size = 0;
	ASSERT_NO_THROW(deflated_size = compressor->deflate(randomData.data(), original_data_size, deflated_buffer.data(),
														deflated_buffer.size()));
	ASSERT_GT(deflated_size, 0);
	ASSERT_LE(deflated_size, original_data_size);

	ssize_t inflated_size = 0;
	ASSERT_NO_THROW(inflated_size = compressor->inflate(deflated_buffer.data(), deflated_size, inflated_buffer.data(),
														inflated_buffer.size()));

	ASSERT_GE(inflated_size, 0);
	ASSERT_EQ(inflated_size, original_data_size);
	ASSERT_NO_THROW(delete compressor);
}

REGISTER_TYPED_TEST_SUITE_P(CompressionTest, CreateInstance, WriteDeflate);

using CompressionTypes = ::testing::Types<BZ2Compression, GZCompression, LZ4Compression>;
INSTANTIATE_TYPED_TEST_SUITE_P(CompressionInternal, CompressionTest, CompressionTypes);