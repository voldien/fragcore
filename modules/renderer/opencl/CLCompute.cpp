#include "CLCompute.h"

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

int supportgl(char *extenions) { return strstr(extenions, "cl_khr_gl_sharing") != nullptr; }

cl_context createCLContext(std::vector<cl_device_id> &devices, SDL_Window *window, SDL_GLContext glcontext) {

	cl_int ciErrNum;
	cl_context context;
	cl_platform_id *platforms;
#ifndef MDHEADLESS
	SDL_SysWMinfo sysinfo;
#endif
	cl_device_id gpuShareId;
	clGetGLContextInfoKHR_fn clGetGLContextInfoKHR;

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
		(cl_context_properties) nullptr,
#ifndef MDHEADLESS
		glcontext ? CL_GL_CONTEXT_KHR : 0,
		(cl_context_properties)glcontext,
#ifdef SDL_VIDEO_DRIVER_X11
		glcontext ? CL_GLX_DISPLAY_KHR : 0,
		glcontext ? (cl_context_properties)sysinfo.info.x11.display : nullptr,
#endif
#endif
		(cl_context_properties) nullptr,
		nullptr,
	};

	unsigned int x, y;
	unsigned int nPlatforms;

	/*	Get Number of platform ids.	*/
	ciErrNum = clGetPlatformIDs(0, nullptr, &nPlatforms);
	if (ciErrNum != CL_SUCCESS)
		throw RuntimeException("failed to get number of OpenCL platforms - {}", getCLStringError(ciErrNum));

	/*  Get platform IDs.	*/
	platforms = (cl_platform_id *)malloc(sizeof(*platforms) * nPlatforms);
	ciErrNum = clGetPlatformIDs(nPlatforms, platforms, nullptr);
	if (ciErrNum != CL_SUCCESS)
		throw RuntimeException("failed to get OpenCL platforms - {}", getCLStringError(ciErrNum));

	/*	Iterate through each platform in till the
	 *	platform associated with OpenGL context is found. */
	for (x = 0; x < nPlatforms; x++) {
		size_t pvar = 4;
		char *extenions;

		props[1] = (cl_context_properties)platforms[x];

		/* TODO change to the device extension instead!

		ciErrNum = clGetPlatformInfo(platforms[x], CL_PLATFORM_EXTENSIONS, sizeof(extenions), &extenions, &pvar);
		if(ciErrNum != CL_SUCCESS){
			fprintf(stderr, "clGetPlatformInfo failed: {}", getCLStringError(ciErrNum));
			return nullptr;
		}
		*/

		/*	Get associated OpenGL context GPU device.	*/
		if (glcontext && supportgl(extenions)) {

			/*	Get extension function from the platform.	*/
			clGetGLContextInfoKHR = (clGetGLContextInfoKHR_fn)clGetExtensionFunctionAddressForPlatform(
				platforms[x], "clGetGLContextInfoKHR");
			if (clGetGLContextInfoKHR == nullptr) {
				throw RuntimeException("clGetExtensionFunctionAddressForPlatform failed");
			}

			pvar = 0;
			ciErrNum = clGetGLContextInfoKHR(props, CL_CURRENT_DEVICE_FOR_GL_CONTEXT_KHR, sizeof(gpuShareId),
											 &gpuShareId, &pvar);
			if (ciErrNum != CL_SUCCESS) {
				throw RuntimeException("failed to get OpenGL context info - {}", getCLStringError(ciErrNum));
			}
		} else {
			int nrDevices;
			/*	get device ids for the GPUS.	*/
			ciErrNum = clGetDeviceIDs(platforms[x], CL_DEVICE_TYPE_GPU, 0, nullptr, &nrDevices);
			if (ciErrNum != CL_SUCCESS) {
				throw RuntimeException("failed to get Device ID number - {}", getCLStringError(ciErrNum));
			}

			devices.resize(nrDevices);
			ciErrNum = clGetDeviceIDs(platforms[x], CL_DEVICE_TYPE_GPU, nrDevices, devices.data(), (cl_uint *)&pvar);
			if (ciErrNum != CL_SUCCESS) {
				throw RuntimeException("failed to get Device ID poiners - {}", getCLStringError(ciErrNum));
			}

			/*  Check which devices support image. */
			for (y = 0; y < *ndevices; y++) {
				cl_bool imageSupport;
				clGetDeviceInfo(*devices[y], CL_DEVICE_IMAGE_SUPPORT, sizeof(cl_bool), &imageSupport, nullptr);
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
	context = clCreateContext(props, devices.size(), devices.data(), nullptr, nullptr, &ciErrNum);

	/*  Error check.    */
	if (context == nullptr || ciErrNum != CL_SUCCESS) {
		throw RuntimeException("failed to create OpenCL context - {}", getCLStringError(ciErrNum));
	}

	free(platforms);
	return context;
}

cl_program createProgram(cl_context context, unsigned int nDevices, cl_device_id *device, const char *source) {
	cl_int ciErrNum;
	cl_program program;

	assert(context);
	/*  TODO add exception. */
	if (device == nullptr || nDevices < 1) {
		throw RuntimeException(fmt::format("Failed to create program CL shader -  {}", getCLStringError(ciErrNum)));
	}

	/*	*/
	program = clCreateProgramWithSource(context, 1, (const char **)&source, nullptr, &ciErrNum);
	if (program == nullptr || ciErrNum != CL_SUCCESS) {
		throw RuntimeException(fmt::format("Failed to create program CL shader -  {}", getCLStringError(ciErrNum)));
	}

	/*	Compile and build CL program.   */
	ciErrNum = clBuildProgram(program, nDevices, device, "", nullptr, nullptr);
	if (ciErrNum != CL_SUCCESS) {
		if (ciErrNum == CL_BUILD_PROGRAM_FAILURE) {
			char build_log[1024];
			size_t build_log_size = sizeof(build_log);
			size_t build_log_ret;
			for (size_t i = 0; i < nDevices; i++) {
				/*	Fetch build log.	*/
				clGetProgramBuildInfo(program, device[i], CL_PROGRAM_BUILD_LOG, build_log_size, build_log,
									  &build_log_ret);

				/*	Throw error,	*/
				throw RuntimeException("failed to compile CL shader - {} - {}", (const char *)build_log,
									   getCLStringError(ciErrNum));
			}

		} else {
			/*  */
			throw RuntimeException("failed to compile CL shader - {}", getCLStringError(ciErrNum));
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
		throw RuntimeException("failed to create OpeNCL kernel from program - {}", getCLStringError(ciErrNum));
		return nullptr;
	}
	return kernel;
}


//
// cl_mem createGLWTextureMem(cl_context context, MDTexture *texture) {
//	cl_int ciErrNum;
//	cl_mem mem;
//
//#ifndef MDHEADLESS
//	if (!glIsTexture(texture->texture))
//		return nullptr;
//#endif
//
//	mem = clCreateFromGLTexture(context, CL_MEM_READ_WRITE, texture->target, 0, texture->texture, &ciErrNum);
//	if (ciErrNum != CL_SUCCESS) {
//		fprintf(stderr, "clCreateFromGLTexture failed: {}", getCLStringError(ciErrNum));
//		return nullptr;
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
	desc.buffer = nullptr;

	/*  Format. */
	form.image_channel_order = CL_RGBA;
	form.image_channel_data_type = CL_UNSIGNED_INT8;

	/*  */
	cl_mem mem = clCreateImage(context, CL_MEM_WRITE_ONLY, &form, &desc, nullptr, &ciErrNum);
	if (ciErrNum != CL_SUCCESS)
		throw RuntimeException(fmt::format("clCreateImage failed: {}", getCLStringError(ciErrNum)));

	return mem;
}

void aquireGLObject(cl_command_queue queue, cl_mem mem) {

	cl_int ciErrNum;

	/*	*/
	ciErrNum = clEnqueueAcquireGLObjects(queue, 1, (const cl_mem *)&mem, 0, nullptr, nullptr);

	if (ciErrNum != CL_SUCCESS) {
		fprintf(stderr, "failed to aquire OpenCL/GL buffer object - {}", getCLStringError(ciErrNum));
	}
}

void releaseGLObject(cl_command_queue queue, cl_mem mem) {

	cl_int ciErrNum;

	ciErrNum = clEnqueueReleaseGLObjects(queue, 1, (const cl_mem *)&mem, 0, nullptr, nullptr);

	if (ciErrNum != CL_SUCCESS) {
		fprintf(stderr, "failed to release OpenCL/GL buffer object - {}", getCLStringError(ciErrNum));
	}
}

CLCompute::CLCompute() {

	int nr_devices;
	this->context = (cl_context)createCLContext(&nr_devices, &clCore->devices, nullptr, nullptr);

	// /*  Requires additional rendering interface to work.     */
	// clCore->anInterface = eOpenGL;
	// clCore->back_renderer = RenderingFactory::createRendering(RenderingFactory::OpenGL, (IConfig *)config);
	// /*  */
	// if (clCore->back_renderer == nullptr) {
	// 	clCore->anInterface = eVulkan;
	// 	clCore->back_renderer = RenderingFactory::createRendering(RenderingFactory::Vulkan, (IConfig *)config);
	// }

	// /*  */
	// if (clCore->back_renderer == nullptr) {
	// 	clCore->anInterface = eDirectX;
	// 	clCore->back_renderer = RenderingFactory::createRendering(RenderingFactory::DirectX, (IConfig *)config);
	// }

	if (this->pdata == nullptr) {
		throw RuntimeException("No valid back renderer for the OpenCL Rendering interface.");
	}
}

bool CLCompute::supportCompute() { return true; }
void CLCompute::OnInitialization() {}
void CLCompute::OnDestruction() {}