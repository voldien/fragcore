#include "GLRenderWindow.h"

#include <SDL2/SDL_egl.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_syswm.h>
#include <SDLWindowManager.h>

#include <fmt/core.h>

using namespace fragcore;

GLRenderWindow::GLRenderWindow(Ref<GLRendererInterface> &renderer) : renderer(renderer) {

	SDL_WindowFlags window_flags =
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

GLRenderWindow::~GLRenderWindow() = default;

void GLRenderWindow::swapBuffer() { SDL_GL_SwapWindow(this->window); }

void GLRenderWindow::vsync(bool state) { int errcode = SDL_GL_SetSwapInterval(state); }

bool GLRenderWindow::assertConfigAttributes(const fragcore::IConfig *iConfig) { return false; }

void GLRenderWindow::createWindow(int x, int y, int width, int height, const char *api) {
	this->window = SDL_CreateWindow(
		"", x, y, width, height, SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_OPENGL);

	/*  */
	if (window == nullptr) {
		throw RuntimeException("Failed to create window {} for API {}", SDL_GetError(), api);
	}
	// this->api = api;
}

void GLRenderWindow::useWindow([[maybe_unused]] void *pdata) {}

void GLRenderWindow::createSwapChain() {}