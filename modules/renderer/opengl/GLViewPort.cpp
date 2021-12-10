#include "../ViewPort.h"
#include "internal_object_type.h"
#include <GL/glew.h>
using namespace fragcore;

void ViewPort::depthRange(double near, double far) {
	const GLViewPort *viewPort = (const GLViewPort *)this->getObject();
	glDepthRangeIndexed(viewPort->viewport, near, far);
}

void ViewPort::setDimensions(int x, int y, int width, int height) {
	const GLViewPort *viewPort = (const GLViewPort *)this->getObject();
	glViewportIndexedf(viewPort->viewport, (GLfloat)x, (GLfloat)y, (GLfloat)width, (GLfloat)height);
}

void ViewPort::setscissorView(int x, int y, int width, int height) {
	const GLViewPort *viewPort = (const GLViewPort *)this->getObject();
	glScissorIndexed(viewPort->viewport, x, y, width, height);
}

void ViewPort::getViewPort(int *x, int *y, int *width, int *height) {
	const GLViewPort *viewPort = (const GLViewPort *)this->getObject();

	GLint dim[4];
	glGetIntegeri_v(GL_VIEWPORT, viewPort->viewport, dim);

	if (x)
		*x = dim[0];
	if (y)
		*y = dim[1];
	if (width)
		*width = dim[2];
	if (height)
		*height = dim[3];
}

void ViewPort::getScissorView(int *x, int *y, int *width, int *height) {
	const GLViewPort *viewPort = (const GLViewPort *)this->getObject();

	GLint dim[4];
	glGetIntegeri_v(GL_SCISSOR_BOX, viewPort->viewport, dim);

	if (x)
		*x = dim[0];
	if (y)
		*y = dim[1];
	if (width)
		*width = dim[2];
	if (height)
		*height = dim[3];
}

void ViewPort::getDepthRange(double *near, double *far) {
	const GLViewPort *viewPort = (const GLViewPort *)this->getObject();
	double range[2];
	glGetDoublei_v(GL_DEPTH_RANGE, viewPort->viewport, range);
	if (near)
		*near = range[0];
	if (far)
		*far = range[1];
}

void ViewPort::enable(IRenderer::State state) {
	const GLViewPort *viewPort = (const GLViewPort *)this->getObject();
	glEnablei(getState(state), viewPort->viewport);
}
void ViewPort::disable(IRenderer::State state) {
	const GLViewPort *viewPort = (const GLViewPort *)this->getObject();
	glDisablei(getState(state), viewPort->viewport);
}
bool ViewPort::isStateEnabled(IRenderer::State state) {
	const GLViewPort *viewPort = (const GLViewPort *)this->getObject();
	return glIsEnabledi(getState(state), viewPort->viewport);
}

intptr_t ViewPort::getNativePtr() const { return 0; }
