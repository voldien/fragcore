#include "Core/IConfig.h"
#include "Renderer/opengl/GLRendererInterface.h"
#include "Renderer/RenderPipeline.h"
#include "Renderer/ViewPort.h"
#include "Renderer/opengl/GLRenderWindow.h"
#include "Renderer/opengl/internal_object_type.h"
#include"Renderer/opengl/GLCommandList.h"
#include"Renderer/FrameBuffer.h"
#include <Exception/InvalidArgumentException.h>
#include <Exception/NotImplementedException.h>
#include <Exception/RuntimeException.h>
#include <GL/glew.h>
#include <Renderer/RenderDesc.h>
#include <Renderer/RendererWindow.h>
#include <Renderer/Sampler.h>
#include <Renderer/Sync.h>
#include <Renderer/opengl/DefaultFrameBufferTexture.h>
#include <SDL2/SDL.h>
#include <Utils/StringUtil.h>
#include <Window/WindowManager.h>
#include <exception>
#include <vector>
using namespace fragcore;

//GL_NV_gpu_program4: SM 4.0 or better.
//GL_NV_vertex_program3: SM 3.0 or better.
//GL_ARB_fragment_program: SM 2.0 or better

/*  ARB extension requirements.   */
static const char *minRequiredExtensions[] = {
		//"GL_ARB_vertex_program",
		//"GL_ARB_fragment_shader",   /*  Shader model 2.0.   */
		//"GL_ARB_vertex_shader",
		"GL_ARB_texture_non_power_of_two",
		"GL_ARB_shader_texture_lod",
		"GL_ARB_texture_swizzle",
		"GL_ARB_vertex_array_object",
		"GL_ARB_framebuffer_object",
		"GL_ARB_draw_buffers",
};
const unsigned int numMinReqExtensions = sizeof(minRequiredExtensions) / sizeof(minRequiredExtensions[0]);

static const char* reqConfigKey[] = {
	"core",
	"gamma-correction",
	"alpha",
	"debug",
	"opengl",
	"anti-aliasing-samples",
	"anti-aliasing",
};
const unsigned int numReqConfigKeys = sizeof(reqConfigKey) / sizeof(reqConfigKey[0]);


void GLRendererInterface::OnInitialization(void) {}
void GLRendererInterface::OnDestruction(void) {}

GLRendererInterface::GLRendererInterface(IConfig *config) {

	OpenGLCore *glcore;
	SDL_Window *window = NULL;
	GLenum status;

	this->setName("OpenGL");

	IConfig setupConfig;
	if(config == NULL){
		setupConfig.set("core", true);
		setupConfig.set("debug", true);
		setupConfig.set("alpha", true);
		setupConfig.set("opengl", -1);
		setupConfig.set("anti-aliasing-samples", 0);
		setupConfig.set("anti-aliasing", false);
		setupConfig.set("gamma-correction", true);
        } else{
		//setupConfig = *config
	}

	// Check all required config variables.
	for (int i = 0; i < numReqConfigKeys; i++){
		if(!setupConfig.isSet(reqConfigKey[i]))
			throw RuntimeException(fvformatf("None valid configuration node - missing attribute %s", reqConfigKey[i]));
	}

	/*  Allocate Opengl internal.	*/
	glcore = new OpenGLCore();
	assert(glcore);
	this->pdata = glcore;

	if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
		throw RuntimeException(fvformatf("SDL_InitSubSystem failed, %s.\n", SDL_GetError()));

	/*  */
	bool useCoreProfile = setupConfig.get<bool>("core");
	glcore->gamma = setupConfig.get<bool>("gamma-correction");
	glcore->alpha  = setupConfig.get<bool>("alpha");
	glcore->debug = setupConfig.get<bool>("debug");

	/*	Default framebuffer.	*/
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, SDL_TRUE);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, glcore->alpha ? 8 : 0);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
	SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, glcore->gamma ? SDL_TRUE : SDL_FALSE);

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
	if(useCoreProfile)
		contextProfileMask = SDL_GL_CONTEXT_PROFILE_CORE;
	else
		contextProfileMask = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY;
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, contextProfileMask);

	/*  Enable Debug mode if enabled.   */
	int curDefaultFlag;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &curDefaultFlag);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, curDefaultFlag |
	                                          (glcore->debug ? (SDL_GL_CONTEXT_DEBUG_FLAG | SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG) : 0));

	/*  Set OpenGL version. */
	if (setupConfig.get<int>("opengl") > 0) {
		const int gmajor = setupConfig.get<int>("opengl") / 100;
		const int gminor = (setupConfig.get<int>("opengl") / 10) % 10;
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gmajor);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gminor);

		glcore->majorVersion = gmajor;
		glcore->minorVersion = gminor;
	} else {
		// Assume the highest version.
		glcore->majorVersion = 4;
		glcore->minorVersion = 6;

		/*  */
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, glcore->majorVersion);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, glcore->minorVersion);
	}

	/*	Create tmp window.	*/
	window = SDL_CreateWindow(
			"", 0, 0, 1, 1,
			SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
	glcore->tpmwindow = window;

	if (window == NULL) {
		throw RuntimeException(fvformatf("Could not create an OpenGL window - %s.", SDL_GetError()));
	}

	/*	Create OpenGL context.	*///TODO add config attribute for auto or force the version.
	//config->get<const char*>("version");
	glcore->openglcontext = SDL_GL_CreateContext(window);
	if (glcore->openglcontext == NULL) {
		const unsigned int validGLVersion[][2] = {
				{4, 6},
				{4, 5},
				{4, 4},
				{4, 3},
				{4, 2},
				{4, 1},
				{4, 0},
				{3, 3},
				{3, 2},
				{3, 1},
				{3, 0},
				{2, 1},
				{2, 0},
				{1, 4},
				{1, 3},
				{1, 2},
		};
		const unsigned int nValidVersions = sizeof(validGLVersion) / sizeof(validGLVersion[0]);


		for (int i = 0; i < nValidVersions; i++) {
			const unsigned int *nthValidGLVersion = &validGLVersion[i][0];
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, nthValidGLVersion[0]);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, nthValidGLVersion[1]);

			glcore->majorVersion = nthValidGLVersion[0];
			glcore->minorVersion = nthValidGLVersion[1];
			glcore->openglcontext = SDL_GL_CreateContext(window);
			if (glcore->openglcontext)
				break;
		}

		/*	Attempt to create compatible.	*/
		/*  Using a top down approach   .    */
		if(!glcore->openglcontext){
			for (int i = 0; i < nValidVersions; i++) {
				const unsigned int *nthValidGLVersion = &validGLVersion[i][0];
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, nthValidGLVersion[0]);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, nthValidGLVersion[1]);
				glcore->majorVersion = nthValidGLVersion[0];
				glcore->minorVersion = nthValidGLVersion[1];

				// Attempt Compatibility.
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
				glcore->openglcontext = SDL_GL_CreateContext(window);
				if (glcore->openglcontext)
					break;
			}

			// Last attempt creating an ES profile context.
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
			glcore->openglcontext = SDL_GL_CreateContext(window);

			//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
			SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &glcore->majorVersion);
			SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &glcore->minorVersion);
			glcore->profile = false;
			glcore->useCompatibility = false;

			/*  Final check.    */
			if (glcore->openglcontext == NULL) {
				SDL_DestroyWindow(window);
				throw RuntimeException(fvformatf("Failed to create compatibility context: %s.", SDL_GetError()));
			}
		}
	}

	//TODO improve.
	int profile, cflag;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
	glGetIntegerv(GL_CONTEXT_FLAGS, &cflag);
	glcore->profile = profile;
	glcore->cflag = cflag;
	/*  Set final attributes.   */
	glcore->useCoreProfile = profile == GL_CONTEXT_CORE_PROFILE_BIT;

	/*  Get the version if supported. Otherwise use the SDL and convert it.   */
	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	if(major > 0)
		glcore->majorVersion = major * 100;
	else
		glcore->majorVersion *= 100;
	if(minor >= 0)
		glcore->minorVersion = minor * 10;
	else
		glcore->minorVersion *= 10;

	/*  Set for core support or not for GLEW.   */
	//TODO resolve.
	if (1)
		glewExperimental = GL_TRUE;
	else
		glewExperimental = GL_FALSE;

	/*	Init GLEW library.	*/
	status = glewInit();
	if (status != GLEW_OK) {
		SDL_GL_DeleteContext(glcore->openglcontext);
		SDL_DestroyWindow(window);
		throw RuntimeException(fvformatf("Could not Initialize GLEW - %s.", glewGetErrorString(status)));
	}

	/*  TODO add function for checking if context is supported. */
	for (int i = 0; i < numMinReqExtensions; i++) {
		const char *extension = minRequiredExtensions[i];
		if (!glewIsExtensionSupported(extension)) {
			throw RuntimeException(
					fvformatf("Non supported GPU - %s using OpenGL version: %s\nGLSL: %s", extension, getVersion(),
					          glGetString(GL_SHADING_LANGUAGE_VERSION)));
		}
	}

	/*  Get internal capability list and cache it.   */
	this->getCapability(&glcore->capability);
	this->getSupportedTextureCompression(&glcore->compression);

	// Determine and cache the supported shader languages.
	if(glewIsExtensionSupported("GL_ARB_gl_spirv"))
		glcore->supportedLanguages = (ShaderLanguage)(GLSL | SPIRV);
	else
		glcore->supportedLanguages = GLSL;

	/*	Set default state.	*/
	this->enableState(eDepthTest);
	this->disableState(eStencilTest);
	this->disableState(eBlend);
	this->enableState(eDither);
	this->enableState(eCullface);
	this->setDepthMask(true);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
	glDepthFunc(GL_LESS);
	glCullFace(GL_FRONT_AND_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glcore->alpha) {
		this->enableState(eAlphaTest);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}
	this->clearColor(0.2f, 0.2f, 0.2f, 1.0f);
	if (glcore->gamma)
		enableState(GLRendererInterface::eSRGB);
	//this->setVSync(config->get<bool>("v-sync"));
	this->setDebug(glcore->debug);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	//TODO improve
	// TODO, add support for version without viewports.
	/*  Create default viewport object. */
	glcore->defaultViewport = new ViewPort();
	//glcore->defaultViewport->iRenderer = this;
	GLViewPort* glViewPort = new GLViewPort();
	glViewPort->viewport = 0;
	//glcore->defaultViewport->pdata = glViewPort;
	for(int i = 0; i < glcore->capability.sMaxViewPorts - 1; i++){
		ViewPort* virtualView = new ViewPort();
		//virtualView->iRenderer = this;
		GLViewPort* glViewPort = new GLViewPort();
		glViewPort->viewport = i + 1;
		//virtualView->pdata = glViewPort;
		glcore->viewports.push_back(virtualView);
	}

	//TODO create default framebuffer.
	//TODO create default texture.
	glcore->defaultFrameBuffer = new FrameBuffer();
	GLFrameBufferObject *frameBufferObject = new GLFrameBufferObject();
	frameBufferObject->framebuffer = 0;
	frameBufferObject->numtextures = 1;
	//glcore->defaultFrameBuffer->pdata = frameBufferObject;
	//glcore->defaultFrameBuffer->iRenderer = this;
	frameBufferObject->desc.attach[0] = new FrameBufferTexture();
	//frameBufferObject->desc.attach[0]->iRenderer = this;
	GLenum drawbuffers[] = { GL_FRONT, GL_BACK };
	glNamedFramebufferDrawBuffers(0, 2, drawbuffers);

	//TODO determine how to utualize the PBO.
	/*  Create PBO for image transfer. */
	BufferDesc pbo = {};
	pbo.size = 0;
	pbo.data = NULL;
	pbo.hint = (BufferDesc::BufferHint)(BufferDesc::eStream | BufferDesc::eWrite);
	pbo.type = BufferDesc::ePixelUnpack;
	glcore->pboUnPack = this->createBuffer(&pbo);

	/*  Create PBO for image transfer. */
	pbo.size = 0;
	pbo.data = NULL;
	pbo.hint = (BufferDesc::BufferHint)(BufferDesc::eStream | BufferDesc::eWrite);
	pbo.type = BufferDesc::ePixelPack;
	glcore->pboPack = this->createBuffer(&pbo);
}

GLRendererInterface::~GLRendererInterface(void) {
	OpenGLCore *glCore = (OpenGLCore *) this->pdata;

	/*  Release PBO.    */

	/*  Release default framebuffer.    */
	this->deleteFrameBuffer(glCore->defaultFrameBuffer);

	//TODO release all the viewports.

	/*  Unbind opengl context.  */
	SDL_GL_MakeCurrent(NULL, NULL);

	//if (glCore->drawwindow)
	//	SDL_DestroyWindow(glCore->drawwindow);

	if (glCore->openglcontext)
		SDL_GL_DeleteContext(glCore->openglcontext);

	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	free(this->pdata);
}

Texture *GLRendererInterface::createTexture(TextureDesc *desc) {
	OpenGLCore *glCore = (OpenGLCore *) this->pdata;

	if (desc->width < 0 && desc->height < 0 && desc->depth < 0)
		throw InvalidArgumentException("Invalid texture dimensions.");
	if (desc->numlevel < 0 && desc->usemipmaps)
		throw InvalidArgumentException("Mips number count must be greater than or equal to 0.");
	if (desc->compression & ~glCore->compression)
		throw InvalidArgumentException(fvformatf("Invalid compression format - %d.", desc->compression));
	if (desc->srgb) {
		if (!glCore->capability.sSRGB)
			throw InvalidArgumentException("sRGB is not supported.");
	}

	Sampler *sampler = NULL;
	GLuint texture;
	GLuint target;
	GLuint format;
	GLuint internalformat;
	GLuint type;

	/*	Extract OpenGL target from desc.	*/
	format = getTextureFormat(desc->pixelFormat, &type);
	internalformat = getGraphicFormat(desc->graphicFormat);
	//
	target = getTextureTarget((TextureDesc::Target) desc->target, desc->nrSamples);

	//TODO add release logic for when failing.

	/*	*/
	glGenTextures(1, &texture);
	if (desc->originalTexture) {
		resetErrorFlag();
		if (glTextureView) {
			const GLTextureObject *textureOriginal = (const GLTextureObject *) desc->originalTexture->getObject();
			glTextureView(texture, target, textureOriginal->texture, internalformat, 0, 1, 0, 1);
			checkError();
			// TODO add reference.
		} else{
			throw RuntimeException("glTextureView not supported");
			}
	} else {
		glBindTexture(target, texture);
		checkError();

		/*	wrap and filter	*/
		glTexParameteri(target, GL_TEXTURE_WRAP_S, getWrapMode(desc->sampler.AddressU));
		checkError();
		glTexParameteri(target, GL_TEXTURE_WRAP_T, getWrapMode(desc->sampler.AddressV));
		checkError();
		glTexParameteri(target, GL_TEXTURE_WRAP_R, getWrapMode(desc->sampler.AddressW));
		checkError();

		glTexParameteri(target, GL_TEXTURE_MIN_FILTER,
		                getFilterMode(desc->sampler.minFilter, desc->sampler.mipmapFilter));
		checkError();
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER,
		                getFilterMode(desc->sampler.magFilter, SamplerDesc::eNoFilterMode));
		checkError();

		const GLint compareMode =
				desc->sampler.compareMode == SamplerDesc::eNoCompare ? GL_NONE : GL_COMPARE_REF_TO_TEXTURE;
		glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, compareMode);
		checkError();

		if (desc->sampler.compareMode){
			glTexParameteri(target, GL_TEXTURE_COMPARE_FUNC, getCompareMode(desc->sampler.compareFunc));
			checkError();
		}

		if (desc->sampler.anisotropy > 0 && glCore->features.ansi) {
			glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, desc->sampler.anisotropy);
			checkError();
		}

		glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, &desc->sampler.borderColor[0]);
		checkError();

		glTexParameteri(target, GL_TEXTURE_MIN_LOD, desc->sampler.maxLOD);
		checkError();

		glTexParameteri(target, GL_TEXTURE_MAX_LOD, desc->sampler.minLOD);
		checkError();

		glTexParameteri(target, GL_TEXTURE_LOD_BIAS, desc->sampler.biasLOD);
		checkError();

		glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
		checkError();

		if(desc->Swizzler != TextureDesc::eNoSwizzle)
			glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, getTextureSwizzle(desc->Swizzler));
		if(desc->Swizzleg != TextureDesc::eNoSwizzle)
			glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, getTextureSwizzle(desc->Swizzleg));
		if(desc->Swizzleb != TextureDesc::eNoSwizzle)
			glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, getTextureSwizzle(desc->Swizzleb));
		if(desc->Swizzlea != TextureDesc::eNoSwizzle)
			glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, getTextureSwizzle(desc->Swizzlea));

		// if (errorStatus != GL_NO_ERROR)
		// 	throw RuntimeException(::fvformatf("Error when setting texture parameters - %d, %s", errorStatus,
		// 	                                   gluErrorString(errorStatus)));

		/*	assign data.	*/
		if (desc->pixel || !desc->immutable) {
			switch (target) {
				case GL_TEXTURE_3D:
					break;
				case GL_TEXTURE_2D:
					glTexImage2D(target, 0, internalformat, desc->width, desc->height, 0, format, type, desc->pixel);
					checkError();
					break;
				case GL_TEXTURE_2D_ARRAY:
					//throw NotImplementedException("");
					//glTexImage3D
					break;
				case GL_TEXTURE_2D_MULTISAMPLE:
					glTexImage2DMultisample(target, desc->nrSamples, internalformat, desc->width, desc->height,
					                        GL_FALSE);
					checkError();
					break;
				case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
					//throw NotImplementedException("");
	//				glTexImage3DMultisample
					break;
				case GL_TEXTURE_CUBE_MAP:
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, internalformat, desc->width, desc->height, 0,
					             format,
					             type, desc->cubepixel[0]);
					checkError();
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, internalformat, desc->width, desc->height, 0,
					             format,
					             type, desc->cubepixel[1]);
					checkError();
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, internalformat, desc->width, desc->height, 0,
					             format,
					             type, desc->cubepixel[2]);
					checkError();
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, internalformat, desc->width, desc->height, 0,
					             format,
					             type, desc->cubepixel[3]);
					checkError();
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, internalformat, desc->width, desc->height, 0,
					             format,
					             type, desc->cubepixel[4]);
					checkError();
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, internalformat, desc->width, desc->height, 0,
					             format,
					             type, desc->cubepixel[5]);
					checkError();
					break;
				case GL_TEXTURE_1D:
				default:
					assert(0);
			}

			/*	Determine if to generate mipmaps.   */
			if (desc->numlevel > 0 && desc->usemipmaps != 0 && target != GL_TEXTURE_CUBE_MAP && target != GL_TEXTURE_CUBE_MAP_ARRAY) {
				// Generate the mip maps.
				glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, desc->numlevel);
				checkError();
				glGenerateMipmap(target);
				checkError();
				//glGenerateTextureMipmap
			}

		} else {
			switch (target) {
				case GL_TEXTURE_3D:
					break;
				case GL_TEXTURE_2D:
					glTexStorage2D(target, 1, internalformat, desc->width,
					               desc->height); // TODO add only if mipmaps is used with NULL pixel object.
					break;
				case GL_TEXTURE_2D_ARRAY:
					break;
				case GL_TEXTURE_2D_MULTISAMPLE:
					//glTexImage2DMultisample(target, desc->nrSamples, internalformat, desc->width, desc->height, GL_FALSE);
					break;
				case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
					break;
				case GL_TEXTURE_CUBE_MAP:
					break;
				case GL_TEXTURE_1D:
				default:
					assert(0);
			}
		}
	}

	/*  Check for error.    */
	GLenum errorStatus = glGetError();
	if (errorStatus != GL_NO_ERROR){
		glDeleteTextures(1, &texture);
		throw RuntimeException(
				::fvformatf("Error when allocating the texture - %d, %s", errorStatus, gluErrorString(errorStatus)));
	}

	// Unbound texture.
	glBindTexture(target, 0);
	checkError();

	// Add debug marker information.
	addMarkerLabel(glCore, GL_TEXTURE, texture, &desc->marker);

	/*	*/
	Texture *gltex = new Texture();
	//gltex->pdata = new GLTextureObject();
	GLTextureObject *glTextureObject = (GLTextureObject *) gltex->getObject();
	glTextureObject->texture = texture;
	glTextureObject->target = target;
	glTextureObject->desc = *desc;
	glTextureObject->sampler = sampler;
	//gltex->iRenderer = this;

	/*	Create mapper object.	*/
	return gltex;
}

void GLRendererInterface::deleteTexture(Texture *texture) {
	GLTextureObject* textureObject = (GLTextureObject*)texture->getObject();

	if(glIsTexture(textureObject->texture)){
		checkError();
		glDeleteTextures(1, &textureObject->texture);
		checkError();
	}
	else
		throw InvalidArgumentException("Invalid texture object.");

	//delete texture->pdata;
	delete texture;
}

Sampler *GLRendererInterface::createSampler(SamplerDesc *desc) {

	/*  Validate desc.  */
	if (desc == NULL)
		throw InvalidArgumentException("Invalid sampler description pointer object.");
	if(desc->anisotropy < 0)
		throw InvalidArgumentException("Anisotropy can not be negative.");

	/*  Does not support.   */
	if (glGenSamplers == NULL)
		throw RuntimeException("OpenGL sampler object not supported.");

	OpenGLCore *glCore = (OpenGLCore *) this->pdata;
	GLuint sampler;

	/*  */
	glGenSamplers(1, &sampler);

	const GLint wrapS = getWrapMode(desc->AddressU);
	const GLint wrapT = getWrapMode(desc->AddressV);
	const GLint wrapR = getWrapMode(desc->AddressW);

	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, wrapS);
	checkError();
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, wrapT);
	checkError();
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, wrapR);
	checkError();

	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, getFilterMode(desc->minFilter, desc->mipmapFilter));
	checkError();
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, getFilterMode(desc->magFilter, SamplerDesc::eNoFilterMode));
	checkError();

	const GLint compareMode = desc->compareMode == SamplerDesc::eNoCompare ? GL_NONE : GL_COMPARE_REF_TO_TEXTURE;
	glSamplerParameteri(sampler, GL_TEXTURE_COMPARE_MODE, compareMode);
	if (desc->compareMode)
		glSamplerParameteri(sampler, GL_TEXTURE_COMPARE_FUNC, getCompareMode(desc->compareFunc));

	/*  */
	if (desc->anisotropy > 0)
		glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, desc->anisotropy);

	glSamplerParameterfv(sampler, GL_TEXTURE_BORDER_COLOR, &desc->borderColor[0]);

	glSamplerParameteri(sampler, GL_TEXTURE_MIN_LOD, desc->maxLOD);
	glSamplerParameteri(sampler, GL_TEXTURE_MAX_LOD, desc->minLOD);
	glSamplerParameteri(sampler, GL_TEXTURE_LOD_BIAS, desc->biasLOD);
	glSamplerParameteri(sampler, GL_TEXTURE_BASE_LEVEL, 0);

	// Add debug marker information.
	addMarkerLabel(glCore, GL_SAMPLER, sampler, &desc->marker);

	/*  */
	GLSamplerObject *samplerObject = new GLSamplerObject();
	samplerObject->sampler = sampler;
	Sampler *samplerI = new Sampler();
	//samplerI->pdata = samplerObject;
	samplerI->setRenderInterface(this);
	return samplerI;
}

void GLRendererInterface::deleteSampler(Sampler *sampler) {
	GLSamplerObject *samplerObject;
	samplerObject = (GLSamplerObject *) sampler->getObject();
	if(glIsSampler(samplerObject->sampler))
		glDeleteSamplers(1, &samplerObject->sampler);
	else
		throw InvalidArgumentException("Invalid sampler object.");

	//delete sampler->pdata;
	delete sampler;
}

static void checkShaderError(int shader) {

	GLint lstatus;

	/*  */
	glGetShaderiv(shader, GL_COMPILE_STATUS, &lstatus);
	checkError();

	/*	*/
	if (lstatus != GL_TRUE) {
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		checkError();

		char log[maxLength];
		glGetShaderInfoLog(shader, sizeof(log), &maxLength, log);
		checkError();
		throw RuntimeException(fvformatf("Shader compilation error\n%s", log));
	}
}

RenderPipeline *GLRendererInterface::createPipeline(const ProgramPipelineDesc *desc) {
	unsigned int pipeline;
	OpenGLCore *glCore = (OpenGLCore *) this->pdata;
	GLProgramPipeline *pipe = new GLProgramPipeline();
	RenderPipeline *programPipeline = new RenderPipeline();

	glCreateProgramPipelines(1, &pipeline);
	checkError();

	if (desc->v) {
		GLShaderObject *v = (GLShaderObject *)desc->v->getObject();
		glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, v->program);
		pipe->v = desc->v;
	}
	if (desc->f) {
		GLShaderObject *f = (GLShaderObject *)desc->f->getObject();
		glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, f->program);
		pipe->f = desc->f;
	}
	if (desc->g) {
		GLShaderObject *g = (GLShaderObject *)desc->g->getObject();
		glUseProgramStages(pipeline, GL_GEOMETRY_SHADER_BIT, g->program);
		pipe->g = desc->g;
	}
	if (desc->tc) {
		GLShaderObject *tc = (GLShaderObject *)desc->c->getObject();
		glUseProgramStages(pipeline, GL_TESS_CONTROL_SHADER_BIT, tc->program);
		pipe->tc = desc->tc;
	}
	if (desc->te) {
		GLShaderObject *te = (GLShaderObject *)desc->te->getObject();
		glUseProgramStages(pipeline, GL_TESS_EVALUATION_SHADER_BIT, te->program);
		pipe->te = desc->te;
	}
	if (desc->c) {
		GLShaderObject *c = (GLShaderObject *) desc->c->getObject();
		glUseProgramStages(pipeline, GL_COMPUTE_SHADER_BIT, c->program);
		pipe->c = desc->c;
	}

	GLint status;
	glValidateProgramPipeline(pipeline);
	glGetProgramPipelineiv(pipeline, GL_VALIDATE_STATUS, &status);
	if (status != GL_TRUE) {
		GLint logLength;
		glGetProgramPipelineiv(pipeline, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			char log[logLength];
			glGetProgramPipelineInfoLog(pipeline, logLength, 0, log);
			glDeleteProgramPipelines(1, &pipeline);
			throw RuntimeException(fvformatf("Shader pipeline not valid:\n%s\n", log));
		}
	}

	//GL_ARB_pipeline_statistics_query
	// Add debug marker information.
	addMarkerLabel(glCore, GL_PROGRAM_PIPELINE, pipeline, &desc->marker);

	pipe->program = pipeline;
	//programPipeline->pdata = pipe;
	programPipeline->setRenderInterface(this);
	return programPipeline;
}

void GLRendererInterface::deletePipeline(RenderPipeline *obj) {
	GLProgramPipeline *pipeline = (GLProgramPipeline *) obj->getObject();

	if (glIsProgramPipeline(pipeline->program))
		glDeleteProgramPipelines(1, &pipeline->program);
	else
		throw InvalidArgumentException("Object is not a valid ProgramPipeline.");

	//delete obj->pdata;
	delete obj;
}

Shader *GLRendererInterface::createShader(ShaderDesc *desc) {

	/*  Validate the argument.  */
	if(desc == NULL)
		throw InvalidArgumentException("Descriptor object may not be null.");

	OpenGLCore *glCore = (OpenGLCore *) this->pdata;
	ShaderLanguage supportedLanguage = this->getShaderLanguage();
	Shader *shader = new Shader();
	GLShaderObject *shaobj;
	GLint lstatus;

	/*  */
	unsigned int program = 0;
	unsigned int ver = 0;
	unsigned int fra = 0;
	unsigned int geo = 0;
	unsigned int tesse = 0;
	unsigned int tessc = 0;
	unsigned int compute = 0;

	////glCreateShaderProgramv

	program = glCreateProgram();
	checkError();

	if (program <= 0)
		throw RuntimeException("Internal error, could not create shader program.");
	/*	Checking for error.	*/

	//TODO determine if validate shader binary fvformatf.
	// TODO resolve
	if(desc->separatetable)
		glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE);

	if(desc->program.pdata && desc->program.binarySize > 0){
		glProgramBinary(program, desc->program.format, desc->program.pdata, desc->program.binarySize);
		goto finished;
	}

	if (desc->Compute.numcompute > 0 || desc->Compute.size > 0 || desc->Compute.type != ShaderCodeType::eNoShaderType)
	{
		if(desc->Compute.language & supportedLanguage){
			if (desc->Compute.type == ShaderCodeType::eSourceCode)
			{
				compute = glCreateShader(GL_COMPUTE_SHADER);
				glShaderSourceARB(compute, desc->Compute.numcompute, desc->Compute.computeSource, NULL);
				glCompileShaderARB(compute);
				checkShaderError(compute);
			}
			if (desc->Compute.type == ShaderCodeType::eBinary)
			{
				if(desc->Compute.language == ShaderLanguage::SPIRV)
					glShaderBinary(1, &compute, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB,  desc->Compute.computeBinary, desc->Compute.size); // glSpecializeShaderARB
				else
					glShaderBinary(1, &compute, desc->Compute.binaryFormat,  desc->Compute.computeBinary, desc->Compute.size);
			}
			glAttachShader(program, compute);
		} else throw RuntimeException("Invalid Compute shader.");
	}

	if (desc->vertex.numvert > 0 || desc->vertex.size > 0 || desc->vertex.type != ShaderCodeType::eNoShaderType)
	{
		if (desc->vertex.language & supportedLanguage) {
			if (desc->vertex.type == ShaderCodeType::eSourceCode)
			{
				ver = glCreateShader(GL_VERTEX_SHADER_ARB);
				glShaderSourceARB(ver, desc->vertex.numvert, desc->vertex.vertexsource, NULL);
				glCompileShaderARB(ver);
				checkShaderError(ver);
			}
			if (desc->vertex.type == eBinary) {
				if (desc->vertex.language == ShaderLanguage::SPIRV)
					glShaderBinary(1, &ver, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, desc->vertex.vertexBinary,
					               desc->vertex.size);
				else
					glShaderBinary(1, &ver, desc->vertex.binaryFormat, desc->vertex.vertexBinary, desc->vertex.size);
			}
			glAttachShader(program, ver);
		} else throw RuntimeException("Invalid Vertex shader.");
	}

	if(desc->fragment.numfrag > 0 || desc->fragment.size > 0 || desc->fragment.type != eNoShaderType) {
		if (desc->fragment.language & supportedLanguage) {
			if (desc->fragment.type == eSourceCode) {
				fra = glCreateShader(GL_FRAGMENT_SHADER_ARB);
				glShaderSourceARB(fra, desc->fragment.numfrag, desc->fragment.fragmentsource, NULL);
				glCompileShaderARB(fra);
				checkShaderError(fra);
			}
			if (desc->fragment.type == eBinary) {
				if (desc->fragment.language == ShaderLanguage::SPIRV)
					glShaderBinary(1, &fra, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, desc->fragment.fragmentBinary,
					               desc->fragment.size);
				else
					glShaderBinary(1, &fra, desc->fragment.binaryFormat, desc->fragment.fragmentBinary,
					               desc->fragment.size);
			}
			glAttachShader(program, fra);
		} else throw RuntimeException("Invalid fragment shader.");
	}

	if(desc->geometry.numgeo > 0 || desc->geometry.size > 0 || desc->geometry.type != eNoShaderType) {
		if (desc->geometry.language & supportedLanguage) {
			if (desc->geometry.type == ShaderCodeType::eSourceCode) {
				geo = glCreateShader(GL_GEOMETRY_SHADER_ARB);
				glShaderSourceARB(geo, desc->geometry.numgeo, desc->geometry.geometrysource, NULL);
				glCompileShaderARB(geo);
				checkShaderError(geo);
			}
			if (desc->geometry.type == ShaderCodeType::eBinary)
			{
				if (desc->geometry.language == ShaderLanguage::SPIRV)
					glShaderBinary(1, &geo, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, desc->geometry.geometryBinary,
					               desc->geometry.size);
				else
					glShaderBinary(1, &geo, desc->geometry.binaryFormat, desc->geometry.geometryBinary,
					               desc->geometry.size);
			}
			glAttachShader(program, geo);
		} else throw RuntimeException("Invalid Geometry shader.");
	}

//
//	if (desc->geometrysource) {
//		geo = glCreateShader(GL_GEOMETRY_SHADER_ARB);
//		glShaderSourceARB(geo, desc->numgeo, desc->geometrysource, NULL);
//		glCompileShaderARB(geo);
//		checkShaderError(geo);
//		glAttachShader(program, geo);
//	}
//
//	if (desc->tessellationev) {
//		tesse = glCreateShader(GL_TESS_EVALUATION_SHADER);
//		glShaderSourceARB(tesse, desc->numtesev, desc->tessellationev, NULL);
//		glCompileShaderARB(tesse);
//		glAttachShader(program, tesse);
//	}
//
//	if (desc->tessellationco) {
//		tessc = glCreateShader(GL_TESS_CONTROL_SHADER);
//		glShaderSourceARB(tessc, desc->numtesco, desc->tessellationco, NULL);
//		glCompileShaderARB(tessc);
//		glAttachShader(program, tessc);
//
//	}

	glLinkProgramARB(program);
	checkError();

	finished:
	glGetProgramiv(program, GL_LINK_STATUS, &lstatus);
	if (lstatus != GL_TRUE) {
		char log[4096];
		glGetProgramInfoLog(program, sizeof(log), NULL, log);
		fprintf(stderr, "%s.\n", log);
		return NULL;
	}

	/*	Remove shader resources not needed after linking.	*/
	if (glIsShader(ver)) {
		glDetachShader(program, ver);
		glDeleteShader(ver);
	}
	if (glIsShader(fra)) {
		glDetachShader(program, fra);
		glDeleteShader(fra);
	}
	if (glIsShader(geo)) {
		glDetachShader(program, geo);
		glDeleteShader(geo);
	}
	if (glIsShader(tesse)) {
		glDetachShader(program, tesse);
		glDeleteShader(tesse);
	}
	if (glIsShader(tessc)) {
		glDetachShader(program, tessc);
		glDeleteShader(tessc);
	}
	if (glIsShader(compute)) {
		glDetachShader(program, compute);
		glDeleteShader(compute);
	}

	// Add debug marker information.
	addMarkerLabel(glCore, GL_PROGRAM, program, &desc->marker);

	/*	*/
	shaobj = new GLShaderObject();
	shaobj->program = program;
	//shader->pdata = shaobj;
	shader->setRenderInterface(this);

	return shader;
}

void GLRendererInterface::deleteShader(Shader *shader) {
	GLShaderObject *glShaderObject = (GLShaderObject *) shader->getObject();

	if(glIsProgramARB(glShaderObject->program))
		glDeleteProgram(glShaderObject->program);
	else
		throw InvalidArgumentException("Not a valid shader object.");

//	delete shader->pdata;
	delete shader;
}

Buffer *GLRendererInterface::createBuffer(BufferDesc *desc) {

	OpenGLCore *glCore = (OpenGLCore *) this->pdata;

	/*	Verify the arguments.	*/
	if (desc == NULL)
		throw InvalidArgumentException("Invalid buffer description pointer object.");
	if(desc->size < 0)
		throw InvalidArgumentException("Buffer size must be 0 or greater.");

	Buffer *buffer = NULL;
	GLBufferObject *glbuf = NULL;
	GLenum error;

	GLuint buf = 0;
	/*	Translate into OpenGL arguments.	*/
	GLenum target = getBufferType((BufferDesc::BufferType) desc->type);
	GLenum usage = getBufferHint((BufferDesc::BufferHint) desc->hint);

	//TODO add for checking if target is supported.

	//TODO improve the extension usage.
	/*	Create buffer object.	*/
	if (glGenBuffersARB) {

		glGenBuffersARB(1, &buf);
		checkError();
		glBindBufferARB(target, buf);
		checkError();

		/*	Allocate buffer.	*/
		glBufferDataARB(target, desc->size, desc->data, usage);
		checkError();

		/*  Check for errors.   */
		error = glGetError();
		if (error != GL_NO_ERROR) {
			glDeleteBuffers(1, &buf);
			checkError();
			throw RuntimeException(fvformatf("Failed to create buffer - %s", glewGetErrorString(error)));
		}

		glBindBufferARB(target, 0);
		checkError();
	} else {
		glGenBuffers(1, &buf);
		checkError();
		glBindBuffer(target, buf);
		checkError();

		/*	Allocate buffer.	*/
		glBufferData(target, desc->size, desc->data, usage);
		checkError();

		/*  Check for errors.   */
		error = glGetError();
		if (error != GL_NO_ERROR) {
			glDeleteBuffers(1, &buf);
			throw RuntimeException(fvformatf("Failed to create buffer - %s", glewGetErrorString(error)));
		}

		glBindBuffer(target, 0);
		checkError();
	}

	// Add debug marker information.
	addMarkerLabel(glCore, GL_BUFFER, buf, &desc->marker);

	/*	Assign. */
	buffer = new Buffer();
	glbuf = new GLBufferObject();
	glbuf->buffer = buf;
	glbuf->desc = *desc;
	glbuf->target = target;
	//buffer->pdata = glbuf;
	buffer->setRenderInterface(this);
	return buffer;
}

void GLRendererInterface::deleteBuffer(Buffer *object) {

	GLBufferObject *glbuf = (GLBufferObject *)object->getObject();

	if (glIsBufferARB(glbuf->buffer)) {
		glDeleteBuffersARB(1, &glbuf->buffer);
	} else {
		throw std::invalid_argument("Buffer is not a valid buffer object.");
	}

	//delete object->pdata;
	delete object;
}

Geometry *GLRendererInterface::createGeometry(GeometryDesc *desc) {

	/*  Validate the argument.  */
	if(desc->primitive & ~(GeometryDesc::ePoint | GeometryDesc::eLines | GeometryDesc::eTriangles | GeometryDesc::eTriangleStrips))
		throw std::invalid_argument("Invalid primitive");
	//GL_TRIANGLES_ADJACENCY
	//GL_PATCHES

	Geometry *geometryObject;
	OpenGLCore *glCore = (OpenGLCore *) this->pdata;
	GLGeometryObject *glgeoobj = NULL;
	unsigned int vao;
	unsigned int x;

	/*	*/
	geometryObject = new Geometry();
	glgeoobj = new GLGeometryObject();

	/*  */
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/*	Requires array buffer.  */
	if (desc->numVerticecs > 0) {
		BufferDesc abuffer = {};
		abuffer.size = desc->numVerticecs * desc->vertexStride;
		abuffer.type = BufferDesc::eArray;
		abuffer.data = desc->buffer;
		abuffer.hint = (BufferDesc::BufferHint)(BufferDesc::eWrite | BufferDesc::eStatic);
		glgeoobj->vertexbuffer = createBuffer(&abuffer);
//		if(glVertexArrayVertexBuffer){
//            //ARB_separate_attrib_format
//		    GLBufferObject* vertexBuffer = (GLBufferObject*)glgeoobj->vertexbuffer->getObject();
//		    glVertexArrayVertexBuffer(0, vertexBuffer->buffer, 0, )
//		}else{
		glgeoobj->vertexbuffer->bind();
		//}
	}

	/*	Requires element buffer.    */
	if (desc->numIndices > 0) {
		BufferDesc abuffer = {};
		abuffer.size = desc->numIndices * desc->indicesStride;
		abuffer.type = BufferDesc::eElementArray;
		abuffer.data = desc->indices;
		abuffer.hint = (BufferDesc::BufferHint)(BufferDesc::eWrite | BufferDesc::eStatic);
		glgeoobj->indicesbuffer = createBuffer(&abuffer);
		glgeoobj->indicesbuffer->bind();

		/*  Determine indices data size type.   */
		switch (desc->indicesStride) {
			case 1:
				glgeoobj->indicesType = GL_UNSIGNED_BYTE;
				break;
			case 2:
				glgeoobj->indicesType = GL_UNSIGNED_SHORT;
				break;
			case 4:
				glgeoobj->indicesType = GL_UNSIGNED_INT;
				break;
		}
		//VertexArrayElementBuffer
	}

	/*	*/
	for (x = 0; x < desc->numVertexAttributes; x++) {
		if(glEnableVertexArrayAttrib){
			glEnableVertexArrayAttrib(vao, desc->vertexattribute[x].index);
			glVertexAttribPointer(desc->vertexattribute[x].index,
			                      desc->vertexattribute[x].size,
			                      getAttributeDataType((GeometryDesc::AttributeType) desc->vertexattribute[x].datatype),
			                      GL_FALSE,
			                      desc->vertexStride,
			                      (const void *) desc->vertexattribute[x].offset);
		}else{
			glEnableVertexAttribArray(desc->vertexattribute[x].index);
			glVertexAttribPointer(desc->vertexattribute[x].index,
			                      desc->vertexattribute[x].size,
			                      getAttributeDataType((GeometryDesc::AttributeType) desc->vertexattribute[x].datatype),
			                      GL_FALSE,
			                      desc->vertexStride,
			                      (const void *) desc->vertexattribute[x].offset);
		}
	}


	glBindVertexArray(0);

	// Add debug marker information.
	addMarkerLabel(glCore, GL_VERTEX_ARRAY, vao, &desc->marker);

	glgeoobj->mode = getPrimitive((GeometryDesc::Primitive) desc->primitive);
	glgeoobj->vao = vao;
	glgeoobj->desc = *desc;

	//geometryObject->pdata = glgeoobj;
	return geometryObject;
}

void GLRendererInterface::deleteGeometry(Geometry *obj) {


}

FrameBuffer *GLRendererInterface::createFrameBuffer(FrameBufferDesc *desc) {

	unsigned int i;                             /*	*/
	GLFrameBufferObject *glfraobj = NULL;       /*	*/
	GLenum frstat;                              /*	*/
	GLenum draw[16];                            /*	*/
	GLuint numatt = 0;                          /*	*/
	OpenGLCore *glCore = (OpenGLCore *) this->pdata;

	/*  Validate the arguments. */
	if (desc == NULL)
		throw std::invalid_argument("Descriptor object must not be null");
	if (desc->depth && desc->stencil && desc->depthstencil)
		throw std::invalid_argument("");


	glfraobj = new GLFrameBufferObject();
	assert(glfraobj);

	memcpy(&glfraobj->desc, desc, sizeof(FrameBufferDesc));

	/*  */
	glGenFramebuffers(1, &glfraobj->framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, glfraobj->framebuffer);
	//TODO add support for named framebuffer.
	/*  */
	if (desc->depth) {
		GLTextureObject *tex = (GLTextureObject *) desc->depth->getObject();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex->target, tex->texture, 0);
	}

	/*  */
	if (desc->stencil) {
		GLTextureObject *tex = (GLTextureObject *) desc->stencil->getObject();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, tex->target, tex->texture, 0);
	}

	/*  */
	if (desc->depthstencil) {
		GLTextureObject *tex = (GLTextureObject *) desc->depthstencil->getObject();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, tex->target, tex->texture, 0);
	}

	/*  */
	for (i = 0; i < sizeof(desc->attach) / sizeof(desc->attach[0]); i++) {

		/*  */
		if (desc->attach[i] != NULL) {
			GLTextureObject *tex = (GLTextureObject *) desc->attach[i]->getObject();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, tex->target, tex->texture, 0);
			/*	*/
			draw[numatt] = GL_COLOR_ATTACHMENT0 + i;
			numatt++;

			//TODO improve
			if(glNamedFramebufferParameteri){
				glNamedFramebufferParameteri(glfraobj->framebuffer, GL_FRAMEBUFFER_DEFAULT_WIDTH, tex->desc.width);
				glNamedFramebufferParameteri(glfraobj->framebuffer, GL_FRAMEBUFFER_DEFAULT_HEIGHT, tex->desc.height);
				glNamedFramebufferParameteri(glfraobj->framebuffer, GL_FRAMEBUFFER_DEFAULT_LAYERS, tex->desc.depth);
			}
		}
	}

	glDrawBuffers(numatt, &draw[0]);

	/*  Validate if created properly.*/
	frstat = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (frstat != GL_FRAMEBUFFER_COMPLETE) {
		/*  Delete  */
		glDeleteFramebuffers(1, &glfraobj->framebuffer);
		delete glfraobj;
		throw RuntimeException(fvformatf("Failed to create framebuffer, 0x%x.\n", frstat));
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Add debug marker information.
	addMarkerLabel(glCore, GL_FRAMEBUFFER, glfraobj->framebuffer, &desc->marker);

	/*  */
	glfraobj->desc = *desc;
	FrameBuffer *frameBuffer = new FrameBuffer();
	//frameBuffer->pdata = glfraobj;
	frameBuffer->setRenderInterface(this);
}

void GLRendererInterface::deleteFrameBuffer(FrameBuffer *obj) {
	OpenGLCore *glCore = (OpenGLCore *) this->pdata;
	GLFrameBufferObject *glfraobj = (GLFrameBufferObject*)obj->getObject();

	if(glIsFramebuffer(glfraobj->framebuffer))
		glDeleteFramebuffers(1, &glfraobj->framebuffer);
	else if(glfraobj->framebuffer != 0) // Check for default framebuffer object.
		throw std::invalid_argument("None valid framebuffer object.");

	/*  Delete memory.  */
	delete obj->getObject();
	delete obj;
}

QueryObject* GLRendererInterface::createQuery(QueryDesc* desc){
	OpenGLCore *glCore = (OpenGLCore *) this->pdata;
	QueryObject* queryObject;
	GLQuery* glQuery;
	GLuint query[8];

	//TODO determine if index is supported or not
	glGenQueries(8, &query[0]);
	addMarkerLabel(glCore, GL_QUERY, query[0], &desc->marker);

	/*  */
	queryObject = new QueryObject();
	/*  Create internal object. */

	glQuery = new GLQuery();
	memcpy(glQuery->query, query, sizeof(query));
//    glQuery->query = query;

	//queryObject->pdata = glQuery;
	queryObject->setRenderInterface(this);

	return queryObject;
}
void GLRendererInterface::deleteQuery(QueryObject* query){
	//TODO add.
	throw NotImplementedException();
}

RendererWindow* GLRendererInterface::createWindow(int x, int y, int width, int height) {
	OpenGLCore *glcore = (OpenGLCore *) this->pdata;

	WindowManager::getInstance();
	Ref<GLRendererInterface> rendRef = Ref<GLRendererInterface>(this);
	GLRenderWindow* renderWindow = new GLRenderWindow(rendRef);

		/*	Cleanup.	*/
	if (glcore->tpmwindow) {
		SDL_DestroyWindow(glcore->tpmwindow);
		glcore->tpmwindow = NULL;
	}

	return renderWindow;

	// rendererWindow->createWindow(x, y, width, height, "opengl");
	// rendererWindow->renderer = this;
	// rendererWindow->renderer->increment();



	/*  */
	createSwapChain();
	return NULL;
	//return rendererWindow;
}

void GLRendererInterface::setCurrentWindow(RendererWindow* window) {
	OpenGLCore *glcore = (OpenGLCore *) this->pdata;
	//window->useWindow(glcore->openglcontext);
	glcore->drawwindow = window;

//	glcore->drawwindow = (SDL_Window *) window;
//	if (SDL_GL_MakeCurrent(glcore->drawwindow, glcore->openglcontext) != 0) {
//		throw RuntimeException(fvformatf(""));
//	}
}

void GLRendererInterface::createSwapChain() {

}

FrameBuffer *GLRendererInterface::getDefaultFramebuffer(void *window) {
	OpenGLCore *glcore = (OpenGLCore *) this->pdata;

	static FrameBuffer *defaultFrambuffer = NULL;
	if (defaultFrambuffer == NULL) {

		defaultFrambuffer = new FrameBuffer();
		GLFrameBufferObject *frameBufferObject = new GLFrameBufferObject();
		frameBufferObject->framebuffer = 0;
		frameBufferObject->numtextures = 1;
		//defaultFrambuffer->pdata = frameBufferObject;
		defaultFrambuffer->setRenderInterface(this);

		frameBufferObject->desc.attach[0] = new FrameBufferTexture();
		//frameBufferObject->desc.attach[0]->iRenderer = this;
	}

	return defaultFrambuffer;
}

void GLRendererInterface::clear(unsigned int bitflag) {
	GLbitfield mask = 0;
	mask |= bitflag & eColor ? GL_COLOR_BUFFER_BIT : 0;
	mask |= bitflag & eDepth ? GL_DEPTH_BUFFER_BIT : 0;
	mask |= bitflag & eStencil ? GL_STENCIL_BUFFER_BIT : 0;
	glClear(mask);
}


void GLRendererInterface::clearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

ViewPort* GLRendererInterface::getView(unsigned int i){
	OpenGLCore *glcore = (OpenGLCore *) this->pdata;
	/*  Validate the index. */
	if(i >= glcore->capability.sMaxViewPorts)
		throw std::invalid_argument(
				fvformatf("Does not support viewport index %d, max index %d.", i, glcore->capability.sMaxViewPorts));

	// If the view does not exits. Create it.
	if(i == 0)
		return glcore->defaultViewport;
	return glcore->viewports[i - 1];
}


void GLRendererInterface::setVSync(int sync) {
	SDL_GL_SetSwapInterval(sync);
}

void GLRendererInterface::setDepthMask(bool flag) {
	glDepthMask(flag ? GL_TRUE : GL_FALSE);
}

void GLRendererInterface::enableState(GLRendererInterface::State state) {
	glEnable(getState(state));
}

void GLRendererInterface::disableState(GLRendererInterface::State state) {
	glDisable(getState(state));
}

bool GLRendererInterface::isStateEnabled(GLRendererInterface::State state){
	return glIsEnabled(getState(state));
}

void GLRendererInterface::swapBuffer(void) {

	OpenGLCore *glcore = (OpenGLCore *) this->pdata;

	glcore->drawwindow->swapBuffer();
	//SDL_GL_SwapWindow(glcore->drawwindow);
}

void GLRendererInterface::drawInstance(Geometry *geometry, unsigned int num) {

	GLGeometryObject *glgeo;
	OpenGLCore *glCore = (OpenGLCore *) this->pdata;
	assert(geometry && num > 0);

	//glgeo = (GLGeometryObject *) geometry->pdata;

	/*  */
	glBindVertexArray(glgeo->vao);
	resetErrorFlag();
	if (glgeo->desc.numIndices > 0) {
		glDrawElementsInstancedARB(glgeo->mode, glgeo->desc.numIndices, glgeo->indicesType, NULL, num);
	} else {
		glDrawArraysInstancedARB(glgeo->mode, 0, glgeo->desc.numVerticecs, num);
	}

	GLenum errorStatus = glGetError();
	if(errorStatus != GL_NO_ERROR)
		throw RuntimeException(
				::fvformatf("Error when dispatching compute - %d, %s", errorStatus, gluErrorString(errorStatus)));

	glBindVertexArray(0);
}

void GLRendererInterface::drawMultiInstance(Geometry &geometries, const unsigned int *first, const unsigned int *count,
                                  unsigned int num) {
	throw NotImplementedException();
}

void GLRendererInterface::drawMultiIndirect(Geometry &geometries, unsigned int offset, unsigned int indirectCount) {
	throw NotImplementedException();
}

void GLRendererInterface::drawIndirect(Geometry *geometry) {
	GLGeometryObject *glgeo;
	OpenGLCore *glCore = (OpenGLCore *) this->pdata;
	assert(geometry);

	//glgeo = (GLGeometryObject *) geometry->pdata;

	/*  */
	glBindVertexArray(glgeo->vao);
	resetErrorFlag();
	if (glgeo->desc.numIndices > 0) {
		glDrawElementsIndirect(glgeo->mode, glgeo->indicesType, NULL);
	} else {
		glDrawArraysIndirect(glgeo->mode, NULL);
	}

	GLenum errorStatus = glGetError();
	if(errorStatus != GL_NO_ERROR)
		throw RuntimeException(
				::fvformatf("Error when dispatching compute - %d, %s", errorStatus, gluErrorString(errorStatus)));

	glBindVertexArray(0);
}

void GLRendererInterface::setLineWidth(float width) {
	glLineWidth(width);
}

void GLRendererInterface::blit(const FrameBuffer *source, FrameBuffer *dest, Texture::FilterMode filterMode) {
	GLFrameBufferObject *read = (GLFrameBufferObject *) source->getObject();
	GLFrameBufferObject *write = (GLFrameBufferObject *) dest->getObject();

	GLenum filter = getTextureFilterModeNoMip(filterMode);
	GLenum attachment = GL_COLOR_BUFFER_BIT;

	if (glBlitNamedFramebuffer) {

		glBlitNamedFramebuffer(read->framebuffer, write->framebuffer, 0, 0, source->width(), source->height(), 0, 0,
		                       dest->width(), dest->height(), attachment, filter);
	} else {
		source->read();
		dest->write();
		glBlitFramebuffer(0, 0, source->width(), source->height(), 0, 0, dest->width(), dest->height(),
		                  attachment, filter);
	}
}

void GLRendererInterface::bindTextures(unsigned int firstUnit, const std::vector<Texture*>& textures){
	const int nTextures = textures.size();

	if (glBindTextures) {

		GLuint texture_list[nTextures];
		for (int i = 0; i < nTextures; i++) {
			Texture *texture = textures[i];
			if (texture) {
				const GLTextureObject *glTextureObject = (const GLTextureObject *) texture->getObject();
				texture_list[i] = glTextureObject->texture;
			} else
				texture_list[i] = 0;
		}

		glBindTextures(firstUnit, nTextures, &texture_list[0]);
	} else if (glBindTextureUnit) {
		for (int i = 0; i < nTextures; i++) {
			Texture *texture = textures[i];
			if (texture) {
				const GLTextureObject *glTextureObject = (const GLTextureObject *) texture->getObject();
				glBindTextureUnit(firstUnit + i, glTextureObject->texture);
			} else
				glBindTextureUnit(firstUnit + i, 0);
		}
	} else {
		for (int i = 0; i < nTextures; i++) {
			Texture *texture = textures[i];
			if (texture) {
				const GLTextureObject *glTextureObject = (const GLTextureObject *) texture->getObject();
				glActiveTextureARB(GL_TEXTURE0_ARB + firstUnit + i);
				glBindTexture(glTextureObject->target, glTextureObject->texture);
			}
		}
	}
}

void GLRendererInterface::bindImages(unsigned int firstUnit, const std::vector<Texture *> &textures,
                           const std::vector<Texture::MapTarget> &mapping,
                           const std::vector<Texture::Format> &formats) {
	const int nTextures = textures.size();

	if (glBindImageTextures) {
		GLuint texture_list[nTextures];
		for (int i = 0; i < nTextures; i++) {
			Texture *texture = textures[i];
			if (texture) {
				const GLTextureObject *glTextureObject = (const GLTextureObject *) texture->getObject();
				texture_list[i] = glTextureObject->texture;
			} else
				texture_list[i] = 0;
		}
		glBindImageTextures(firstUnit, nTextures, texture_list);
	} else if (glBindImageTexture) {
		GLuint texture_list[nTextures];
		for (int i = 0; i < nTextures; i++) {
			Texture *texture = textures[i];
			if(texture){
//				GLenum gformat = getImageInternalFormat(fvformatf);
//				glBindImageTexture(index, texobj->texture, level, GL_FALSE, 0, access, gformat);
//				GLenum access = getAccess(target);
			}
		}
//
//		//TODO relocate to seperate function
//		GLenum access = getAccess(target);
//		GLenum gformat = getImageInternalFormat(fvformatf);
//
//		glBindImageTexture(index, texobj->texture, level, GL_FALSE, 0, access, gformat);
		throw NotImplementedException();
	} else
		throw RuntimeException("glBindImageTexture not supported,");
}

void GLRendererInterface::copyTexture(const Texture *source, Texture *target) {
	const GLTextureObject *so = (const GLTextureObject *) source->getObject();
	GLTextureObject *ta = (GLTextureObject *) target->getObject();

	if (glCopyImageSubData) {
		glCopyImageSubData(so->texture, so->target, 0, 0, 0, 0,
		                   ta->texture, ta->target, 0, 0, 0, 0,
		                   target->width(), target->height(), 1);   //TODO add depth and region.
	} else {
		throw NotImplementedException();
		("copyTexture not supported");
	}
}

Sync* GLRendererInterface::createSync(SyncDesc* desc){
	Sync* sync = new Sync();
	//sync->iRenderer = this;
	GLSync* glSync = new GLSync();
	//sync->pdata = glSync;
	return sync;
}

void GLRendererInterface::deleteSync(Sync* sync){
	delete sync->getObject();
	delete sync;
}

void GLRendererInterface::dispatchCompute(unsigned int *global, unsigned int *local, unsigned int offset) {

	/*  */
	resetErrorFlag();
	if (global) {
		if (local && glDispatchComputeGroupSizeARB)
			glDispatchComputeGroupSizeARB(global[0], global[1], global[2], local[0], local[1], local[2]);
		else
			glDispatchCompute(global[0], global[1], global[2]);
	} else {
		// Presume indirect.
		glDispatchComputeIndirect(offset);
	}

	GLenum errorStatus = glGetError();
	if(errorStatus != GL_NO_ERROR)
		throw RuntimeException(
				::fvformatf("Error when dispatching compute - %d, %s", errorStatus, gluErrorString(errorStatus)));

	//TODO relocate
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	glMemoryBarrier(GL_TEXTURE_UPDATE_BARRIER_BIT);
}

void GLRendererInterface::memoryBarrier(void){

}

void callback_debug_gl(GLenum source, GLenum type, GLuint id, GLenum severity,
                       GLsizei length, const GLchar *message, GLvoid *userParam) {

	const char *sourceString;
	const char *typeString;
	const char *severityString;

	switch (source) {
		case GL_DEBUG_CATEGORY_API_ERROR_AMD:
		case GL_DEBUG_SOURCE_API: {
			sourceString = "API";
			break;
		}
		case GL_DEBUG_CATEGORY_APPLICATION_AMD:
		case GL_DEBUG_SOURCE_APPLICATION: {
			sourceString = "Application";
			break;
		}
		case GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD:
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: {
			sourceString = "Window System";
			break;
		}
		case GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD:
		case GL_DEBUG_SOURCE_SHADER_COMPILER: {
			sourceString = "Shader Compiler";
			break;
		}
		case GL_DEBUG_SOURCE_THIRD_PARTY: {
			sourceString = "Third Party";
			break;
		}
		case GL_DEBUG_CATEGORY_OTHER_AMD:
		case GL_DEBUG_SOURCE_OTHER: {
			sourceString = "Other";
			break;
		}
		default: {
			sourceString = "Unknown";
			break;
		}
	}/**/
	printf(sourceString);

	switch (type) {
		case GL_DEBUG_TYPE_ERROR: {
			typeString = "Error";
			break;
		}
		case GL_DEBUG_CATEGORY_DEPRECATION_AMD:
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {
			typeString = "Deprecated Behavior";
			break;
		}
		case GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD:
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: {
			typeString = "Undefined Behavior";
			break;
		}
		case GL_DEBUG_TYPE_PORTABILITY_ARB: {
			typeString = "Portability";
			break;
		}
		case GL_DEBUG_CATEGORY_PERFORMANCE_AMD:
		case GL_DEBUG_TYPE_PERFORMANCE: {
			typeString = "Performance";
			break;
		}
		case GL_DEBUG_CATEGORY_OTHER_AMD:
		case GL_DEBUG_TYPE_OTHER: {
			typeString = "Other";
			break;
		}
		default: {
			typeString = "Unknown";
			break;
		}
	}/**/
	printf(typeString);

	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH: {
			severityString = "High";
			break;
		}
		case GL_DEBUG_SEVERITY_MEDIUM: {
			severityString = "Medium";
			break;
		}
		case GL_DEBUG_SEVERITY_LOW: {
			severityString = "Low";
			break;
		}
		default: {
			severityString = "Unknown";
			break;
		}
	}/**/
	printf(severityString);

	printf(message);
	printf("\n");
}

void GLRendererInterface::setDebug(bool enable) {

	/*	TODO CLEAN.	*/
	typedef void (*glDebugMessageCallback)(GLenum source, GLenum type, GLuint id, GLenum severity,
	                                       GLsizei length, const GLchar *message, GLvoid *userParam);
	typedef void (*PROC)(glDebugMessageCallback, void *);

	if (enable) {
		PROC callback;
		PROC callbackARB;
		PROC callbackAMD;

		/*	Load function pointer by their symbol name.	*/
		callback = (PROC) SDL_GL_GetProcAddress("glDebugMessageCallback");
		callbackARB = (PROC) SDL_GL_GetProcAddress("glDebugMessageCallbackARB");
		callbackAMD = (PROC) SDL_GL_GetProcAddress("glDebugMessageCallbackAMD");



		/*	Set Debug message callback.	*/
		if (callback) {
			callback(callback_debug_gl, NULL);
		}
		if (callbackARB) {
			callbackARB(callback_debug_gl, NULL);
		}
		if (callbackAMD) {
			callbackAMD(callback_debug_gl, NULL);
		}

		/*	*/
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}
}

//GL_NV_gpu_program4: SM 4.0 or better.
//GL_NV_vertex_program3: SM 3.0 or better.
//GL_ARB_fragment_program: SM 2.0 or better
const char *GLRendererInterface::getShaderVersion(ShaderLanguage language) const {
	const char *strcore;
	OpenGLCore *glcore = (OpenGLCore *) this->getData();
	bool profile;

	if(language == GLSL){
		/*  Lookup table.   */
		int glVersion = glcore->majorVersion + glcore->minorVersion;
		switch(glVersion){
			case 200:return "110";
			case 210:return "120";
			case 300:return "130";
			case 310:return "140";
			case 320:return "150";
			case 330:return "330";
			case 400:return "400";
			case 410:return "410";
			case 420:return "420";
			case 430:return "430";
			case 440:return "440";
			case 450:return "450";
			case 460:return "460";
			// Add the rest of the mapping as each new version is released.
			default:
				throw RuntimeException(fvformatf("OpenGL version %d is not supported", glVersion));
		}
		//TODO add core and compatibility on the name.
		profile = glcore->useCoreProfile;
		strcore = profile ? "core" : "";
	}
	if (language == SPIRV) {
		return "100";
	}
	throw std::invalid_argument("Invalid shader language);");
}

ShaderLanguage GLRendererInterface::getShaderLanguage(void) const {
	OpenGLCore *glCore = (OpenGLCore *) this->pdata;
	return glCore->supportedLanguages;

}

const char *GLRendererInterface::getAPIVersion(void) const {
	return (const char *) glGetString(GL_VERSION);
}

const char *GLRendererInterface::getVersion(void) const {
	return FV_STR_VERSION(1, 0, 0);
}

void GLRendererInterface::getSupportedTextureCompression(TextureDesc::Compression* pCompressions){
	if (pCompressions == NULL)
		throw std::invalid_argument("pCompressions may not be a null pointer.");

	unsigned int compressions = 0;

	//TODO add and improve.
	if (glewIsExtensionSupported("GL_ARB_texture_compression_bptc"))
		compressions |= TextureDesc::Compression::eBPTC;
	if (glewIsExtensionSupported("GL_ANGLE_texture_compression_dxt1"))
		compressions |= TextureDesc::Compression::eDXT1;
	if (glewIsExtensionSupported("GL_EXT_texture_compression_rgtc") ||
	    glewIsExtensionSupported("GL_ARB_texture_compression_rgtc"))
		compressions |= TextureDesc::Compression::eRGTC;
	if (glewIsExtensionSupported("GL_ANGLE_texture_compression_dxt5"))
		compressions |= TextureDesc::Compression::eDXT5;
	if (glewIsExtensionSupported("GL_ATI_texture_compression_3dc"))
		compressions |= TextureDesc::Compression::e3DC;
	if (glewIsExtensionSupported("GL_EXT_texture_compression_s3tc"))
		compressions |= TextureDesc::Compression::eS3TC;
	if (glewIsExtensionSupported("GL_ANGLE_texture_compression_dxt3"))
		compressions |= TextureDesc::Compression::eDXT3;
	if (glewIsExtensionSupported("GL_KHR_texture_compression_astc_ldr"))
		compressions |= TextureDesc::Compression::eASTC_LDR;

	// Add support for default compression fvformatf.
	if(compressions != 0)
		compressions |= TextureDesc::eCompression;

	*pCompressions = (TextureDesc::Compression) compressions;
}

void GLRendererInterface::getCapability(Capability *capability) {

	assert(capability);

	if (capability == NULL)
		throw std::invalid_argument("capability object may not a null pointer.");

	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &capability->sMaxVertexAttributes);
	if (glGetInteger64v) {
		glGetInteger64v(GL_MAX_ELEMENTS_VERTICES, &capability->sMaxElementVertices);
		glGetInteger64v(GL_MAX_ELEMENTS_INDICES, &capability->sMaxElementIndices);
	} else {
		capability->sMaxElementVertices = 0;
		capability->sMaxElementIndices = 0;
		glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, (GLint *) &capability->sMaxElementVertices);
		glGetIntegerv(GL_MAX_ELEMENTS_INDICES, (GLint *) &capability->sMaxElementIndices);
	}
	//	/*	*/
//	GLIF_CONSTANTI_PRINT(GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET);
//	GLIF_CONSTANTI_PRINT(GL_MAX_VERTEX_ATTRIB_BINDINGS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_ELEMENT_INDEX);
//GL_MAX_ELEMENTS_INDICES
//GL_MAX_ELEMENTS_VERTICES

	/*	TODO resolve*/
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,
	              &capability->sMaxTextureUnitActive);            /*	GL_MAX_TEXTURE_IMAGE_UNITS	*/
	glGetIntegerv(GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS, &capability->numMaxSubRoutines);                /*	*/

	//glGetIntegerv(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, &capability->sMaxCombinedShaderStorageBlocks);

	/*  Compute shader. */
	//glGetIntegerv(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS, &capability->sMaxComputeShaderStorageBlocks);
	//glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_BLOCKS, &capability->sMaxComputeUniformBlocks);
	glGetIntegerv(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS, &capability->sMaxComputeTextureImageUnits);
	glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &capability->sMaxComputeSharedMemory);
	//glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_COMPONENTS, &capability->sMaxComputeUniformComponents);
//	glGetIntegerv(GL_MAX_COMPUTE_ATOMIC_COUNTERS, &capability->sMaxDrawBuffers);
//	glGetIntegerv(GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS, &capability->sMaxDrawBuffers);
//	glGetIntegerv(GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS, &capability->sMaxDrawBuffers);
	capability->sWorkGroupDimensions = 3;
	//glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &capability->sWorkGroupDimensions);
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_COUNT, capability->sMaxWorKGroupSize);
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_SIZE, capability->sMaxLocalWorkGroupSize);
	//glGetIntegerv(MAX_COMPUTE_VARIABLE_GROUP_INVOCATIONS_ARB, capability->sMaxLocalWorkGroupSize);
	//glGetIntegerv(MAX_COMPUTE_VARIABLE_GROUP_SIZE_ARB, capability->sMaxLocalWorkGroupSize);
	//glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_BLOCKS, &capability->sMaxDrawBuffers);
	//glGetIntegerv(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS, &capability->sMaxDrawBuffers);


	//glGetIntegerv(GL_MAX_DEBUG_GROUP_STACK_DEPTH, &capability->sMaxDrawBuffers);
	//glGetIntegerv(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, &capability->sMaxDrawBuffers);

//	GLIF_CONSTANTI_PRINT(GL_MAX_ARRAY_TEXTURE_LAYERS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_CLIP_DISTANCES);
//	GLIF_CONSTANTI_PRINT(GL_MAX_COLOR_TEXTURE_SAMPLES);
//	GLIF_CONSTANTI_PRINT(GL_MAX_COMBINED_ATOMIC_COUNTERS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_COMBINED_UNIFORM_BLOCKS);

//	GLIF_CONSTANTI_PRINT(GL_MAX_DEPTH_TEXTURE_SAMPLES);


//	GLIF_CONSTANTI_PRINT(GL_MAX_FRAGMENT_ATOMIC_COUNTERS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_FRAGMENT_INPUT_COMPONENTS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_FRAGMENT_UNIFORM_VECTORS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_FRAGMENT_UNIFORM_BLOCKS);

	glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &capability->sMaxFrameBufferWidth);
	glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &capability->sMaxFrameBufferHeight);
	glGetIntegerv(GL_MAX_FRAMEBUFFER_LAYERS, &capability->sMaxFrameBufferLayers);
	glGetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, &capability->sMaxFrameBufferSamples);

	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &capability->sMaxDrawBuffers);
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &capability->sMaxColorAttachments);
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &capability->sMaxFrameBufferAttachment);        /*	*/
	//	GLIF_CONSTANTI_PRINT(GL_MAX_DUAL_SOURCE_DRAW_BUFFERS);

//	GLIF_CONSTANTI_PRINT(GL_MAX_GEOMETRY_ATOMIC_COUNTERS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_GEOMETRY_INPUT_COMPONENTS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_GEOMETRY_OUTPUT_COMPONENTS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_GEOMETRY_UNIFORM_BLOCKS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS);

//	GLIF_CONSTANTI_PRINT(GL_MAX_SAMPLE_MASK_WORDS);

//	GLIF_CONSTANTI_PRINT(GL_MIN_MAP_BUFFER_ALIGNMENT);
//	GLIF_CONSTANTI_PRINT(GL_MIN_PROGRAM_TEXEL_OFFSET);
//	GLIF_CONSTANTI_PRINT(GL_MAX_RECTANGLE_TEXTURE_SIZE);

//	GLIF_CONSTANTI64_PRINT(GL_MAX_SERVER_WAIT_TIMEOUT);
//
//	/*	*/
//	GLIF_CONSTANTI_PRINT(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS);

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
//	GLIF_CONSTANTI_PRINT(GL_MAX_VARYING_COMPONENTS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_VARYING_VECTORS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_VARYING_FLOATS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_VERTEX_ATOMIC_COUNTERS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_VERTEX_ATTRIBS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_VERTEX_UNIFORM_COMPONENTS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_VERTEX_UNIFORM_VECTORS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_VERTEX_OUTPUT_COMPONENTS);
//	GLIF_CONSTANTI_PRINT(GL_MAX_VERTEX_UNIFORM_BLOCKS);

	glGetIntegerv(GL_MAX_VIEWPORT_DIMS, &capability->sMaxViewPortDims);
	glGetIntegerv(GL_MAX_VIEWPORTS, &capability->sMaxViewPorts);
	glGetIntegerv(GL_VIEWPORT_BOUNDS_RANGE, &capability->sViewPortBoundRange[0]);

	//GL_ALIASED_LINE_WIDTH_RANGE, GL_SMOOTH_LINE_WIDTH_RANGE, and GL_SMOOTH_LINE_WIDTH_GRANULARITY.
	glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, capability->lineWidthRange);
	glGetFloatv(GL_SMOOTH_LINE_WIDTH_GRANULARITY, &capability->lineWidthGranularity);


	glGetIntegerv(GL_MIN_MAP_BUFFER_ALIGNMENT, &capability->sMinMapBufferAlignment);
	//

	/*  */
	capability->sShadow = glewIsExtensionSupported("GL_ARB_texture_float");
	capability->sTransformFeedback = glewIsExtensionSupported("GL_EXT_transform_feedback") || glewIsExtensionSupported("GL_NV_transform_feedback");

	/*  */
	capability->sTextureCompression = capability->sNumCompressedTexture > 0;

	/*  Shaders.    */
	capability->sVertexShader = glewIsExtensionSupported("GL_ARB_vertex_program");
	capability->sFragmentShader = glewIsExtensionSupported("GL_ARB_fragment_shader") || glewIsExtensionSupported("GL_ARB_fragment_program") || glewIsExtensionSupported("GL_ATI_fragment_shader");
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
	capability->sQueryOcclusion = glewIsExtensionSupported("GL_ARB_occlusion_query") || glewIsExtensionSupported("GL_NV_occlusion_query");
	capability->sQueryTime = glewIsExtensionSupported("GL_ARB_timer_query");
	capability->sInstancing =
			glewIsExtensionSupported("GL_ARB_draw_instanced") || glewIsExtensionSupported("GL_EXT_draw_instanced");

	capability->sIndirectDraw = glewIsExtensionSupported("GL_ARB_draw_indirect"); //GL_NV_bindless_multi_draw_indirect
	capability->sIndirectMultiDraw = glewIsExtensionSupported("GL_ARB_multi_draw_indirect") || glewIsExtensionSupported("GL_AMD_multi_draw_indirect");

	//capability->sRenderTarget = glewIsExtensionSupported("GL_ARB_occlusion_query");
//	capability->sShaderImageLoadStorage = VDSystemInfo::supportShaderImageLoadStorage();
	capability->sUniformBuffer = glewIsExtensionSupported("GL_ARB_uniform_buffer_object");
	capability->sShaderStorageBuffer = glewIsExtensionSupported("GL_ARB_shader_storage_buffer_object");

	capability->rayTracing = glewIsExtensionSupported("GL_NV_ray_tracing");

	//GL_ARB_multisample
	capability->sVirtualViewPort = glewIsExtensionSupported("GL_ARB_viewport_array") && (glewIsExtensionSupported("GL_NV_viewport_array2") || glewIsExtensionSupported("GL_ARB_shader_viewport_layer_array"));
	capability->sSRGB = glewIsExtensionSupported("GL_ARB_framebuffer_sRGB") || glewIsExtensionSupported("GL_EXT_framebuffer_sRGB") || glewIsExtensionSupported("GL_EXT_texture_sRGB");
	capability->sConditionalRendering = glewIsExtensionSupported("GL_NV_conditional_render") || glewIsExtensionSupported("GL_NVX_conditional_render");
	//GL_ARB_conditional_render_inverted

	GLint nShaderBinary;
	glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, &nShaderBinary);
	capability->sShaderBinary = nShaderBinary > 0 && glewIsExtensionSupported("GL_ARB_get_program_binary");

	//GL_EXT_texture_sRGB_decode
	//GL_DECODE_EXT;
}

void GLRendererInterface::getStatus(MemoryInfo *memoryInfo) {

	GLint dedicatedVRMem;

	//GL_ATI_meminfo
	glGetIntegerv(GL_VBO_FREE_MEMORY_ATI, &dedicatedVRMem);            //dedicated video memory, total size (in kb) of the GPU memory
	glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI, &dedicatedVRMem);            //dedicated video memory, total size (in kb) of the GPU memory
	glGetIntegerv(GL_RENDERBUFFER_FREE_MEMORY_ATI, &dedicatedVRMem);            //dedicated video memory, total size (in kb) of the GPU memory

	// GL_NVX_gpu_memory_info
	glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &dedicatedVRMem);            //dedicated video memory, total size (in kb) of the GPU memory
	memoryInfo->totalVRam = dedicatedVRMem;
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &dedicatedVRMem);      // total available memory, total size (in Kb) of the memory available for allocations

	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &dedicatedVRMem);    // current available dedicated video memory (in kb),
	memoryInfo->currentVRam = dedicatedVRMem;
	glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTION_COUNT_NVX, &dedicatedVRMem);              //count of total evictions seen by system
	glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &dedicatedVRMem);              // size of total video memory evicted (in kb)

}

void GLRendererInterface::getFeatures(Features *features) {
	OpenGLCore *glCore = (OpenGLCore *)this->pdata;
	memcpy(features, &glCore->features, sizeof(Features));

	features->raytracing = glewIsExtensionSupported("GL_NV_ray_tracing");
	features->variableRateShading = glewIsExtensionSupported("GL_NV_shading_rate_image");
}

CommandList *GLRendererInterface::createCommandBuffer(void) { return new GLCommandList(); }

void GLRendererInterface::submittCommand(Ref<CommandList> &list){
	
}

void GLRendererInterface::execute(CommandList *list) {
	GLCommandList *glist = (GLCommandList *)list;

	for (int i = 0; i < glist->commands.size(); i++) {
		const GLCommandBase *base = glist->commands[i];
		switch (base->getCommand()) {
		case ClearColor:{
			const GLCommandClearColor *clearColor = (const GLCommandClearColor *)base;
			glClearColor(clearColor->clear.x(), clearColor->clear.x(), clearColor->clear.x(), clearColor->clear.x());
			//glClearNamedFramebufferfv(fraobj->framebuffer, GL_COLOR, GL_COLOR_ATTACHMENT0 + index, (GLfloat *)color);
			/* code */
		} break;

		default:
			break;
		}
	}
}

void *GLRendererInterface::getData(void) const {
	return this->pdata;
}

extern "C" IRenderer *createInternalRenderer(IConfig *config) { return new GLRendererInterface(config); }
