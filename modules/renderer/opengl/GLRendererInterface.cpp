#include "GLRendererInterface.h"
#include "Core/IConfig.h"
#include "FragDef.h"
#include "GLRenderWindow.h"
#include <../RenderDesc.h>
#include <../RendererWindow.h>
#include <../Sampler.h>
#include <../Sync.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDLWindowManager.h>
#include <fmt/core.h>
using namespace fragcore;

// GL_NV_gpu_program4: SM 4.0 or better.
// GL_NV_vertex_program3: SM 3.0 or better.
// GL_ARB_fragment_program: SM 2.0 or better

/*  ARB extension requirements.   */
static const char *minRequiredExtensions[] = {
	//"GL_ARB_vertex_program",
	//"GL_ARB_fragment_shader",   /*  Shader model 2.0.   */
	//"GL_ARB_vertex_shader",
	"GL_ARB_texture_non_power_of_two", "GL_ARB_shader_texture_lod", "GL_ARB_texture_swizzle",
	"GL_ARB_vertex_array_object",	   "GL_ARB_framebuffer_object", "GL_ARB_draw_buffers",
};
const unsigned int numMinReqExtensions = sizeof(minRequiredExtensions) / sizeof(minRequiredExtensions[0]);

static const char *reqConfigKey[] = {
	"core", "gamma-correction", "alpha", "debug", "opengl", "anti-aliasing-samples", "anti-aliasing",
};
const unsigned int numReqConfigKeys = sizeof(reqConfigKey) / sizeof(reqConfigKey[0]);

void GLRendererInterface::onInitialization() {}
void GLRendererInterface::onDestruction() {}

bool GLRendererInterface::supportCompute() noexcept { return this->capability.sComputeShader; }

IConfig GLRendererInterface::getDefaultConfig() const noexcept {
	IConfig defaultConfig;
	defaultConfig.set("core", true);
#if NDEBUG
	defaultConfig.set("debug", false);
#else
	defaultConfig.set("debug", true);
#endif
	defaultConfig.set("alpha", false);
	defaultConfig.set("opengl", -1);
	defaultConfig.set("anti-aliasing-samples", 0);
	defaultConfig.set("anti-aliasing", false);
	defaultConfig.set("gamma-correction", false);
	return defaultConfig;
}

GLRendererInterface::GLRendererInterface(const IConfig *config) {

	SDL_Window *window = nullptr;
	GLenum status;

	this->setName("OpenGL");

	IConfig setupConfig;
	if (config == nullptr) {
		setupConfig = this->getDefaultConfig();
	} else {
		setupConfig = *config;
	}

	// Check all required config variables.
	for (unsigned int i = 0; i < numReqConfigKeys; i++) {
		if (!setupConfig.isSet(reqConfigKey[i])) {
			throw RuntimeException("None valid configuration node - missing attribute {}", reqConfigKey[i]);
		}
	}

	/*	*/
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
		throw RuntimeException("SDL_InitSubSystem failed, {}", SDL_GetError());
	}

	/*  */
	bool useCoreProfile = setupConfig.get<bool>("core");
	this->gamma = setupConfig.get<bool>("gamma-correction");
	this->alpha = setupConfig.get<bool>("alpha");
	this->debug = setupConfig.get<bool>("debug");

	/*	Default framebuffer.	*/
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, SDL_TRUE);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, this->alpha ? 8 : 0);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
	SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, this->gamma ? SDL_TRUE : SDL_FALSE);

	/*	Set default framebuffer Multisampling.	*/
	if (setupConfig.get<bool>("anti-aliasing")) {
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, SDL_TRUE);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, setupConfig.get<int>("anti-aliasing-samples"));
	} else {
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, SDL_FALSE);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
	}

	/*  Context profile type.   */
	int contextProfileMask;
	if (useCoreProfile && !debug) {
		contextProfileMask = SDL_GL_CONTEXT_PROFILE_CORE;
	} else {
		contextProfileMask = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, contextProfileMask);

	/*  Enable Debug mode if enabled.   */
	int curDefaultFlag;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &curDefaultFlag);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, curDefaultFlag | (this->debug ? (SDL_GL_CONTEXT_DEBUG_FLAG) : 0));

	/*  Set OpenGL version. */
	if (setupConfig.get<int>("opengl") > 0) {
		const int gmajor = setupConfig.get<int>("opengl") / 100;
		const int gminor = (setupConfig.get<int>("opengl") / 10) % 10;
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gmajor);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gminor);

		this->majorVersion = gmajor;
		this->minorVersion = gminor;
	} else {
		// Assume the highest version.
		this->majorVersion = 4;
		this->minorVersion = 6;

		/*  */
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, this->majorVersion);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, this->minorVersion);
	}

	/*	Create tmp window.	*/
	window = SDL_CreateWindow("", 0, 0, 1, 1, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
	this->tpmwindow = window;

	if (window == nullptr) {
		throw RuntimeException("Could not create an OpenGL window - {}.", SDL_GetError());
	}

	/*	Create OpenGL context.	*/
	this->openglcontext = SDL_GL_CreateContext(window);
	if (this->openglcontext == nullptr) {
		const unsigned int validGLVersion[][2] = {
			{4, 6}, {4, 5}, {4, 4}, {4, 3}, {4, 2}, {4, 1}, {4, 0}, {3, 3},
			{3, 2}, {3, 1}, {3, 0}, {2, 1}, {2, 0}, {1, 4}, {1, 3}, {1, 2},
		};
		const size_t nValidVersions = sizeof(validGLVersion) / sizeof(validGLVersion[0]);

		for (size_t i = 0; i < nValidVersions; i++) {
			const unsigned int *nthValidGLVersion = &validGLVersion[i][0];
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, nthValidGLVersion[0]);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, nthValidGLVersion[1]);

			this->majorVersion = nthValidGLVersion[0];
			this->minorVersion = nthValidGLVersion[1];
			this->openglcontext = SDL_GL_CreateContext(window);
			if (this->openglcontext != nullptr) {
				break;
			}
		}

		/*	Attempt to create compatible.	*/
		/*  Using a top down approach   .    */
		if (!this->openglcontext) {
			/*	*/
			for (unsigned int i = 0; i < nValidVersions; i++) {
				const unsigned int *nthValidGLVersion = &validGLVersion[i][0];
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, nthValidGLVersion[0]);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, nthValidGLVersion[1]);
				this->majorVersion = nthValidGLVersion[0];
				this->minorVersion = nthValidGLVersion[1];

				// Attempt Compatibility.
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
				this->openglcontext = SDL_GL_CreateContext(window);
				if (this->openglcontext) {
					break;
				}
			}

			/*	Last attempt creating an ES profile context.	*/
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
			this->openglcontext = SDL_GL_CreateContext(window);

			// SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
			SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &this->majorVersion);
			SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &this->minorVersion);
			this->profile = false;
			this->useCompatibility = false;

			/*  Final check.    */
			if (this->openglcontext == nullptr) {
				SDL_DestroyWindow(window);
				throw RuntimeException("Failed to create compatibility context: {} - OpenGLVersion: {}.{}.",
									   SDL_GetError(), this->majorVersion, this->minorVersion);
			}
		}
	}

	// TODO improve.
	int profile = 0, cflag = 0;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
	glGetIntegerv(GL_CONTEXT_FLAGS, &cflag);
	this->profile = profile;
	this->cflag = cflag;
	/*  Set final attributes.   */
	this->useCoreProfile = profile == GL_CONTEXT_CORE_PROFILE_BIT;

	/*  Get the version if supported. Otherwise use the SDL and convert it.   */
	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	if (major > 0) {
		this->majorVersion = major * 100;
	} else {
		this->majorVersion *= 100;
	}
	if (minor >= 0) {
		this->minorVersion = minor * 10;
	} else {
		this->minorVersion *= 10;
	}

	/*  Set for core support or not for GLEW.   */
	const bool UseGLExperiments = debug;
	if (UseGLExperiments) {
		glewExperimental = GL_TRUE;
	} else {
		glewExperimental = GL_FALSE;
	}

	/*	Init GLEW library.	*/
	status = glewInit();
	if (status != GLEW_OK) {
		SDL_GL_DeleteContext(this->openglcontext);
		SDL_DestroyWindow(window);
		throw RuntimeException("Could not Initialize GLEW - {}.", (const char *)glewGetErrorString(status));
	}

	/*  TODO add function for checking if context is supported. */
	for (unsigned int i = 0; i < numMinReqExtensions; i++) {
		const char *extension = minRequiredExtensions[i];
		if (!glewIsExtensionSupported(extension)) {
			throw RuntimeException("Non supported GPU - {} using OpenGL version: {}\nGLSL: {}", extension, getVersion(),
								   (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
		}
	}

	/*  Get internal capability list and cache it.   */
	this->getCapability(&this->capability);
	this->getSupportedTextureCompression(&this->compression);

	// Determine and cache the supported shader languages.
	if (glewIsExtensionSupported("GL_ARB_gl_spirv")) {
		this->supportedLanguages = (ShaderLanguage)(GLSL | SPIRV);
	} else {
		this->supportedLanguages = GLSL;
	}

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	this->setDebug(this->debug);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

GLRendererInterface::~GLRendererInterface() {

	/*  Release PBO.    */

	/*  Release default framebuffer.    */
	// delete this->defaultFrameBuffer;
	//	this->deleteFrameBuffer(this->defaultFrameBuffer);

	/*  Unbind opengl context.  */
	SDL_GL_MakeCurrent(nullptr, nullptr);

	if (this->openglcontext) {
		SDL_GL_DeleteContext(this->openglcontext);
	}

	/*	*/
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

RendererWindow *GLRendererInterface::createWindow(int window_x, int window_y, int width, int height) {

	Ref<GLRendererInterface> rendRef(this);

	GLRenderWindow *renderWindow = new GLRenderWindow(rendRef);

	renderWindow->setPosition(window_x, window_y);
	renderWindow->setSize(width, height);

	/*	Cleanup.	*/
	if (this->tpmwindow) {
		SDL_DestroyWindow(static_cast<SDL_Window *>(this->tpmwindow));
		this->tpmwindow = nullptr;
	}

	return renderWindow;
}

void GLRendererInterface::setCurrentWindow(RendererWindow *window) {

	// window->useWindow(this->openglcontext);
	this->drawwindow = window;
}

void GLRendererInterface::createSwapChain() {}

static void default_callback_debug_gl(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
									  const GLchar *message, GLvoid *userParam) {

	const char *_source;
	const char *_type;
	const char *_severity;

	switch (source) {
	case GL_DEBUG_SOURCE_API:
		_source = "API";
		break;

	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		_source = "WINDOW SYSTEM";
		break;

	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		_source = "SHADER COMPILER";
		break;

	case GL_DEBUG_SOURCE_THIRD_PARTY:
		_source = "THIRD PARTY";
		break;

	case GL_DEBUG_SOURCE_APPLICATION:
		_source = "APPLICATION";
		return;

	case GL_DEBUG_SOURCE_OTHER:
		_source = "UNKNOWN";
		break;

	default:
		_source = "UNKNOWN";
		break;
	}

	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		_type = "ERROR";
		break;

	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		_type = "DEPRECATED BEHAVIOR";
		break;

	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		_type = "UDEFINED BEHAVIOR";
		break;

	case GL_DEBUG_TYPE_PORTABILITY:
		_type = "PORTABILITY";
		break;

	case GL_DEBUG_TYPE_PERFORMANCE:
		_type = "PERFORMANCE";
		break;

	case GL_DEBUG_TYPE_OTHER:
		_type = "OTHER";
		break;

	case GL_DEBUG_TYPE_MARKER:
		_type = "MARKER";
		break;

	default:
		_type = "UNKNOWN";
		break;
	}

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		_severity = "HIGH";
		break;

	case GL_DEBUG_SEVERITY_MEDIUM:
		_severity = "MEDIUM";
		break;

	case GL_DEBUG_SEVERITY_LOW:
		_severity = "LOW";
		break;

	case GL_DEBUG_SEVERITY_NOTIFICATION:
		_severity = "NOTIFICATION";
		break;

	default:
		_severity = "UNKNOWN";
		break;
	}

	if (std::string(_severity) != "NOTIFICATION") {
		printf("OpenGL error [%d]: %s of %s severity, raised from %s: %s\n", id, _type, _severity, _source, message);
	}
}

void GLRendererInterface::setDebug(bool enable) {

	/*	TODO CLEAN.	*/
	using __glewDebugMessageCallback = void (*)(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar *, GLvoid *);
	using PROC = void (*)(__glewDebugMessageCallback, void *);
	PROC callback;

	/*	Load function pointer by their symbol name.	*/
	callback = (PROC)SDL_GL_GetProcAddress("glDebugMessageCallback");

	if (enable) {

		/*	Set Debug message callback.	*/
		if (callback) {
			callback(default_callback_debug_gl, nullptr);
		}

		/*	*/
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	} else {

		if (callback) {
			callback(default_callback_debug_gl, nullptr);
		}

		glDisable(GL_DEBUG_OUTPUT);
		glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}
}

const char *GLRendererInterface::getShaderVersion(ShaderLanguage language) const {

	if (language == GLSL) {
		/*  Lookup table.   */
		int glVersion = this->majorVersion + this->minorVersion;
		switch (glVersion) {
		case 200:
			return "110";
		case 210:
			return "120";
		case 300:
			return "130";
		case 310:
			return "140";
		case 320:
			return "150";
		case 330:
			return "330";
		case 400:
			return "400";
		case 410:
			return "410";
		case 420:
			return "420";
		case 430:
			return "430";
		case 440:
			return "440";
		case 450:
			return "450";
		case 460:
			return "460";
		// Add the rest of the mapping as each new version is released.
		default:
			throw RuntimeException("OpenGL version {} is not supported", glVersion);
		}
	}
	if (language == SPIRV) {
		return "460";
	}
	throw InvalidArgumentException("Invalid shader language");
}

ShaderLanguage GLRendererInterface::getShaderLanguage() const { return this->supportedLanguages; }

const char *GLRendererInterface::getAPIVersion() const { return (const char *)glGetString(GL_VERSION); }

const char *GLRendererInterface::getVersion() const { return FV_STR_VERSION(1, 0, 0); }

void GLRendererInterface::getSupportedTextureCompression(TextureDesc::Compression *pCompressions) {

	if (pCompressions == nullptr) {
		throw InvalidArgumentException("Compressions may not be a null pointer.");
	}

	unsigned int compressions = 0;

	// TODO add and improve.
	if (glewIsExtensionSupported("GL_ARB_texture_compression_bptc")) {
		compressions |= (unsigned int)TextureDesc::Compression::BPTC;
	}
	if (glewIsExtensionSupported("GL_ANGLE_texture_compression_dxt1")) {
		compressions |= (unsigned int)TextureDesc::Compression::DXT1;
	}
	if (glewIsExtensionSupported("GL_EXT_texture_compression_rgtc") ||
		glewIsExtensionSupported("GL_ARB_texture_compression_rgtc")) {
		compressions |= (unsigned int)TextureDesc::Compression::RGTC;
	}
	if (glewIsExtensionSupported("GL_ANGLE_texture_compression_dxt5")) {
		compressions |= (unsigned int)TextureDesc::Compression::DXT5;
	}
	if (glewIsExtensionSupported("GL_ATI_texture_compression_3dc")) {
		compressions |= (unsigned int)TextureDesc::Compression::_3DC;
	}
	if (glewIsExtensionSupported("GL_EXT_texture_compression_s3tc")) {
		compressions |= (unsigned int)TextureDesc::Compression::S3TC;
	}
	if (glewIsExtensionSupported("GL_ANGLE_texture_compression_dxt3")) {
		compressions |= (unsigned int)TextureDesc::Compression::DXT3;
	}
	if (glewIsExtensionSupported("GL_KHR_texture_compression_astc_ldr")) {
		compressions |= (unsigned int)TextureDesc::Compression::ASTC_LDR;
	}

	// Add support for default compression fmt::format.
	if (compressions != 0) {
		compressions |= (unsigned int)TextureDesc::Compression::Compression;
	}

	*pCompressions = (TextureDesc::Compression)compressions;
}

void GLRendererInterface::getCapability(Capability *capability) {

	assert(capability);

	if (capability == nullptr) {
		throw std::invalid_argument("Capability object may not a null pointer.");
	}

	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &capability->sMaxVertexAttributes);
	if (glGetInteger64v) {
		glGetInteger64v(GL_MAX_ELEMENTS_VERTICES, &capability->sMaxElementVertices);
		glGetInteger64v(GL_MAX_ELEMENTS_INDICES, &capability->sMaxElementIndices);
	} else {
		capability->sMaxElementVertices = 0;
		capability->sMaxElementIndices = 0;
		glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, (GLint *)&capability->sMaxElementVertices);
		glGetIntegerv(GL_MAX_ELEMENTS_INDICES, (GLint *)&capability->sMaxElementIndices);
	}

	/*	TODO resolve*/
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &capability->sMaxTextureUnitActive);		/*	GL_MAX_TEXTURE_IMAGE_UNITS	*/
	glGetIntegerv(GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS, &capability->numMaxSubRoutines); /*	*/

	/*  Compute shader. */
	glGetIntegerv(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS, &capability->sMaxComputeTextureImageUnits);
	glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &capability->sMaxComputeSharedMemory);

	capability->sWorkGroupDimensions = 3;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_COUNT, capability->sMaxWorKGroupSize);
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_SIZE, capability->sMaxLocalWorkGroupSize);

	/*	*/
	glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &capability->sMaxFrameBufferWidth);
	glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &capability->sMaxFrameBufferHeight);
	glGetIntegerv(GL_MAX_FRAMEBUFFER_LAYERS, &capability->sMaxFrameBufferLayers);
	glGetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, &capability->sMaxFrameBufferSamples);

	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &capability->sMaxDrawBuffers);
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &capability->sMaxColorAttachments);
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &capability->sMaxFrameBufferAttachment); /*	*/

	/*	*/
	glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &capability->sNumCompressedTexture);
	glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &capability->sMaxTextureBufferSize);
	glGetIntegerv(GL_MAX_TEXTURE_LOD_BIAS, &capability->sMaxTextureLODBias);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &capability->sMaxTextureSize);
	glGetIntegerv(GL_MAX_COLOR_TEXTURE_SAMPLES, &capability->sMaxTextureSamples);
	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &capability->sMaxTextureArrayLayers);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &capability->sMaxTextureSize);
	glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &capability->sMaxTexture3DSize);
	glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &capability->sMaxCubeTextureSize);
	glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &capability->sMaxAnisotropy);
	//
	/*	Uniform buffer max.	*/
	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &capability->sMaxUniformBufferBinding);
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &capability->sMaxUniformBlockSize);
	glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &capability->sMaxUniformLocations);

	/*	*/
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS, &capability->sMaxViewPortDims);
	glGetIntegerv(GL_MAX_VIEWPORTS, &capability->sMaxViewPorts);
	glGetIntegerv(GL_VIEWPORT_BOUNDS_RANGE, &capability->sViewPortBoundRange[0]);

	// GL_ALIASED_LINE_WIDTH_RANGE, GL_SMOOTH_LINE_WIDTH_RANGE, and GL_SMOOTH_LINE_WIDTH_GRANULARITY.
	glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, capability->lineWidthRange);
	glGetFloatv(GL_SMOOTH_LINE_WIDTH_GRANULARITY, &capability->lineWidthGranularity);

	glGetIntegerv(GL_MIN_MAP_BUFFER_ALIGNMENT, &capability->sMinMapBufferAlignment);

	/*  */
	capability->sTransformFeedback =
		glewIsExtensionSupported("GL_EXT_transform_feedback") || glewIsExtensionSupported("GL_NV_transform_feedback");

	/*  */
	capability->sTextureCompression = capability->sNumCompressedTexture > 0;

	/*  Shaders.    */
	capability->sVertexShader = glewIsExtensionSupported("GL_ARB_vertex_program");
	capability->sFragmentShader = glewIsExtensionSupported("GL_ARB_fragment_shader") ||
								  glewIsExtensionSupported("GL_ARB_fragment_program") ||
								  glewIsExtensionSupported("GL_ATI_fragment_shader");
	capability->sGeometryShader = glewIsExtensionSupported("GL_ARB_geometry_shader4");
	capability->sComputeShader = glewIsExtensionSupported("GL_ARB_compute_shader");
	capability->sTessellation = glewIsExtensionSupported("GL_ARB_tessellation_shader");

	/*  */
	capability->sDepthStencil = glewIsExtensionSupported("GL_ARB_stencil_texturing,");
	capability->sFramebuffer = glewIsExtensionSupported("GL_ARB_framebuffer_object") ||
							   glewIsExtensionSupported("GL_ARB_framebuffer_sRGB") ||
							   glewIsExtensionSupported("GL_EXT_framebuffer_object");

	capability->sFrameBufferMSAA = glewIsExtensionSupported("GL_EXT_framebuffer_multisample");
	capability->sShaderPrecision = glewIsExtensionSupported("GL_ARB_shader_precision");
	//	capability->sNonAttachableFrameBuffer = false;
	capability->sMSAA = glewIsExtensionSupported("GLX_ARB_multisample") || capability->sFrameBufferMSAA;
	capability->sCubeMap = glewIsExtensionSupported("GL_ARB_texture_cube_map");
	capability->sCubeMapArray = glewIsExtensionSupported("GL_ARB_texture_cube_map_array");
	capability->sTextureArray = glewIsExtensionSupported("GL_EXT_texture_array");
	capability->sQueryOcclusion =
		glewIsExtensionSupported("GL_ARB_occlusion_query") || glewIsExtensionSupported("GL_NV_occlusion_query");
	capability->sQueryTime = glewIsExtensionSupported("GL_ARB_timer_query");
	capability->sInstancing =
		glewIsExtensionSupported("GL_ARB_draw_instanced") || glewIsExtensionSupported("GL_EXT_draw_instanced");

	capability->sIndirectDraw = glewIsExtensionSupported("GL_ARB_draw_indirect"); // GL_NV_bindless_multi_draw_indirect
	capability->sIndirectMultiDraw = glewIsExtensionSupported("GL_ARB_multi_draw_indirect") ||
									 glewIsExtensionSupported("GL_AMD_multi_draw_indirect");

	capability->sUniformBuffer = glewIsExtensionSupported("GL_ARB_uniform_buffer_object");
	capability->sShaderStorageBuffer = glewIsExtensionSupported("GL_ARB_shader_storage_buffer_object");

	capability->rayTracing = glewIsExtensionSupported("GL_NV_ray_tracing");

	// GL_ARB_multisample
	capability->sVirtualViewPort = glewIsExtensionSupported("GL_ARB_viewport_array") &&
								   (glewIsExtensionSupported("GL_NV_viewport_array2") ||
									glewIsExtensionSupported("GL_ARB_shader_viewport_layer_array"));
	capability->sSRGB = glewIsExtensionSupported("GL_ARB_framebuffer_sRGB") ||
						glewIsExtensionSupported("GL_EXT_framebuffer_sRGB") ||
						glewIsExtensionSupported("GL_EXT_texture_sRGB");
	capability->sConditionalRendering =
		glewIsExtensionSupported("GL_NV_conditional_render") || glewIsExtensionSupported("GL_NVX_conditional_render");

	GLint nShaderBinary;
	glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, &nShaderBinary);
	capability->sShaderBinary = nShaderBinary > 0 && glewIsExtensionSupported("GL_ARB_get_program_binary");
}

void GLRendererInterface::getStatus(MemoryInfo *memoryInfo) {

	GLint dedicatedVRMem;

	/*	GL_ATI_meminfo	*/
	glGetIntegerv(GL_VBO_FREE_MEMORY_ATI,
				  &dedicatedVRMem); // dedicated video memory, total size (in kb) of the GPU memory
	glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI,
				  &dedicatedVRMem); // dedicated video memory, total size (in kb) of the GPU memory
	glGetIntegerv(GL_RENDERBUFFER_FREE_MEMORY_ATI,
				  &dedicatedVRMem); // dedicated video memory, total size (in kb) of the GPU memory

	/*	GL_NVX_gpu_memory_info	*/
	glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX,
				  &dedicatedVRMem); // dedicated video memory, total size (in kb) of the GPU memory
	memoryInfo->totalVRam = dedicatedVRMem;
	glGetIntegerv(
		GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX,
		&dedicatedVRMem); // total available memory, total size (in Kb) of the memory available for allocations

	/*	*/
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX,
				  &dedicatedVRMem); // current available dedicated video memory (in kb),
	memoryInfo->currentVRam = dedicatedVRMem;
	glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTION_COUNT_NVX, &dedicatedVRMem); // count of total evictions seen by system
	glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &dedicatedVRMem); // size of total video memory evicted (in kb)
}

void GLRendererInterface::getFeatures(Features *features) {

	memcpy(features, &this->features, sizeof(Features));

	features->raytracing = false;
	features->variableRateShading = glewIsExtensionSupported("GL_NV_shading_rate_image");
	features->floatVertex = glewIsExtensionSupported("GL_ARB_half_float_vertex");
	features->halfpixel = glewIsExtensionSupported("GL_ARB_half_float_pixel");
}

const char *GLRendererInterface::getExtensions() const noexcept { return (const char *)glGetString(GL_EXTENSIONS); }

bool GLRendererInterface::isExtensionSupported(const char *extension) const noexcept {
	return glewIsExtensionSupported(extension);
}

void *GLRendererInterface::getOpenGLContext() noexcept { return this->openglcontext; }
void GLRendererInterface::bindWindowContext(void *window, void *context) {}

void *GLRendererInterface::getData() const { return this->pdata; }

extern "C" IRenderer *createInternalRenderer(IConfig *config) { return new GLRendererInterface(config); }
