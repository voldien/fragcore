#include <CommandList.h>
#include <FragCore.h>
#include <GLRendererInterface.h>
#include <RendererFactory.h>
#include <RendererWindow.h>
#include <SDL2/SDL.h>
#include <SDLDisplay.h>
#include <SDLWindowManager.h>
#include <VKRenderInterface.h>
#include <ViewPort.h>

using namespace fragcore;
typedef struct _vertex_t {
	float pos[2];
	float color[3];
} Vertex;

const std::vector<Vertex> vertices = {
	{0.0f, -0.5f, 1.0f, 1.0f, 1.0f}, /*	Vertex (2), Color(3)	*/
	{0.5f, 0.5f, 0.0f, 1.0f, 0.0f},	 /*	Vertex (2), Color(3)	*/
	{-0.5f, 0.5f, 0.0f, 0.0f, 1.0f}	 /*	Vertex (2), Color(3)*/
};
int main(int argc, const char **argv) {

	Display *primaryDisplay = new SDLDisplay(0);

	int renderOpt = atoi(argv[1]);
	Ref<IRenderer> renderer = nullptr;
	if (renderOpt == 0) {
		renderer = Ref<IRenderer>(new VKRenderInterface(nullptr));
	} else if (renderOpt == 1) {
		renderer = Ref<IRenderer>(new GLRendererInterface(nullptr));
	} else {
		return EXIT_FAILURE;
	}
	RendererWindow *window = renderer->createWindow(primaryDisplay->width() / 4, primaryDisplay->height() / 4,
													primaryDisplay->width() / 2, primaryDisplay->width() / 2);

	window->vsync(true);
	window->setTitle(fmt::format("Cube Example: {}", renderer->getName()));
	window->show();

	// ShaderUtil::loadProgram()
	// Shader* shader = renderer->createShader(NULL);
	// Geometry* obj= renderer->createGeometry(NULL);
	BufferDesc vertexBufferDesc = {
		.type = BufferDesc::BufferType::eArray,
		.size = vertices.size() * sizeof(Vertex),
		.data = (void *)vertices.data(),
		.hint = (BufferDesc::BufferHint)(BufferDesc::BufferHint::eStatic | BufferDesc::BufferHint::eRead)};
	RenderPipelineDesc pipeineDesc = {};
	// RenderPipeline *RenderPipeline = renderer->createRenderPipeline(&pipeineDesc);
	Buffer *buffer = renderer->createBuffer(&vertexBufferDesc);

	CommandList *clc = renderer->createCommandBuffer();
	clc->begin();
	clc->pushDebugGroup("Main Rendering");
	Ref<FrameBuffer> defaultFramebuffer = Ref<FrameBuffer>(window->getDefaultFrameBuffer());
	clc->bindFramebuffer(defaultFramebuffer);
	clc->setViewport(0, 0, 100, 100);
	clc->clearColorTarget(0, Color(1.0f, 0, 0, 1));

	clc->insertDebugMarker("Rendering Cube");
	//	clc->bindPipeline()

	// clc->bindPipeline()
	// clc->dispatch(0, 0, 0);
	// clc->bindPipeline(NULL);
	// clc->draw
	clc->popDebugGroup();
	clc->end();

	int width, height;
	window->getSize(&width, &height);

	Vector3 cameraPos; //= HCVector3(options->width / -2, options->height / -2, 0);
					   // Quaternion cubeRotation = Quaternion::identity();
					   /*  Set init matrix.	*/
	Matrix4x4 proj, scale, translation, view;
	// proj = PVMatrix4x4::orth(-(float)width / orthdiv, (float)width / orthdiv, -(float)height / orthdiv, (float)height
	// / orthdiv, -1.0f, 1.0f);
	// scale = Matrix4x4::scale(1, 1, 0);
	// translation = Matrix4x4::translate(cameraPos);
	// view = Matrix4x4::identity();

	/*	*/
	// window->getDefaultFrameBuffer()->clearColor(0.125f,0.125f,0.125f,1.0f);
	Ref<CommandList> refClc = Ref<CommandList>(clc);
	SDL_Event event = {};
	const int timeout = 0;
	while (true) {
		while (SDL_WaitEventTimeout(&event, timeout)) {
			switch (event.type) {
			case SDL_QUIT:
				return 0; /*  Exit.  */
			case SDL_KEYDOWN:
				break;
			case SDL_KEYUP:
				break;
			case SDL_MOUSEMOTION:
				break;
			case SDL_MOUSEBUTTONDOWN:
				break;
			case SDL_MOUSEBUTTONUP:
				break;
			case SDL_MOUSEWHEEL:
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
				case SDL_WINDOWEVENT_CLOSE:
					goto done;
				case SDL_WINDOWEVENT_SIZE_CHANGED:
				case SDL_WINDOWEVENT_RESIZED:
					// visible = 1;
					width = (float)event.window.data1;
					height = (float)event.window.data2;
					// viewport->setDimensions(0, 0, screen[0],
					// 						screen[1]);
					// Log::log(Log::Verbose, "%dx%d\n",
					// 			(int)screen[0], (int)screen[1]);

					// proj = HCMatrix4x4::orth(
					// 	-screen[0] / orthdiv, screen[0] / orthdiv,
					// 	-screen[1] / orthdiv, screen[1] / orthdiv,
					// 	-0.0f, 1.0f);
					// needMatrixUpdate = true;
					// window->getViewPort()->setDimensions(0, 0, width, height);
					break;
				case SDL_WINDOWEVENT_HIDDEN:
				case SDL_WINDOWEVENT_MINIMIZED:
					// visible = 0;
					break;
				case SDL_WINDOWEVENT_EXPOSED:
				case SDL_WINDOWEVENT_SHOWN:
					// visible = 1;
					break;
				}
				break;
			default:
				break;
			}
		}
		// window->getDefaultFrameBuffer()->bind();
		// window->getDefaultFrameBuffer()->clear(Color | Depth);

		renderer->submittCommand(refClc);
		/*	Draw rotating cube.	*/
		// cubeRotation *= PVQuaternion::rotate(0.01f,0.01f,0.01f);
		// shader->setMatrix4f(shader->getLocation("mvp"), NULL);
		// shader->bind();
		// renderer->drawInstance(obj, 1);

		// window->getDefaultFrameBuffer()->unBind();
		window->swapBuffer();
	}
done:
	window->close();
	SDL_Quit();
}
