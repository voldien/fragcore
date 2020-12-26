
#ifndef _FRAGVIEW_INTERNAL_OBJECT_TYPE_H_
#define _FRAGVIEW_INTERNAL_OBJECT_TYPE_H_ 1
#include"Renderer/RenderDesc.h"
#include"Renderer/Buffer.h"
#include"Renderer/IRenderer.h"
#include<SDL2/SDL.h>
#include <Renderer/RendererWindow.h>

namespace fragcore {

	/**
	 *
	 */
	typedef struct OpenGLCore_t {
		void *openglcontext;
		SDL_Window *tpmwindow;
		fragcore::RendererWindow *drawwindow;

		bool useCoreProfile;
		bool useCompatibility;

		/*  */
		//TODO determine if to use pool or something .
		Buffer *pboUnPack;
		Buffer *pboPack;

		/*  Cached internal capabilities.   */
		Capability capability;
		TextureDesc::Compression compression;

		/*  Context version.    */
		int majorVersion;
		int minorVersion;
		int profile;
		int cflag;

		bool debug;
		bool alpha;

		ViewPort *defaultViewport;
		std::vector<ViewPort *> viewports;   //TODO remove pointer.
		FrameBuffer *defaultFrameBuffer;
		/*  Texture gamma corrections.  */
		bool gamma;
		ShaderLanguage supportedLanguages;
	} OpenGLCore;

	typedef struct opengl_viewport_t {
		unsigned int viewport;
	} GLViewPort;

	/**
	 *
	 */
	typedef struct opengl_texture_object_t {
		TextureDesc desc;
		unsigned int target;
		unsigned int texture;
		unsigned int pbo;   /*  TODO resolve if to relocate.    */
		Sampler *sampler;
	} GLTextureObject;

	typedef struct opengl_sampler_object_t {
		unsigned int sampler;
	} GLSamplerObject;

	/**
	 *
	 */
	typedef struct opengl_buffer_object_t {
		BufferDesc desc;
		unsigned int target;
		unsigned int buffer;
		unsigned int base;
	} GLBufferObject;

	typedef struct opengl_program_pipeline_t {
		unsigned int program;

		Shader *v, *f, *g, *tc, *te, *c;
	} GLProgramPipeline;

	/**
	 *
	 */
	typedef struct opengl_shader_object_t {
		unsigned int program;
		unsigned int shader;
	} GLShaderObject;

	/**
	 *
	 */
	typedef struct opengl_geometry_object_t {
		GeometryDesc desc;
		unsigned int mode;
		unsigned int vao;
		unsigned int indicesType;
		Buffer *indicesbuffer;
		Buffer *vertexbuffer;
	} GLGeometryObject;

	/**
	 *
	 */
	typedef struct opengl_framebuffer_t {
		FrameBufferDesc desc;
		Texture *textures;
		unsigned int numtextures;
		unsigned int framebuffer;

	} GLFrameBufferObject;

	typedef struct opengl_query_t {
		unsigned int query[8];
	} GLQuery;

	typedef struct opengl_sync_t {
		void *sync;
	} GLSync;

}

//TODO change name to resolve potential symbol name conflict.
/**
 * Helper functions.
 */
extern void addMarkerLabel(const fragcore::OpenGLCore *glcore, unsigned int identifier,
		unsigned int object, const fragcore::MarkerDebug *debug);

extern void resetErrorFlag(void);
extern unsigned int getWrapMode(fragcore::SamplerDesc::AddressMode mode);

extern unsigned int getFilterMode(fragcore::SamplerDesc::FilterMode mode, fragcore::SamplerDesc::FilterMode mips);

extern unsigned int getCompareMode(fragcore::SamplerDesc::CompareFunc mode);

extern unsigned int getGraphicFormat(fragcore::GraphicFormat graphicFormat);
extern unsigned int getTextureFormat(fragcore::TextureFormat textureFormat, unsigned int* pixelType);

extern unsigned int getTextureFormat(fragcore::TextureDesc::Format format);
extern unsigned int getInternalTextureFormat(fragcore::TextureDesc::Format format, bool sRGB,
                                             fragcore::TextureDesc::Compression compression, fragcore::TextureDesc::Type type);

extern unsigned int getTextureTarget(fragcore::TextureDesc::Target target, int nrSamples);

extern unsigned int getTextureType(fragcore::TextureDesc::Type type);

extern unsigned int getTextureSwizzle(fragcore::TextureDesc::Swizzle swizzle);

extern unsigned int getBufferType(fragcore::BufferDesc::BufferType type);

extern unsigned int getBufferHint(fragcore::BufferDesc::BufferHint hint);

extern unsigned int getPrimitive(fragcore::GeometryDesc::Primitive primitive);

extern unsigned int getAttributeDataType(fragcore::GeometryDesc::AttributeType type);

extern unsigned int getState(fragcore::IRenderer::State state);

extern unsigned int getTextureFilterModeNoMip(fragcore::Texture::FilterMode format);

// FrameBuffer
extern unsigned int getBlendEqu(fragcore::FrameBuffer::BlendEqu equ);

extern unsigned int getBlendFunc(fragcore::FrameBuffer::BlendFunc func);

extern unsigned int getClearBitMask(fragcore::CLEARBITMASK clearbitmask);

// Texture.
extern unsigned int getTextureWrapMode(fragcore::Texture::WrapMode mode);

extern unsigned int getTextureFilterMode(fragcore::Texture::FilterMode mode);

extern unsigned int getTextureCompareMode(fragcore::Texture::CompareFunc compareFunc);

extern unsigned getImageInternalFormat(fragcore::Texture::Format format);

extern unsigned int getAccess(fragcore::Texture::MapTarget target);

extern unsigned int getTextureGLFormat(fragcore::Texture::Format format);

extern fragcore::Texture::Format getTextureFormatUserSpace(unsigned int format);


#endif
