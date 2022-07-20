
#include "internal_object_type.h"
#include <fmt/core.h>
#include <limits>
using namespace fragcore;

// TODO move to the helper file.
unsigned int getTextureFormat(TextureDesc::Format format) {
	switch (format) {
	case TextureDesc::eRGB:
		return VK_FORMAT_R8G8B8_UNORM;
	case TextureDesc::eRGBA:
		return VK_FORMAT_R8G8B8A8_UNORM;
	case TextureDesc::eBGR:
		return VK_FORMAT_B8G8R8_UNORM;
	case TextureDesc::eBGRA:
		return VK_FORMAT_B8G8R8A8_UNORM;
	case TextureDesc::eSRGB:
		return VK_FORMAT_B8G8R8_SRGB;
	case TextureDesc::eSRGBA:
		return VK_FORMAT_B8G8R8A8_UNORM;
	case TextureDesc::eSingleColor:
		return VK_FORMAT_R8_UNORM;
	case TextureDesc::eDepth:
		return VK_FORMAT_R8_UNORM;
	case TextureDesc::eStencil:
		return VK_FORMAT_R8_UNORM;
	case TextureDesc::eDepthStencil:
		return VK_FORMAT_R8G8_UNORM;
	default:
		throw InvalidArgumentException("Invalid texture format");
	}
}

unsigned int getTextureTarget(TextureDesc::Target target) {
	switch (target) {
	case TextureDesc::Target::Texture1D:
		return VK_IMAGE_TYPE_1D;
	case TextureDesc::Target::Texture2D:
		return VK_IMAGE_TYPE_2D;
	case TextureDesc::Target::Texture3D:
		return VK_IMAGE_TYPE_3D;
	case TextureDesc::Target::CubeMap:
	default:
		break;
	}
	throw InvalidArgumentException("Invalid Texture target");
}

unsigned int getTextureType(TextureDesc::Type type) {
	return 0;
}

unsigned int getBufferType(BufferDesc::BufferType type) {
	switch (type) {
	case BufferDesc::eArray:
		return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	case BufferDesc::eElementArray:
		return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	case BufferDesc::eUniform:
		return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	case BufferDesc::eShaderStorage:
		return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	case BufferDesc::ePixelUnpack:
		return VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	case BufferDesc::ePixelPack:
		return VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	default:
		assert(0);
		break;
	}
	throw InvalidArgumentException("Invalid Texture target");
}

unsigned int getBufferHint(BufferDesc::BufferHint hint) { return 0; }

unsigned int getPrimitive(GeometryDesc::Primitive primitive) {
	switch (primitive) {
	case GeometryDesc::Primitive::Point:
		return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
	case GeometryDesc::Primitive::Lines:
		return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	case GeometryDesc::Primitive::Triangles:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	case GeometryDesc::Primitive::TriangleStrips:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	default:
		return -1;
	}
}

unsigned int getAttributeDataType(GeometryDesc::AttributeType type) { return 0; }

unsigned int getState(unsigned int state) { return 0; }
