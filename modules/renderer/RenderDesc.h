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
#ifndef _FRAG_CORE_RENDER_DESC_H_
#define _FRAG_CORE_RENDER_DESC_H_ 1
#include "GraphicFormat.h"
#include "RenderPrerequisites.h"
#include <ImageFormat.h>

namespace fragcore {
	/**
	 *  Support languages.
	 */
	enum ShaderLanguage {
		GLSL = 0x1,			   /*  OpenGL GLSL.    */
		SPIRV = 0x2,		   /*  SPIRV.  */
		HLSL = 0x4,			   /*  High Level Shading Language.    */
		CLC = 0x8,			   /*  OpenCL C language.  */
		CG = 0x10,			   /*  Legacy. */
		unKnownLanguage = 0x0, /*	*/
	};

	typedef struct marker_debug_t {
		const char *markerName;
	} MarkerDebug;

	typedef struct memory_info_t {
		long int totalVRam;
		long int currentVRam;
	} MemoryInfo;

	typedef struct renderer_info_t {
		MemoryInfo memoryInfo;
	} RendererInfo;

	typedef struct sampler_desc_t {
		/**
		 *
		 */
		enum class FilterMode {
			NoFilterMode, /*  */
			Linear,		  /*  */
			Nearset,	  /*  */
		};

		/**
		 *
		 */
		enum AddressMode {
			NoAddressMode,
			Repeat,		  /*  */
			RepeatMirror, /*  */
			Clamp,		  /*  */
			ClampBorder,  /*  */
		};

		enum CompareFunc {
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

		/*  Sampler.    */
		float anisotropy;		 /*  */
		FilterMode minFilter;	 /*  */
		FilterMode magFilter;	 /*  */
		AddressMode AddressU;	 /*  */
		AddressMode AddressV;	 /*  */
		AddressMode AddressW;	 /*  */
		FilterMode mipmapFilter; /*  */
		int maxLOD;				 /*  */
		int minLOD;				 /*  */
		int biasLOD;			 /*  */
		float borderColor[4];	 /*  */
		int compareMode;		 /*  */
		CompareFunc compareFunc; /*  */

		/*  Debug attributes.   */
		MarkerDebug marker; // TODo add support for adding after creating the object.
	} SamplerDesc;

	/**
	 *	Texture descriptor.
	 */
	typedef struct texture_desc_t {
		enum Target {
			eTexture1D = 0x1,		/*	1D texture.	*/
			eTexture2D = 0x2,		/*	2D texture.	*/
			eTexture3D = 0x4,		/*	3D texture.	*/
			eCubeMap = 0x8,			/*	Cubemap texture.	*/
			eCubeMapArray = 0x10,	/*	Cubemap array texture.	*/
			eTexture2DArray = 0x20, /*  */
		};

		/**
		 *	Texture format.
		 */
		enum Format { // GraphicsFormat
			eNoFormat,
			eRGB = 0x1,	 /*	RGB components.	*/
			eRGBA = 0x2, /*	RGBA components.	*/
			eBGR = 0x3,	 /*	BGR components.	*/
			eBGRA = 0x4, /*	BGRA components.	*/
			eSRGB = 0x5, /*	SRGB components.	*/
			eRG = 0x6,
			eA = 0x8,
			eSRGBA = 0x7,		 /*	SRGBA components.	*/
			eSingleColor = 0x9,	 /*	Single color component.	*/
			eDepth = 0xA,		 /*	Depth component.	*/
			eStencil = 0xB,		 /*	Stencil component.	*/
			eDepthStencil = 0xC, /*	Depth and stencil componets.	*/
		};

		/**
		 *	Texture pixel type data.
		 */
		enum Type {
			eNoType,
			eUnsignedByte = 0x1, /*	Each color component encoded in a single byte.	*/
			eSignedByte = 0x2,	 /*	Each color component encoded in a single signed byte.	*/
			eUnsignedShort = 0x3,
			eSignedShort = 0x4,
			eUnsignedInt = 0x5,
			eSignedInt = 0x6,
			eFloat = 0x7,		 /*	Each color component encoded in a single float.	*/
			eHalfFloat = 0x8,	 /*  */
			eDouble = 0x9,		 /*  */
			eUnsigned24_8 = 0xA, /*	Each color component encoded.	*/
		};

		/**
		 *	Texture compression types.
		 */
		enum Compression {
			eNoCompression = 0x0, /*	Non-compressed texture.	*/
			eCompression = 0x1,	  /*	Compressed texture using default compression.   */
			eRGTC = 0x2,		  /*  */
			eDXT1 = 0x4,		  /*  */
			eDXT3 = 0x8,		  /*  */
			eDXT4 = 0x10,		  /*  */
			eDXT5 = 0x20,		  /*  */
			eS3TC = 0x20,		  /*  */
			eBPTC = 0x40,		  /*  */
			e3DC = 0x80,		  /*  */
			eASTC_LDR = 0x100,	  /*  */
			eETC2 = 0x200,		  /*  */
		};

		Texture *originalTexture; /*  Use viewport.   */

		/*  Target. */
		unsigned int target; /*	Texture target.	*/
		unsigned int width;	 /*	Texture width in pixels.	*/
		unsigned int height; /*	Texture height in pixels.	*/
		unsigned int depth;	 /*	Texture depth in pixels.	*/

		/*  */
		// Input format.
		// TODO add support for loading compressed data.
		// TODO add support for texture internal data type.
		unsigned int type; /*	Texture data type.	*/

		// TODO REMOVE
		unsigned int format; /*	Texture input data type.	*/

		// TODO Remove
		unsigned int internalformat; /*	Texture internal data type.	*/
		TextureFormat pixelFormat;	 /*  */
		GraphicFormat graphicFormat; /*  */

		bool immutable; /**/

		/*  */
		unsigned int numlevel;	 /*	Number of mipmaps.	*/
		unsigned int usemipmaps; /*	Using mipmaps?	*/
		unsigned int nrSamples;	 /*  Enable Multisampling.   */
		// TODO decouple to the graphic fvformatf.
		unsigned int compression; /*	What Compression to use.	*/
		// TODO decouple to the graphic fvformatf.
		unsigned int srgb; /*  Enable gamma correction.    */

		enum Swizzle {
			eNoSwizzle = 0x0, /*  */
			eZero = 0x1,	  /*  */
			eOne = 0x2,		  /*  */
			eRed = 0x3,		  /*  */
			eGreen = 0x4,	  /*  */
			eBlue = 0x5,	  /*  */
			eAlpha = 0x6,	  /*  */
		};

		/*  Set swizzle.    */
		Swizzle Swizzler; /*  */
		Swizzle Swizzleg; /*  */
		Swizzle Swizzleb; /*  */
		Swizzle Swizzlea; /*  */

		/*	Texture pixel data.	*/
		union {
			const void *pixel; /*  Pixel pointer.  */
			struct {
				const void *cubepixel[6]; /*	Cubemap pixel point references. */
			};
			const void *pixelarray; /*  */
		};
		unsigned long pixelSize; /*  Image size in bytes.    */
		SamplerDesc sampler;	 /*  Texture sampling.   */

		/*  Debug attributes.   */
		MarkerDebug marker;
	} TextureDesc;

	/**
	 *
	 */
	typedef struct program_pipeline_desc_t {
		// TODO add binding.
		Shader *v;
		Shader *f;
		Shader *g;
		Shader *tc;
		Shader *te;
		Shader *c;
		/*  Debug attributes.   */
		MarkerDebug marker;
	} ProgramPipelineDesc;

	// TOOD rename all enum without the e infront.
	enum ShaderType {
		eUnknownShader,
		eVertex,  /*  */
		eFrag,	  /*  */
		eGeom,	  /*  */
		eTesseC,  /*  */
		eTesseE,  /*  */
		eCompute, /*  */
	};

	enum ShaderCodeType {
		eNoShaderType, /*  Undefined.	*/
		eSourceCode,   /*  Source code.    */
		eBinary,	   /*  Binary code.    */
	};

	/**
	 *	Shader descriptor.
	 */
	typedef struct shader_desc_t {

		struct {
			const void *pdata;	 /*	*/
			int binarySize;		 /*	*/
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
				unsigned int size;
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
				unsigned int size;
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
				unsigned int size;
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
				unsigned int size;
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
				unsigned int size;
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
				unsigned int size;
			};
			unsigned int binaryFormat;
			ShaderCodeType type;
			ShaderLanguage language;
		} Compute;

		bool separatetable; /*	TODO evoluate if supported on all the API.	*/

		/*  Debug attributes.   */
		MarkerDebug marker;
	} ShaderDesc;

	/**
	 *	Buffer descriptor.
	 */
	typedef struct buffer_desc_t {
		/**
		 *
		 */
		enum BufferType {
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
		enum BufferHint {
			eRead = 0x1,	/*  Reading from the buffer.    */
			eWrite = 0x2,	/*  Writing to the buffer.  */
			eStatic = 0x4,	/*  Written once.   */
			eDynamic = 0x8, /*  Will be updated frequently.    */
			eStream = 0x10, /*  */
		};

		enum BufferLoc {

		};

		/**
		 *
		 */
		unsigned int type; /*	Buffer data type.	*/
		unsigned int size; /*	Buffer size in bytes.	*/
		void *data;		   /*	Buffer host data pointer.	*/
		BufferHint hint;   /*	Buffer hint of usages.	*/
		/*  Debug attributes.   */
		MarkerDebug marker;
	} BufferDesc;

	/**
	 *	Framebuffer description.
	 */
	typedef struct framebuffer_desc_t {
		Texture *attach[16];   /*	Texture attachment.	*/
		Texture *depth;		   /*	Depth attachment.	*/
		Texture *stencil;	   /*	Stencil attachment.	*/
		Texture *depthstencil; /*	depth stencil attachment.	*/
		/*  Debug attributes.   */
		MarkerDebug marker;
	} FrameBufferDesc;

	// TODO determine if packing compiler qualifier needed.
	/**
	 *
	 */
	typedef struct indirect_draw_array_t {
		unsigned int count;			/*  */
		unsigned int instanceCount; /*  */
		unsigned int first;			/*  */
		unsigned int baseInstance;	/*  */
	} IndirectDrawArray;

	/**
	 *
	 */
	typedef struct indirect_draw_element_t {
		unsigned int count;			/*  */
		unsigned int instanceCount; /*  */
		unsigned int firstIndex;	/*  */
		unsigned int baseVertex;	/*  */
		unsigned int baseInstance;	/*  */
	} IndirectDrawElement;

	typedef struct indirect_dispatch_t {
		unsigned int num_groups_x;
		unsigned int num_groups_y;
		unsigned int num_groups_z;
	} IndirectDispatch;

	/**
	 *	Geometry description.
	 */
	typedef struct geometry_desc_t {
		enum Primitive {
			ePoint = 0x1,			 /**/
			eLines = 0x2,			 /**/
			eTriangles = 0x3,		 /**/
			eTriangleStrips = 0x4,	 /**/
			eTriangleAdjacant = 0x5, /**/
		};

		enum AttributeType {
			eFloat = 0x1,  /*	*/
			eInt = 0x2,	   /*	*/
			eDouble = 0x3, /*  */
			eHalf = 0x4,   /*  */
		};

		/*	*/
		unsigned int primitive;		/*	Primitive type.	*/
		unsigned int numVerticecs;	/*	Number of vertices.	*/
		unsigned int numIndices;	/*	Number of indices.	*/
		unsigned int indicesStride; /*	Size per indices in bytes.	*/
		unsigned int vertexStride;	/*	Size per vertex in bytes.	*/

		/*  */
		void *indices; /*	Indices host pointer data.	*/
		void *buffer;  /*	Vertex buffer.	*/

		/*	*/
		unsigned int numVertexAttributes; /*	Number of vertex attributes */
		struct {
			unsigned int index;	   /*	Index of vertex attributes.	*/
			unsigned int size;	   /*	Size in numbers of components (1 - 4).	*/
			unsigned int datatype; /*	Data type ( AttributeType ) */
			unsigned int offset;   /*	Offset in bytes.	*/
		} vertexattribute[16];

		/*	bound.	*/
		bool autoBound;

		/*  Debug attributes.   */
		MarkerDebug marker;
	} GeometryDesc;

	/**
	 *	Query description.
	 */
	typedef struct query_desc_t {
		/*  Debug attributes.   */
		MarkerDebug marker;
	} QueryDesc;

	typedef struct sync_desc_t {
		/*  Debug attributes.   */
		MarkerDebug marker;
	} SyncDesc;

	/**
	 *
	 */
	typedef struct features_t {
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
	} Features;

	/**
	 *
	 */
	typedef struct compatibility_t {
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
		bool sShadow;			   /*  */
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
		bool variableRateShading;

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
	} Capability;

	/**
	 *
	 */
	// TODO improve.
	typedef struct device_info_t {
		const char *name;
		const int deviceType;
		const char *vendor;
		Capability capability;
	} DeviceInfo;
} // namespace fragcore

#endif
