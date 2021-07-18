#include "Renderer/ViewPort.h"
#include "internal_object_type.h"
using namespace fragcore;

void ViewPort::depthRange(double near, double far) {}

void ViewPort::setDimensions(int x, int y, int width, int height) { VKViewPort *viewPort = (VKViewPort *)this->pdata; }

void ViewPort::setscissorView(int x, int y, int width, int height) {}

void ViewPort::getViewPort(int *x, int *y, int *width, int *height) {}

void ViewPort::getScissorView(int *x, int *y, int *width, int *height) {}

void ViewPort::getDepthRange(double *near, double *far) {}

void ViewPort::enable(IRenderer::State state) {}

void ViewPort::disable(IRenderer::State state) {}

bool ViewPort::isStateEnabled(IRenderer::State state) {}

intptr_t ViewPort::getNativePtr(void) const { return 0; }
