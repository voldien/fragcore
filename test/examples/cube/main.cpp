#include <FragCore.h>
#include <Renderer/RendererWindow.h>
#include <Renderer/ViewPort.h>
#include <SDL2/SDL.h>
#include <Window/WindowManager.h>
//#include"Config.h"

using namespace fragcore;

int main(int argc, const char **argv) {
	// /*  Read first options.   */
	// Config* config = Config::createConfig(argc, argv,
	// Config::getConfigFilePath(argc, argv));

	SDL_Init(SDL_INIT_EVERYTHING);
	Display *primaryDisplay = WindowManager::getInstance()->getDisplay(0);

	Ref<IRenderer> renderer =
		Ref<IRenderer>(RenderingFactory::createRendering(RenderingFactory::RenderingAPI::Vulkan, NULL));
	RendererWindow *window = renderer->createWindow(primaryDisplay->width() / 4, primaryDisplay->height() / 4,
													primaryDisplay->width() / 2, primaryDisplay->width() / 2);
	window->vsync(true);
	window->setTitle(fmt::format("Cube Example: %s", renderer->getName()).c_str());
	window->show();

	// ShaderUtil::loadProgram()
	// Shader* shader = renderer->createShader(NULL);
	// Geometry* obj= renderer->createGeometry(NULL);

	CommandList *clc = renderer->createCommandBuffer();
	Ref<FrameBuffer> defaultFramebuffer = Ref<FrameBuffer>(renderer->getDefaultFramebuffer(NULL));
	clc->bindFramebuffer(defaultFramebuffer);
	clc->clearColorTarget(0, Color(1, 0, 0, 1));
	// clc->bindPipeline(NULL);
	// clc->draw

	int width, height;
	window->getSize(&width, &height);

	Vector3 cameraPos; //= HCVector3(options->width / -2, options->height / -2, 0);
	Quaternion cubeRotation = Quaternion::identity();
	/*  Set init matrix.	*/
	Matrix4x4 proj, scale, translation, view;
	// proj = PVMatrix4x4::orth(-(float)width / orthdiv, (float)width / orthdiv, -(float)height / orthdiv, (float)height
	// / orthdiv, -1.0f, 1.0f);
	scale = Matrix4x4::scale(1, 1, 0);
	translation = Matrix4x4::translate(cameraPos);
	view = Matrix4x4::identity();

	/*	*/
	// window->getDefaultFrameBuffer()->clearColor(0.125f,0.125f,0.125f,1.0f);

	SDL_Event event = {};
	const int timeout = 0;
	while (1) {
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
					window->getViewPort()->setDimensions(0, 0, width, height);
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
		// window->getDefaultFrameBuffer()->clear(eColor | eDepth);

		// renderer->submittCommand(Ref<CommadList>(clc));
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
	delete *renderer;
	SDL_Quit();
}
