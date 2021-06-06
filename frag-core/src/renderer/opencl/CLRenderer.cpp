#include "Exception/RuntimeException.h"
#include "Renderer/IRenderer.h"
#include "Renderer/RenderDesc.h"
#include "Renderer/RendererFactory.h"
#include "Renderer/opencl/internal_object_type.h"
#include "Utils/StringUtil.h"
#include <SDL2/SDL_syswm.h>

using namespace fragcore;

int supportgl(char *extenions);

const char *get_cl_error_str(unsigned int errorcode) {
	static const char *errorString[] = {
		"CL_SUCCESS",
		"CL_DEVICE_NOT_FOUND",
		"CL_DEVICE_NOT_AVAILABLE",
		"CL_COMPILER_NOT_AVAILABLE",
		"CL_MEM_OBJECT_ALLOCATION_FAILURE",
		"CL_OUT_OF_RESOURCES",
		"CL_OUT_OF_HOST_MEMORY",
		"CL_PROFILING_INFO_NOT_AVAILABLE",
		"CL_MEM_COPY_OVERLAP",
		"CL_IMAGE_FORMAT_MISMATCH",
		"CL_IMAGE_FORMAT_NOT_SUPPORTED",
		"CL_BUILD_PROGRAM_FAILURE",
		"CL_MAP_FAILURE",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"CL_INVALID_VALUE",
		"CL_INVALID_DEVICE_TYPE",
		"CL_INVALID_PLATFORM",
		"CL_INVALID_DEVICE",
		"CL_INVALID_CONTEXT",
		"CL_INVALID_QUEUE_PROPERTIES",
		"CL_INVALID_COMMAND_QUEUE",
		"CL_INVALID_HOST_PTR",
		"CL_INVALID_MEM_OBJECT",
		"CL_INVALID_IMAGE_FORMAT_DESCRIPTOR",
		"CL_INVALID_IMAGE_SIZE",
		"CL_INVALID_SAMPLER",
		"CL_INVALID_BINARY",
		"CL_INVALID_BUILD_OPTIONS",
		"CL_INVALID_PROGRAM",
		"CL_INVALID_PROGRAM_EXECUTABLE",
		"CL_INVALID_KERNEL_NAME",
		"CL_INVALID_KERNEL_DEFINITION",
		"CL_INVALID_KERNEL",
		"CL_INVALID_ARG_INDEX",
		"CL_INVALID_ARG_VALUE",
		"CL_INVALID_ARG_SIZE",
		"CL_INVALID_KERNEL_ARGS",
		"CL_INVALID_WORK_DIMENSION",
		"CL_INVALID_WORK_GROUP_SIZE",
		"CL_INVALID_WORK_ITEM_SIZE",
		"CL_INVALID_GLOBAL_OFFSET",
		"CL_INVALID_EVENT_WAIT_LIST",
		"CL_INVALID_EVENT",
		"CL_INVALID_OPERATION",
		"CL_INVALID_GL_OBJECT",
		"CL_INVALID_BUFFER_SIZE",
		"CL_INVALID_MIP_LEVEL",
		"CL_INVALID_GLOBAL_WORK_SIZE",
	};

	/*	compute error index code. 	*/
	const int errorCount = sizeof(errorString) / sizeof(errorString[0]);
	const int index = -errorcode;

	/*	return error string.	*/
	return (index >= 0 && index < errorCount) ? errorString[index] : "Unspecified Error";
}

const char *getCLStringError(unsigned int errorcode) { return get_cl_error_str(errorcode); }

int supportgl(char *extenions) { return strstr(extenions, "cl_khr_gl_sharing") != NULL; }

cl_context createCLContext(unsigned int *ndevices, cl_device_id **devices, SDL_Window *window,
						   SDL_GLContext glcontext) {

	cl_int ciErrNum;
	cl_context context;
	cl_platform_id *platforms;
#ifndef MDHEADLESS
	SDL_SysWMinfo sysinfo;
#endif
	cl_device_id gpuShareId;
	clGetGLContextInfoKHR_fn clGetGLContextInfoKHR;

	/*  Check if argument is non null reference.	*/
	assert(ndevices && devices);

	/*  Get window information.	*/
#ifndef MDHEADLESS
	if (window && glcontext) {
		SDL_VERSION(&sysinfo.version);
		SDL_GetWindowWMInfo(window, &sysinfo);
	}
#endif

	/*  Context properties.	*/
	cl_context_properties props[] = {
		CL_CONTEXT_PLATFORM,
		(cl_context_properties)NULL,
#ifndef MDHEADLESS
		glcontext ? CL_GL_CONTEXT_KHR : 0,
		(cl_context_properties)glcontext,
#ifdef SDL_VIDEO_DRIVER_X11
		glcontext ? CL_GLX_DISPLAY_KHR : 0,
		glcontext ? (cl_context_properties)sysinfo.info.x11.display : NULL,
#endif
#endif
		(cl_context_properties)NULL,
		NULL,
	};

	unsigned int x, y;
	unsigned int nPlatforms;

	/*	Get Number of platform ids.	*/
	ciErrNum = clGetPlatformIDs(0, NULL, &nPlatforms);
	if (ciErrNum != CL_SUCCESS)
		throw RuntimeException(
			fmt::format("failed to get number of OpenCL platforms - %s\n", getCLStringError(ciErrNum)));

	/*  Get platform IDs.	*/
	platforms = (cl_platform_id *)malloc(sizeof(*platforms) * nPlatforms);
	ciErrNum = clGetPlatformIDs(nPlatforms, platforms, NULL);
	if (ciErrNum != CL_SUCCESS)
		throw RuntimeException(fmt::format("failed to get OpenCL platforms - %s\n", getCLStringError(ciErrNum)));

	/*	Iterate through each platform in till the
	 *	platform associated with OpenGL context is found. */
	for (x = 0; x < nPlatforms; x++) {
		size_t pvar = 4;
		char *extenions;

		props[1] = (cl_context_properties)platforms[x];

		/* TODO change to the device extension instead!

		ciErrNum = clGetPlatformInfo(platforms[x], CL_PLATFORM_EXTENSIONS, sizeof(extenions), &extenions, &pvar);
		if(ciErrNum != CL_SUCCESS){
			fprintf(stderr, "clGetPlatformInfo failed: %s\n", getCLStringError(ciErrNum));
			return NULL;
		}
		*/

		/*	Get associated OpenGL context GPU device.	*/
		if (glcontext && supportgl(extenions)) {

			/*	Get extension function from the platform.	*/
			clGetGLContextInfoKHR = (clGetGLContextInfoKHR_fn)clGetExtensionFunctionAddressForPlatform(
				platforms[x], "clGetGLContextInfoKHR");
			if (clGetGLContextInfoKHR == NULL) {
				throw RuntimeException(fmt::format("clGetExtensionFunctionAddressForPlatform failed\n"));
				return NULL;
			}

			pvar = 0;
			ciErrNum = clGetGLContextInfoKHR(props, CL_CURRENT_DEVICE_FOR_GL_CONTEXT_KHR, sizeof(gpuShareId),
											 &gpuShareId, &pvar);
			if (ciErrNum != CL_SUCCESS) {
				throw RuntimeException(
					fmt::format("failed to get OpenGL context info - %s\n", getCLStringError(ciErrNum)));
				return NULL;
			}
		} else {
			/*	get device ids for the GPUS.	*/
			ciErrNum = clGetDeviceIDs(platforms[x], CL_DEVICE_TYPE_GPU, 0, NULL, ndevices);
			if (ciErrNum != CL_SUCCESS) {
				throw RuntimeException(
					fmt::format("failed to get Device ID number - %s\n", getCLStringError(ciErrNum)));
				return NULL;
			}

			*devices = (cl_device_id *)malloc(sizeof(cl_device_id) * *ndevices);
			ciErrNum = clGetDeviceIDs(platforms[x], CL_DEVICE_TYPE_GPU, *ndevices, *devices, (cl_uint *)&pvar);
			if (ciErrNum != CL_SUCCESS) {
				throw RuntimeException(
					fmt::format("failed to get Device ID poiners - %s\n", getCLStringError(ciErrNum)));
				return NULL;
			}

			/*  Check which devices support image. */
			for (y = 0; y < *ndevices; y++) {
				cl_bool imageSupport;
				clGetDeviceInfo(*devices[y], CL_DEVICE_IMAGE_SUPPORT, sizeof(cl_bool), &imageSupport, NULL);
				if (imageSupport == CL_TRUE) {

				} else {
				}
			}

			/*  Done.   */
			break;
		}
	}

	/*	Create context.	*/
	assert(props[1] && props[0]);
	context = clCreateContext(props, *ndevices, *devices, NULL, NULL, &ciErrNum);

	/*  Error check.    */
	if (context == NULL || ciErrNum != CL_SUCCESS) {
		throw RuntimeException(fmt::format("failed to create OpenCL context - %s\n", getCLStringError(ciErrNum)));
		return NULL;
	}

	free(platforms);
	return context;
}

cl_program createProgram(cl_context context, unsigned int nDevices, cl_device_id *device, const char *source) {
	cl_int ciErrNum;
	cl_program program;
	int i;

	assert(context);
	/*  TODO add exception. */
	if (device == NULL || nDevices < 1) {
		throw RuntimeException(fmt::format("Failed to create program CL shader - \n %s", getCLStringError(ciErrNum)));
	}

	/*	*/
	program = clCreateProgramWithSource(context, 1, (const char **)&source, NULL, &ciErrNum);
	if (program == NULL || ciErrNum != CL_SUCCESS) {
		throw RuntimeException(fmt::format("Failed to create program CL shader - \n %s", getCLStringError(ciErrNum)));
	}

	/*	Compile and build CL program.   */
	ciErrNum = clBuildProgram(program, nDevices, device, "", NULL, NULL);
	if (ciErrNum != CL_SUCCESS) {
		if (ciErrNum == CL_BUILD_PROGRAM_FAILURE) {
			char build_log[900];
			size_t build_log_size = sizeof(build_log);
			size_t build_log_ret;
			for (i = 0; i < nDevices; i++) {
				/*	Fetch build log.	*/
				clGetProgramBuildInfo(program, device[i], CL_PROGRAM_BUILD_LOG, build_log_size, build_log,
									  &build_log_ret);

				/*	Throw error,	*/
				throw RuntimeException(fmt::format("failed to compile CL shader - %s - %s\n", (const char *)build_log,
												   getCLStringError(ciErrNum)));
			}

		} else {
			/*  */
			throw RuntimeException(fmt::format("failed to compile CL shader - %s", getCLStringError(ciErrNum)));
		}
	}

	return program;
}

cl_kernel createKernel(cl_program program, const char *name) {

	cl_int ciErrNum;
	cl_kernel kernel;

	kernel = clCreateKernel(program, name, &ciErrNum);

	/*  Check error.    */
	if (ciErrNum != CL_SUCCESS || !kernel) {
		throw RuntimeException(
			fmt::format("failed to create OpeNCL kernel from program - %s\n", getCLStringError(ciErrNum)));
		return NULL;
	}
	return kernel;
}

cl_command_queue createCommandQueue(cl_context context, cl_device_id device) {

	cl_int ciErrNum;
	cl_command_queue queue;
	cl_command_queue_properties pro = 0;

	/*  Create command.	*/
	queue = clCreateCommandQueue(context, device, pro, &ciErrNum);
	/*  Check error.    */
	if (ciErrNum != CL_SUCCESS)
		throw RuntimeException(fmt::format("failed to create command queue - %s\n", getCLStringError(ciErrNum)));

	return queue;
}
//
// cl_mem createGLWTextureMem(cl_context context, MDTexture *texture) {
//	cl_int ciErrNum;
//	cl_mem mem;
//
//#ifndef MDHEADLESS
//	if (!glIsTexture(texture->texture))
//		return NULL;
//#endif
//
//	mem = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, texture->target, 0, texture->texture, &ciErrNum);
//	if (ciErrNum != CL_SUCCESS) {
//		fprintf(stderr, "clCreateFromGLTexture failed: %s\n", getCLStringError(ciErrNum));
//		return NULL;
//	}
//
//	return mem;
//}

cl_mem createWTextureMem(cl_context context, int width, int height) {
	cl_int ciErrNum;
	cl_image_desc desc;
	cl_image_format form;

	/*  Image description.  */
	desc.image_type = CL_MEM_OBJECT_IMAGE2D;
	desc.image_width = (size_t)width;
	desc.image_height = (size_t)height;
	desc.image_depth = 1;
	desc.image_array_size = 0;
	desc.image_row_pitch = 0;
	desc.image_slice_pitch = 0;
	desc.num_mip_levels = 0;
	desc.num_samples = 0;
	desc.buffer = NULL;

	/*  Format. */
	form.image_channel_order = CL_RGBA;
	form.image_channel_data_type = CL_UNSIGNED_INT8;

	/*  */
	cl_mem mem = clCreateImage(context, CL_MEM_WRITE_ONLY, &form, &desc, NULL, &ciErrNum);
	if (ciErrNum != CL_SUCCESS)
		throw RuntimeException(fmt::format("clCreateImage failed: %s\n", getCLStringError(ciErrNum)));

	return mem;
}

void aquireGLObject(cl_command_queue queue, cl_mem mem) {

	cl_int ciErrNum;

	/*	*/
	ciErrNum = clEnqueueAcquireGLObjects(queue, 1, (const cl_mem *)&mem, 0, NULL, NULL);

	if (ciErrNum != CL_SUCCESS) {
		fprintf(stderr, "failed to aquire OpenCL/GL buffer object - %s\n", getCLStringError(ciErrNum));
	}
}

void releaseGLObject(cl_command_queue queue, cl_mem mem) {

	cl_int ciErrNum;

	ciErrNum = clEnqueueReleaseGLObjects(queue, 1, (const cl_mem *)&mem, 0, NULL, NULL);

	if (ciErrNum != CL_SUCCESS) {
		fprintf(stderr, "failed to release OpenCL/GL buffer object - %s\n", getCLStringError(ciErrNum));
	}
}

IRenderer::IRenderer(IConfig *config) {

	assert(config);

	OpenCLCore *clCore;
	this->pdata = (OpenCLCore *)malloc(sizeof(OpenCLCore));
	clCore = (OpenCLCore *)this->pdata;

	/*  Requires additional rendering interface to work.     */
	clCore->anInterface = eOpenGL;
	clCore->back_renderer = RenderingFactory::createRendering(RenderingFactory::OpenGL, (IConfig *)config);
	/*  */
	if (clCore->back_renderer == NULL) {
		clCore->anInterface = eVulkan;
		clCore->back_renderer = RenderingFactory::createRendering(RenderingFactory::Vulkan, (IConfig *)config);
	}

	/*  */
	if (clCore->back_renderer == NULL) {
		clCore->anInterface = eDirectX;
		clCore->back_renderer = RenderingFactory::createRendering(RenderingFactory::DirectX, (IConfig *)config);
	}

	if (this->pdata == NULL)
		throw RuntimeException("No valid back renderer for the OpenCL Rendering interface.");
}

IRenderer::~IRenderer(void) {
	OpenCLCore *clCore = (OpenCLCore *)this->pdata;
	delete clCore->back_renderer;
	free(this->pdata);
}

Texture *IRenderer::createTexture(TextureDesc *desc) {
	OpenCLCore *core = (OpenCLCore *)this->pdata;
	return core->back_renderer->createTexture(desc);
}

void IRenderer::deleteTexture(Texture *texture) { OpenCLCore *core = (OpenCLCore *)this->pdata; }

RenderPipeline *IRenderer::createPipeline(const ProgramPipelineDesc *desc) {
	OpenCLCore *core = (OpenCLCore *)this->pdata;
	unsigned int pipeline;

	return core->back_renderer->createPipeline(desc);
}

void IRenderer::deletePipeline(RenderPipeline *obj) {
	OpenCLCore *core = (OpenCLCore *)this->pdata;

	core->back_renderer->deletePipeline(obj);
}

Shader *IRenderer::createShader(ShaderDesc *desc) {
	OpenCLCore *core = (OpenCLCore *)this->pdata;
	CLShaderObject *shaderObject;
	Shader *shader;

	if (desc->Compute.numcompute <= 0)
		return core->back_renderer->createShader(desc);

	cl_program program = createProgram(core->context, core->ndevices, core->devices, desc->Compute.computeSource[0]);
	shaderObject = new CLShaderObject();
	shader = new Shader();

	shader->pdata = shaderObject;
	shaderObject->program = program;

	return shader;
}

void IRenderer::deleteShader(Shader *shader) {
	OpenCLCore *core = (OpenCLCore *)this->pdata;

	return core->back_renderer->deleteShader(shader);
}

Buffer *IRenderer::createBuffer(BufferDesc *desc) {
	OpenCLCore *core = (OpenCLCore *)this->pdata;
	return core->back_renderer->createBuffer(desc);
}

void IRenderer::deleteBuffer(Buffer *object) { OpenCLCore *core = (OpenCLCore *)this->pdata; }

Geometry *IRenderer::createGeometry(GeometryDesc *desc) {
	OpenCLCore *core = (OpenCLCore *)this->pdata;
	return core->back_renderer->createGeometry(desc);
}

void IRenderer::deleteGeometry(Geometry *obj) {
	OpenCLCore *core = (OpenCLCore *)this->pdata;
	return core->back_renderer->deleteGeometry(obj);
}

FrameBuffer *IRenderer::createFrameBuffer(FrameBufferDesc *desc) {
	OpenCLCore *core = (OpenCLCore *)this->pdata;
	return NULL;
}

void IRenderer::deleteFrameBuffer(FrameBuffer *obj) { OpenCLCore *core = (OpenCLCore *)this->pdata; }

RendererWindow *IRenderer::createWindow(int x, int y, int width, int height) {
	OpenCLCore *clCore = (OpenCLCore *)this->pdata;

	/*  Create window.  */
	clCore->window = clCore->back_renderer->createWindow(x, y, width, height);

	/*  Create OpenCL context.  */
	// clCore->context = (cl_context)createCLContext(&clCore->ndevices, &clCore->devices, clCore->window, NULL);
	// clCore->selectDevice = clCore->devices[0];

	/*  Create OpenCL command queue.    */
	clCore->selectqueue = createCommandQueue(clCore->context, clCore->selectDevice);

	return NULL; // clCore->window;
}

void IRenderer::createSwapChain() {
	OpenCLCore *core = (OpenCLCore *)this->pdata;
	core->back_renderer->createSwapChain();
}

void IRenderer::clear(unsigned int bitflag) {
	OpenCLCore *core = (OpenCLCore *)this->pdata;
	core->back_renderer->clear(bitflag);
}

void IRenderer::clearColor(float r, float g, float b, float a) {
	OpenCLCore *core = (OpenCLCore *)this->pdata;
	core->back_renderer->clearColor(r, g, b, a);
}

void IRenderer::setVSync(int sync) {
	OpenCLCore *core = (OpenCLCore *)this->pdata;
	core->back_renderer->setVSync(sync);
}

void IRenderer::setDepthMask(bool flag) {
	OpenCLCore *core = (OpenCLCore *)this->pdata;
	core->back_renderer->setDepthMask(flag);
}

void IRenderer::enableState(IRenderer::State state) {
	OpenCLCore *core = (OpenCLCore *)this->pdata;
	core->back_renderer->enableState(state);
}

void IRenderer::disableState(IRenderer::State state) {
	OpenCLCore *core = (OpenCLCore *)this->pdata;
	core->back_renderer->disableState(state);
}

void IRenderer::swapBuffer(void) {
	OpenCLCore *core = (OpenCLCore *)this->pdata;
	core->back_renderer->swapBuffer();
}

void IRenderer::drawInstance(Geometry *geometry, unsigned int num) {
	OpenCLCore *core = (OpenCLCore *)this->pdata;
	core->back_renderer->drawInstance(geometry, num);
}

void IRenderer::dispatchCompute(unsigned int *global, unsigned int *local, unsigned int offset) {
	// clEnqueueNDRangeKernel()
}

void IRenderer::setDebug(bool enable) {
	OpenCLCore *core = (OpenCLCore *)this->pdata;
	core->back_renderer->setDebug(enable);

	/*  */
}
const char *IRenderer::getShaderVersion(ShaderLanguage language) const {
	static char shaderversion[64];
	OpenCLCore *core = (OpenCLCore *)this->pdata;
	if (language != ShaderLanguage::CLC)
		return core->back_renderer->getShaderVersion(language);
	else {
		clGetDeviceInfo(core->selectDevice, CL_DEVICE_OPENCL_C_VERSION, sizeof(shaderversion), &shaderversion, NULL);
		return shaderversion;
	}
}

ShaderLanguage IRenderer::getShaderLanguage(void) const {
	OpenCLCore *core = (OpenCLCore *)this->getData();
	return (ShaderLanguage)(CLC | core->back_renderer->getShaderLanguage());
}

const char *IRenderer::getAPIVersion(void) const {
	OpenCLCore *core = (OpenCLCore *)this->pdata;
	const char *v;

	clGetDeviceInfo(core->selectDevice, CL_DEVICE_VERSION, sizeof(v), &v, NULL);
	return v;
}

const char *IRenderer::getVersion(void) const {
	return ""; // FV_STR_VERSION(RENDER_VULKAN_MAJOR, RENDER_VULKAN_MINOR, RENDER_VULKAN_PATCH);
}

/*const char *IRenderer::getName(void) const {
	return "OpenCL";
}*/

void IRenderer::getSupportedTextureCompression(TextureDesc::Compression *pCompressions) {
	OpenCLCore *core = (OpenCLCore *)this->getData();
}

void IRenderer::getCapability(Capability *capability) {
	OpenCLCore *core = (OpenCLCore *)this->getData();
	// clGetDeviceInfo
	// CL_DEVICE_GLOBAL_MEM_SIZE
}

void IRenderer::getStatus(MemoryInfo *memoryInfo) { OpenCLCore *core = (OpenCLCore *)this->getData(); }

extern "C" IRenderer *createInternalRenderer(IConfig *options) { return new IRenderer(options); }