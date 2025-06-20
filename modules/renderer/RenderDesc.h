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
#ifndef _FRAGCORE_RENDER_DESC_H_
#define _FRAGCORE_RENDER_DESC_H_ 1
#include "GraphicFormat.h"
#include "RenderPrerequisites.h"
#include "ShaderLanguage.h"
#include <ImageFormat.h>
#include <cstddef>
#include <cstdint>

namespace fragcore {

	/**
	 *
	 */
	using IndirectDrawArray = struct indirect_draw_array_t {
		unsigned int count;			/*  */
		unsigned int instanceCount; /*  */
		unsigned int first;			/*  */
		unsigned int baseInstance;	/*  */
	};

	/**
	 *
	 */
	using IndirectDrawElement = struct indirect_draw_element_t {
		unsigned int count;			/*  */
		unsigned int instanceCount; /*  */
		unsigned int firstIndex;	/*  */
		int baseVertex;				/*  */
		unsigned int baseInstance;	/*  */
	};

	/**
	 *
	 */
	using IndirectDispatch = struct indirect_dispatch_t {
		unsigned int num_groups_x;
		unsigned int num_groups_y;
		unsigned int num_groups_z;
	};

	/**
	 *
	 */
	enum class TextureSwizzle : uint32_t {
		NoSwizzle = 0x0, /*  */
		Zero = 0x1,		 /*  */
		One = 0x2,		 /*  */
		Red = 0x3,		 /*  */
		Green = 0x4,	 /*  */
		Blue = 0x5,		 /*  */
		eAlpha = 0x6,	 /*  */
	};

	/**
	 *
	 */
	enum class TextureFilterMode : uint32_t {
		NoFilterMode, /*  */
		Nearset,	  /*  */
		Linear,		  /*  */
		Trilinear,	  /*	*/
	};

	/**
	 *
	 */
	// TODO: refractor name
	enum class TextureUVMappingMode : uint32_t {
		UV,		/*  */
		Sphere, /*  */
		Box,	/*  */
		Cylinder,
		Plane,
		Other
	};

	/**
	 *
	 */
	enum class TextureWrappingMode : uint32_t {
		NoAddressMode,
		Repeat,		  /*  */
		RepeatMirror, /*  */
		Clamp,		  /*  */
		ClampBorder,  /*  */
	};

	enum class TextureCompareFunc : uint32_t {
		eNoCompare, /*  */
		lessEqual,
		greaterEqual,
		less,
		greater,
		equal,
		notequal,
		always,
		never,
	};

	using MarkerDebug = struct marker_debug_t {
		const char *markerName;
	};

	using SamplerDesc = struct sampler_desc_t {

		/*  Sampler.    */
		float anisotropy;				/*  */
		TextureFilterMode minFilter;	/*  */
		TextureFilterMode magFilter;	/*  */
		TextureWrappingMode AddressU;	/*  */
		TextureWrappingMode AddressV;	/*  */
		TextureWrappingMode AddressW;	/*  */
		TextureFilterMode mipmapFilter; /*  */
		/*  Set swizzle.    */
		TextureSwizzle Swizzler;		/*  */
		TextureSwizzle Swizzleg;		/*  */
		TextureSwizzle Swizzleb;		/*  */
		TextureSwizzle Swizzlea;		/*  */
		int maxLOD;						/*  */
		int minLOD;						/*  */
		int biasLOD;					/*  */
		float borderColor[4];			/*  */
		int compareMode;				/*  */
		TextureCompareFunc compareFunc; /*  */
		/*  Debug attributes.   */
		MarkerDebug *marker; // TODO: add support for adding after creating the object.
	};

	/**
	 *	Texture descriptor.
	 */
	using TextureDesc = struct texture_desc_t {
		enum class Target : uint32_t {
			Texture1D = 0x1,	   /*	1D texture.	*/
			Texture2D = 0x2,	   /*	2D texture.	*/
			Texture3D = 0x4,	   /*	3D texture.	*/
			CubeMap = 0x8,		   /*	Cubemap texture.	*/
			CubeMapArray = 0x10,   /*	Cubemap array texture.	*/
			Texture2DArray = 0x20, /*	2D Texture Array.*/
		};

		/**
		 *	Texture format.
		 */
		enum class DataPixelFormat : uint32_t {
			NoFormat = 0,
			RGB = 0x1,	/*	RGB components.	*/
			RGBA = 0x2, /*	RGBA components.	*/
			BGR = 0x3,	/*	BGR components.	*/
			BGRA = 0x4, /*	BGRA components.	*/
			SRGB = 0x5, /*	SRGB components.	*/
			RG = 0x6,
			A = 0x8,
			SRGBA = 0x7,		/*	SRGBA components.	*/
			SingleColor = 0x9,	/*	Single color component.	*/
			Depth = 0xA,		/*	Depth component.	*/
			Stencil = 0xB,		/*	Stencil component.	*/
			DepthStencil = 0xC, /*	Depth and stencil componets.	*/
		};

		/**
		 *	Texture pixel type data.
		 */
		enum class PixelDataType : uint32_t {
			NoType = 0,
			UnsignedByte = 0x1, /*	Each color component encoded in a single byte.	*/
			SignedByte = 0x2,	/*	Each color component encoded in a single signed byte.	*/
			UnsignedShort = 0x3,
			SignedShort = 0x4,
			UnsignedInt = 0x5,
			SignedInt = 0x6,
			Float = 0x7,		/*	Each color component encoded in a single float.	*/
			HalfFloat = 0x8,	/*  */
			Double = 0x9,		/*  */
			Unsigned24_8 = 0xA, /*	Each color component encoded.	*/
		};

		/**
		 *	Texture compression types.
		 */
		enum class Compression : uint32_t {
			NoCompression = 0x0, /*	Non-compressed texture.	*/
			Compression = 0x1,	 /*	Compressed texture using default compression.   */
			RGTC = 0x2,			 /*  */
			DXT1 = 0x4,			 /*  */
			DXT3 = 0x8,			 /*  */
			DXT4 = 0x10,		 /*  */
			DXT5 = 0x20,		 /*  */
			S3TC = 0x20,		 /*  */
			BPTC = 0x40,		 /*  */
			_3DC = 0x80,		 /*  */
			ASTC_LDR = 0x100,	 /*  */
			ETC2 = 0x200,		 /*  */
		};

		Texture *originalTexture; /*  Use viewport.   */

		/*  Target. */
		Target target; /*	Texture target.	*/
		/*	*/
		int width = 1;	/*	Texture width in pixels.	*/
		int height = 1; /*	Texture height in pixels.	*/
		int depth = 1;	/*	Texture depth in pixels.	*/

		/*	Internal format.	*/
		GraphicFormat graphicFormat; /*  */
		bool immutable;				 /*	*/

		/*  */
		unsigned int numlevel = 0;	 /*	Number of mipmaps.	*/
		unsigned int usemipmaps = 0; /*	Using mipmaps	*/
		unsigned int nrSamples = 0;	 /*	Enable Multisampling.   */

		/*  Set swizzle.    */
		TextureSwizzle Swizzler; /*  */
		TextureSwizzle Swizzleg; /*  */
		TextureSwizzle Swizzleb; /*  */
		TextureSwizzle Swizzlea; /*  */

		/*	Input format.	*/
		ImageFormat pixelFormat; /*  */

		/*	Texture pixel data.	*/
		union {
			const void *pixel;		  /*  Pixel pointer.  */
			const void *cubepixel[6]; /*	Cubemap pixel point references. */
			const void *pixelarray;	  /*  */
		};
		size_t pixelSize;	 /*  Image size in bytes.    */
		SamplerDesc sampler; /*  Texture sampling.   */

		/*  Debug attributes.   */
		MarkerDebug *marker;
	};

	/**
	 *
	 */
	using RenderPipelineDesc = struct program_pipeline_desc_t {
		// TODO add binding.
		Shader *v;
		Shader *f;
		Shader *g;
		Shader *tc;
		Shader *te;
		Shader *c;
		/*  Debug attributes.   */
		MarkerDebug marker;
	};

	enum class ShaderType {
		Unknown,
		Vertex,	 /*  */
		Frag,	 /*  */
		Geom,	 /*  */
		TesseC,	 /*  */
		TesseE,	 /*  */
		Compute, /*  */
		Mesh,
		Task,
		RayHit,
		RayMiss
	};

	enum class ShaderCodeType {
		ShaderTypeUndefined, /*  Undefined.	*/
		SourceCode,			 /*  Source code.    */
		Binary,				 /*  Binary code.    */
	};

	/**
	 *	Shader descriptor.
	 */
	using ShaderDesc = struct shader_desc_t {

		struct {
			const void *pdata;	 /*	*/
			size_t binarySize;	 /*	*/
			unsigned int format; /*	*/
		} program;

		// TODO add binary format.
		/*  Rendering pipeline shaders. */
		struct {
			union {
				const char **vertexsource; /*	Vertex shader sources.	*/
				const char *vertexBinary;  /*	Vertex shader sources.	*/
			};
			union {
				unsigned int numvert; /*	Number of vertex shader string sources.	*/
				size_t size;
			};
			unsigned int binaryFormat;
			ShaderCodeType type;
			ShaderLanguage language;
		} vertex;
		struct {
			union {
				const char **fragmentsource; /*	Fragment shader sources.	*/
				const char *fragmentBinary;	 /*	Fragment shader sources.	*/
			};
			union {
				unsigned int numfrag; /*	Number of vertex shader string sources.	*/
				size_t size;
			};
			unsigned int binaryFormat;
			ShaderCodeType type;
			ShaderLanguage language;
		} fragment;
		struct {
			union {
				const char **geometrysource; /*	Vertex shader sources.	*/
				const char *geometryBinary;	 /*	Vertex shader sources.	*/
			};
			union {
				unsigned int numgeo; /*	Number of vertex shader string sources.	*/
				size_t size;
			};
			unsigned int binaryFormat;
			ShaderCodeType type;
			ShaderLanguage language;
		} geometry;
		struct {
			union {
				const char **tessellationco;	  /*	Vertex shader sources.	*/
				const char *tessellationcoBinary; /*	Vertex shader sources.	*/
			};
			union {
				unsigned int numtesco; /*	Number of vertex shader string sources.	*/
				size_t size;
			};
			unsigned int binaryFormat;
			ShaderCodeType type;
			ShaderLanguage language;
		} tessellationControl;

		struct {
			union {
				const char **tessellationev;	 /*	Vertex shader sources.	*/
				const char *tessellationeBinary; /*	Vertex shader sources.	*/
			};
			union {
				unsigned int numtesev; /*	Number of vertex shader string sources.	*/
				size_t size;
			};
			unsigned int binaryFormat;
			ShaderCodeType type;
			ShaderLanguage language;
		} tessellationEvolution;

		struct {
			union {
				const char **computeSource; /*	Vertex shader sources.	*/
				const char *computeBinary;	/*	Vertex shader sources.	*/
			};
			union {
				unsigned int numcompute; /*	Number of vertex shader string sources.	*/
				size_t size;
			};
			unsigned int binaryFormat;
			ShaderCodeType type;
			ShaderLanguage language;
		} Compute;

		bool separatetable; /*	TODO evoluate if supported on all the API.	*/

		/*  Debug attributes.   */
		MarkerDebug *marker;
	};

	/**
	 *	Buffer descriptor.
	 */
	using BufferDesc = struct buffer_desc_t {
		/**
		 *
		 */
		enum BufferType : uint32_t {
			eArray,				/*	*/
			eElementArray,		/*	*/
			eUniform,			/*	*/
			eTexture,			/*  */
			eShaderStorage,		/*  */
			eTransformFeedback, /*  */
			ePixelUnpack,		/*  */
			ePixelPack,			/*  */
			eIndirectDraw,		/*  */
			eIndirectDispatch	/*  */
		};

		/**
		 *	Hint the usage of the
		 *	buffer object.
		 */
		enum BufferHint : uint32_t {
			eRead = 0x1,	/*  Reading from the buffer.    */
			eWrite = 0x2,	/*  Writing to the buffer.  */
			eStatic = 0x4,	/*  Written once.   */
			eDynamic = 0x8, /*  Will be updated frequently.    */
			eStream = 0x10, /*  */
		};

		enum BufferLoc {

		};

		unsigned int type; /*	Buffer data type.	*/
		size_t size;	   /*	Buffer size in bytes.	*/
		void *data;		   /*	Buffer host data pointer.	*/
		BufferHint hint;   /*	Buffer hint of usages.	*/
		/*  Debug attributes.   */
		MarkerDebug *marker;
	};

	/**
	 *	Framebuffer description.
	 */
	using FrameBufferDesc = struct framebuffer_desc_t {
		unsigned int nrAttachments;
		Texture **attach; /*	Texture attachment.	*/

		Texture *depth;		   /*	Depth attachment.	*/
		Texture *stencil;	   /*	Stencil attachment.	*/
		Texture *depthstencil; /*	depth stencil attachment.	*/
		/*  Debug attributes.   */
		MarkerDebug *marker; // TODO add as pointer.
	};

	/**
	 *	Geometry description.
	 */
	using GeometryDesc = struct geometry_desc_t {
		enum class Primitive : uint32_t {
			Point = 0x1,			/**/
			Lines = 0x2,			/**/
			Triangles = 0x3,		/**/
			TriangleStrips = 0x4,	/**/
			TriangleAdjacant = 0x5, /**/
		};

		enum class AttributeType {
			eFloat = 0x1,  /*	*/
			eInt = 0x2,	   /*	*/
			eDouble = 0x3, /*  */
			eHalf = 0x4,   /*  */
		};

		/*	*/
		unsigned int primitive; /*	Primitive type.	*/
		size_t numVerticecs;	/*	Number of vertices.	*/
		size_t numIndices;		/*	Number of indices.	*/
		size_t indicesStride;	/*	Size per indices in bytes.	*/
		size_t vertexStride;	/*	Size per vertex in bytes.	*/

		/*  */
		void *indices; /*	Indices host pointer data.	*/
		void *buffer;  /*	Vertex buffer.	*/

		/*	*/
		unsigned int numVertexAttributes; /*	Number of vertex attributes */
		struct {
			unsigned int index;	   /*	Index of vertex attributes.	*/
			size_t size;		   /*	Size in numbers of components (1 - 4).	*/
			unsigned int datatype; /*	Data type ( AttributeType ) */
			unsigned int offset;   /*	Offset in bytes.	*/
		} vertexattribute[16];

		/*	bound.	*/
		bool autoBound;

		/*  Debug attributes.   */
		MarkerDebug *marker;
	};

	/**
	 *	Query description.
	 */
	using QueryDesc = struct query_desc_t {
		/*  Debug attributes.   */
		MarkerDebug *marker;
	};

	using SyncDesc = struct sync_desc_t {
		/*  Debug attributes.   */
		MarkerDebug *marker;
	};

	using MemoryInfo = struct memory_info_t {
		long int totalVRam;
		long int currentVRam;
	};

	using RendererInfo = struct renderer_info_t {
		MemoryInfo memoryInfo;
	};

	/**
	 *
	 */
	using Features = struct features_t {
		bool computeShaders;
		bool geomtryShader;
		bool tesselationShader;
		bool raytracing;
		bool multiviewports;
		bool samplerLodBias;
		bool drawBaseVertex;
		bool drawBaseInstance;
		bool drawIndirect;
		bool drawIndirectBaseInstance;
		bool independentBlend;
		bool mapBuffer;
		bool commandListDebugMarkers;
		bool variableRateShading;
		bool shaderFloat64;
		bool ansi;
		bool floatVertex;
		bool halfpixel;
	};

	/**
	 *
	 */
	using Capability = struct compatibility_t {
		bool windowRenderer;
		bool sDepthStencil; /*  */
		bool sFramebuffer;
		bool sFrameBufferMSAA; /*  */
		bool sShaderPrecision;
		bool sNonAttachableFrameBuffer;
		bool sMSAA; /*  */
		bool sCSAA;
		bool sCubeMap;
		bool sCubeMapArray;	  /*  */
		bool sTextureArray;	  /*  */
		bool sMultiBind;	  /*	Support multi bind texture.	*/
		bool sQueryOcclusion; /*  */
		bool sQueryTime;
		bool sRenderTarget;			  /*  */
		bool sShaderImageLoadStorage; /*  */
		bool sShaderStorageBuffer;	  /*  */
		bool sUniformBuffer;
		bool sMapBuffer;
		bool sInstancing;	/*  Instance rendering supported.   */
		bool sIndirectDraw; /*  */
		bool sIndirectMultiDraw;
		bool sSpritePoint;		   /*  Point sprite supported. */
		bool sRenderTargetCubeMap; /*  */
		bool sVertexShader;
		bool sFragmentShader;	  /*  */
		bool sGeometryShader;	  /*  */
		bool sTessellation;		  /*  */
		bool sComputeShader;	  /*  */
		bool sTransformFeedback;  /*  */
		bool sTextureCompression; /*  */
		bool sSRGB;
		bool rayTracing;
		bool sVirtualViewPort;
		bool sConditionalRendering;
		bool sShaderBinary;
		bool sVirtualTexture; /*  */
		bool sVariableRateShading;

		/*	*/
		long int sMaxElementVertices;
		long int sMaxElementIndices;
		int sMaxVertexAttributes;

		/*	*/

		int sMaxTextureUnitActive; /**/
		int numMaxSubRoutines;	   /**/

		/*	*/
		int sMaxUniformBufferBinding; /*		GL_MAX_UNIFORM_BUFFER_BINDINGS
									   */
		int sMaxUniformBlockSize;	  /*		GL_MAX_UNIFORM_BLOCK_SIZE
									   */
		int sMaxUniformLocations;	  /*		GL_MAX_UNIFORM_LOCATIONS
									   */

		/*  Textures.   */
		int sNumCompressedTexture;
		int sMaxTextureBufferSize;
		int sMaxTextureLODBias;
		int sMaxTextureSamples;
		int sMaxTextureSize;
		int sMaxTexture3DSize;
		int sMaxTextureArrayLayers;
		int sMaxCubeTextureSize;
		int sMaxAnisotropy;

		/*  Viewports.  */
		int sMaxViewPortDims;
		int sMaxViewPorts;
		int sViewPortBoundRange[2];

		/*  Render target.  */
		int sMaxFrameBufferWidth;
		int sMaxFrameBufferHeight;
		int sMaxFrameBufferLayers;
		int sMaxFrameBufferSamples;
		int sMaxDrawBuffers;		   /*	GL_MAX_DRAW_BUFFERS	*/
		int sMaxColorAttachments;	   /*	GL_MAX_COLOR_ATTACHMENTS
										*/
		int sMaxFrameBufferAttachment; /**/

		/*  Compute shaders. */
		int sMaxComputeTextureImageUnits;
		int sMaxComputeSharedMemory;
		int sMaxLocalWorkGroupSize[3];
		int sMaxWorKGroupSize[3];
		int sWorkGroupDimensions;

		/*  Line.   */
		float lineWidthGranularity;
		float lineWidthRange[2];

		int sMinMapBufferAlignment;
	};

	/**
	 *
	 */
	using DeviceInfo = struct device_info_t {
		const char *name;
		const int deviceType;
		const char *vendor;
		Capability capability;
	};
} // namespace fragcore

#endif
