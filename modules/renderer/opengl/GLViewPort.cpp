#include "GLViewport.h"
#include "internal_object_type.h"
#include <GL/glew.h>
using namespace fragcore;

void GLViewPort::depthRange(double near, double far) { glDepthRangeIndexed(this->viewport, near, far); }

void GLViewPort::setDimensions(int x, int y, int width, int height) {

	glViewportIndexedf(this->viewport, (GLfloat)x, (GLfloat)y, (GLfloat)width, (GLfloat)height);
}

void GLViewPort::setscissorView(int x, int y, int width, int height) {

	glScissorIndexed(this->viewport, x, y, width, height);
}

void GLViewPort::getViewPort(int *x, int *y, int *width, int *height) {

	GLint dim[4];
	glGetIntegeri_v(GL_VIEWPORT, this->viewport, dim);

	if (x) {
		*x = dim[0];
	}
	if (y) {
		*y = dim[1];
	}
	if (width) {
		*width = dim[2];
	}
	if (height) {
		*height = dim[3];
	}
}

void GLViewPort::getScissorView(int *x, int *y, int *width, int *height) {

	GLint dim[4];
	glGetIntegeri_v(GL_SCISSOR_BOX, this->viewport, dim);

	if (x) {
		*x = dim[0];
	}
	if (y) {
		*y = dim[1];
	}
	if (width) {
		*width = dim[2];
	}
	if (height) {
		*height = dim[3];
	}
}

void GLViewPort::getDepthRange(double *near, double *far) {

	double range[2];
	glGetDoublei_v(GL_DEPTH_RANGE, this->viewport, range);
	if (near) {
		*near = range[0];
	}
	if (far) {
		*far = range[1];
	}
}

void GLViewPort::enable(IRenderer::State state) { glEnablei(GLHelper::getState(state), this->viewport); }
void GLViewPort::disable(IRenderer::State state) { glDisablei(GLHelper::getState(state), this->viewport); }
bool GLViewPort::isStateEnabled(IRenderer::State state) { return glIsEnabledi(GLHelper::getState(state), this->viewport); }

intptr_t GLViewPort::getNativePtr() const { return 0; }
