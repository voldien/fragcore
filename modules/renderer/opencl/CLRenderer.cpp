
#include "Renderer/IRenderer.h"
#include "Renderer/RenderDesc.h"
#include "Renderer/RendererFactory.h"
#include "Renderer/opencl/internal_object_type.h"

#include <SDL2/SDL_syswm.h>

using namespace fragcore;

IRenderer::IRenderer(IConfig *config) {

	/*  Requires additional rendering interface to work.     */
	clCore->anInterface = eOpenGL;
	clCore->back_renderer = RenderingFactory::createRendering(RenderingFactory::OpenGL, (IConfig *)config);
	/*  */
	if (clCore->back_renderer == nullptr) {
		clCore->anInterface = eVulkan;
		clCore->back_renderer = RenderingFactory::createRendering(RenderingFactory::Vulkan, (IConfig *)config);
	}

	/*  */
	if (clCore->back_renderer == nullptr) {
		clCore->anInterface = eDirectX;
		clCore->back_renderer = RenderingFactory::createRendering(RenderingFactory::DirectX, (IConfig *)config);
	}

	if (this->pdata == nullptr) {
		throw RuntimeException("No valid back renderer for the OpenCL Rendering interface.");
	}
}

IRenderer::~IRenderer() {
	OpenCLCore *clCore = (OpenCLCore *)this->pdata;
	delete clCore->back_renderer;
	free(this->pdata);
}

Texture *IRenderer::createTexture(TextureDesc *desc) { return core->back_renderer->createTexture(desc); }

void IRenderer::deleteTexture(Texture *texture) {}

RenderPipeline *IRenderer::createPipeline(const ProgramPipelineDesc *desc) {

	unsigned int pipeline;

	return core->back_renderer->createPipeline(desc);
}

void IRenderer::deletePipeline(RenderPipeline *obj) { core->back_renderer->deletePipeline(obj); }

Shader *IRenderer::createShader(ShaderDesc *desc) {

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

void IRenderer::deleteShader(Shader *shader) { return core->back_renderer->deleteShader(shader); }

Buffer *IRenderer::createBuffer(BufferDesc *desc) { return core->back_renderer->createBuffer(desc); }

void IRenderer::deleteBuffer(Buffer *object) {}

Geometry *IRenderer::createGeometry(GeometryDesc *desc) { return core->back_renderer->createGeometry(desc); }

void IRenderer::deleteGeometry(Geometry *obj) { return core->back_renderer->deleteGeometry(obj); }

FrameBuffer *IRenderer::createFrameBuffer(FrameBufferDesc *desc) { return nullptr; }

void IRenderer::deleteFrameBuffer(FrameBuffer *obj) {}

RendererWindow *IRenderer::createWindow(int x, int y, int width, int height) {
	OpenCLCore *clCore = (OpenCLCore *)this->pdata;

	/*  Create window.  */
	clCore->window = clCore->back_renderer->createWindow(x, y, width, height);

	/*  Create OpenCL context.  */
	// clCore->context = (cl_context)createCLContext(&clCore->ndevices, &clCore->devices, clCore->window, nullptr);
	// clCore->selectDevice = clCore->devices[0];

	/*  Create OpenCL command queue.    */
	clCore->selectqueue = createCommandQueue(clCore->context, clCore->selectDevice);

	return nullptr; // clCore->window;
}

void IRenderer::createSwapChain() { core->back_renderer->createSwapChain(); }

void IRenderer::clear(unsigned int bitflag) { core->back_renderer->clear(bitflag); }

void IRenderer::clearColor(float r, float g, float b, float a) { core->back_renderer->clearColor(r, g, b, a); }

void IRenderer::setVSync(int sync) { core->back_renderer->setVSync(sync); }

void IRenderer::setDepthMask(bool flag) { core->back_renderer->setDepthMask(flag); }

void IRenderer::enableState(IRenderer::State state) { core->back_renderer->enableState(state); }

void IRenderer::disableState(IRenderer::State state) { core->back_renderer->disableState(state); }

void IRenderer::swapBuffer() { core->back_renderer->swapBuffer(); }

void IRenderer::drawInstance(Geometry *geometry, unsigned int num) { core->back_renderer->drawInstance(geometry, num); }

void IRenderer::dispatchCompute(unsigned int *global, unsigned int *local, unsigned int offset) {
	// clEnqueueNDRangeKernel()
}

void IRenderer::setDebug(bool enable) {

	core->back_renderer->setDebug(enable);

	/*  */
}
const char *IRenderer::getShaderVersion(ShaderLanguage language) const {
	static char shaderversion[64];

	if (language != ShaderLanguage::CLC)
		return core->back_renderer->getShaderVersion(language);
	else {
		clGetDeviceInfo(core->selectDevice, CL_DEVICE_OPENCL_C_VERSION, sizeof(shaderversion), &shaderversion, nullptr);
		return shaderversion;
	}
}

ShaderLanguage IRenderer::getShaderLanguage() const {
	OpenCLCore *core = (OpenCLCore *)this->getData();
	return (ShaderLanguage)(CLC | core->back_renderer->getShaderLanguage());
}

const char *IRenderer::getAPIVersion() const {

	const char *v;

	clGetDeviceInfo(core->selectDevice, CL_DEVICE_VERSION, sizeof(v), &v, nullptr);
	return v;
}

const char *IRenderer::getVersion() const {
	return ""; // FV_STR_VERSION(RENDER_VULKAN_MAJOR, RENDER_VULKAN_MINOR, RENDER_VULKAN_PATCH);
}

/*const char *IRenderer::getName() const {
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