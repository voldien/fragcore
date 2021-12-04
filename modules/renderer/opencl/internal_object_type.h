/*
 *	FragCore - Core Framework Functionalities for Game Engines
 *	Copyright (C) 2018  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _FRAGCORE_INTERNAL_OPENCL_OBJECT_TYPE_H_
#define _FRAGCORE_INTERNAL_OPENCL_OBJECT_TYPE_H_ 1
#include "Renderer/Buffer.h"
#include "Renderer/IRenderer.h"
#include "Renderer/RenderDesc.h"
#include <CL/opencl.h>
#include <SDL2/SDL.h>

namespace fragcore {

	enum CLRendererInterface {
		eNone,
		eVulkan,
		eOpenGL,
		eDirectX,
		eCustom,
	};

	typedef struct opencl_core_t {
		IRenderer *back_renderer;
		void *window; // TODO evoluate.
		cl_context context;
		cl_command_queue *queues;
		uint32_t num_queues;
		cl_command_queue selectqueue;
		cl_device_id *devices;
		cl_device_id selectDevice;
		CLRendererInterface anInterface;
		unsigned int ndevices;

		/*  */
		Shader *current;
	} OpenCLCore;

	/**
	 *
	 */
	typedef struct opencl_texture_object_t {
		TextureDesc desc;
		unsigned int target;
		cl_mem texture;
		Texture *interp;
	} CLTextureObject;

	/**
	 *
	 */
	typedef struct opencl_shader_object_t {
		cl_program program;
	} CLShaderObject;

	/**
	 *
	 */
	typedef struct opencl_buffer_object_t {
		BufferDesc desc;
		cl_mem target;
		unsigned int buffer;
		unsigned int base;
		Buffer *interp;
	} CLBufferObject;

	/**
	 *
	 */
	typedef struct opencl_geometry_object_t {
		GeometryDesc desc;
		unsigned int mode;
		unsigned int vao;
		unsigned int indicesType;
		Buffer *indicesbuffer;
		Buffer *vertexbuffer;
	} CLGeometryObject;

	/**
	 *
	 */
	typedef struct opengl_framebuffer_object_t {
		FrameBufferDesc desc;
		unsigned int framebuffer;
	} CLFrameBufferObject;
} // namespace fragcore

#endif
