#include "GLFWWindow.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

using namespace fragcore;

void GLFWWindow::show() { glfwShowWindow(this->window); }

void GLFWWindow::hide() { glfwHideWindow(this->window); }

void GLFWWindow::close() {
	this->hide();
	glfwDestroyWindow(this->window);
}

void GLFWWindow::setPosition(int x, int y) noexcept { glfwSetWindowPos(this->window, x, y); }

void GLFWWindow::setSize(int width, int height) noexcept {}

void GLFWWindow::getPosition(int *x, int *y) const { glfwGetWindowPos(this->window, x, y); }

void GLFWWindow::getSize(int *width, int *height) const { glfwGetWindowSize(this->window, width, height); }
void GLFWWindow::setTitle(const std::string &title) { glfwSetWindowTitle(this->window, title.c_str()); }

std::string GLFWWindow::getTitle() const { return this->title; }

// int GLFWWindow::x() const noexcept {
// 	int x, y;

// 	return x;
// }
// int GLFWWindow::y() const noexcept {
// 	int x, y;

// 	return y;
// }
void GLFWWindow::resizable(bool resizable) noexcept {}

void GLFWWindow::setFullScreen(bool fullscreen) {}

bool GLFWWindow::isFullScreen() const { return false; }

void GLFWWindow::setBordered(bool bordered) {}

// int GLFWWindow::width() const noexcept {
// 	int w, h;
// 	getSize(&w, &h);
// 	return w;
// }
// int GLFWWindow::height() const noexcept {
// 	int w, h;
// 	getSize(&w, &h);
// 	return h;
// }

void GLFWWindow::setMinimumSize(int width, int height) {}
void GLFWWindow::getMinimumSize(int *width, int *height) {}

void GLFWWindow::setMaximumSize(int width, int height) {}
void GLFWWindow::getMaximumSize(int *width, int *height) {}

void GLFWWindow::focus() {}

void GLFWWindow::restore() {}

void GLFWWindow::maximize() {}

void GLFWWindow::minimize() {}