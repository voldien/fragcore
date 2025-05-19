/*
 *	FragCore - Fragment Core - Engine Core
 *	Copyright (C) 2018 Valdemar Lindberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program;
 */
#ifndef _FRAGCORE_VULKAN_INTERNAL_OBJECT_TYPE_H_
#define _FRAGCORE_VULKAN_INTERNAL_OBJECT_TYPE_H_ 1
/*	Expose the correct platform specific surface creation functions.	*/

// #if defined(FV_UNIX)
// #define VK_USE_PLATFORM_XLIB_KHR
// #define VK_USE_PLATFORM_WAYLAND_KHR
// #elif defined(FV_WINDOW)
// #define VK_USE_PLATFORM_WIN32_KHR
// #elif defined(FV_ANDROID)
// #define VK_USE_PLATFORM_ANDROID_KHR
// #elif defined(FV_MACOSX)
// #define VK_USE_PLATFORM_MACOS_MVK
// #endif

#include <SDL2/SDL.h>
#include <VKDevice.h>
#include <vulkan/vulkan.h>

namespace fragcore {
	class VKBuffer;
	class VKTexture;
	class VKSampler;
	class VKShader;
	class VKFrameBuffer;
	class VKViewport;
	class VKQuery;
	class VKCommandList;
	class VKRenderPipeline;
	class VKRenderInterface;
} // namespace fragcore

#define ArraySize(a) (sizeof(a) / sizeof(*a))
namespace fragcore {

	// extern FVDECLSPEC unsigned int getTextureFormat(fragcore::TextureDesc::Format format);

	// extern FVDECLSPEC unsigned int getTextureTarget(fragcore::TextureDesc::Target target);

	// extern FVDECLSPEC unsigned int getTextureType(fragcore::TextureDesc::Type type);

	// extern FVDECLSPEC unsigned int getBufferType(fragcore::BufferDesc::BufferType type);

	// extern FVDECLSPEC unsigned int getBufferHint(fragcore::BufferDesc::BufferHint hint);

	// extern FVDECLSPEC unsigned int getPrimitive(fragcore::GeometryDesc::Primitive primitive);

	// extern FVDECLSPEC unsigned int getAttributeDataType(fragcore::GeometryDesc::AttributeType type);

	// extern FVDECLSPEC unsigned int getState(unsigned int state);

} // namespace fragcore

#endif
