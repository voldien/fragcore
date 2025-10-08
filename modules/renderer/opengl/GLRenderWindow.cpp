#include "GLRenderWindow.h"
#include <SDLWindowManager.h>

#include "FragDef.h"
#include "SDL_error.h"
#include "SDL_video.h"
#include <SDL2/SDL_egl.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_syswm.h>

#if defined(FRAG_CORE_INTERNAL_IMP) // TODO resolve to a single file or
									// something later
#include <SDL2/SDL_video.h>
#endif

#include <fmt/core.h>

using namespace fragcore;

GLRenderWindow::GLRenderWindow(const Ref<GLRendererInterface> &renderer) : renderer(renderer) {

	const SDL_WindowFlags window_flags =
		(SDL_WindowFlags)(SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_OPENGL);
	this->window = SDL_CreateWindow("", 0, 0, 800, 600, window_flags);

	/*  */
	if (this->window == nullptr) {
		throw RuntimeException("Failed to create window {} for API {}", SDL_GetError(), api);
	}

	if (SDL_GL_MakeCurrent(this->window, renderer->getOpenGLContext()) != 0) {
		throw RuntimeException("Failed to make OpenGL Current {}", SDL_GetError());
	}
}

GLRenderWindow::~GLRenderWindow() {
	SDL_DestroyWindow(this->window);
	SDL_GL_MakeCurrent(nullptr, renderer->getOpenGLContext());
	this->window = nullptr;
}

void GLRenderWindow::swapBuffer() { SDL_GL_SwapWindow(this->window); }

void GLRenderWindow::vsync(bool state) {
	int errcode = SDL_GL_SetSwapInterval(state);
	if (!errcode) {
		throw RuntimeException("Failed to Set Swap Interval {}", SDL_GetError());
	}
}

bool GLRenderWindow::assertConfigAttributes(const fragcore::IConfig *iConfig) { return false; }

void GLRenderWindow::createWindow(int window_x, int window_y, int width, int height, const char *api) {
	const SDL_WindowFlags window_flags =
		(SDL_WindowFlags)(SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_OPENGL);
	this->window = SDL_CreateWindow("", window_x, window_y, width, height, window_flags);

	/*  */
	if (window == nullptr) {
		throw RuntimeException("Failed to create window {} for API {}", SDL_GetError(), api);
	}
	// this->api = api;
}

void GLRenderWindow::useWindow([[maybe_unused]] void *pdata) {}

void GLRenderWindow::createSwapChain() {}