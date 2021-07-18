#include "Renderer/Buffer.h"
#include "Renderer/Geometry.h"
#include "Renderer/IRenderer.h"
#include "internal_object_type.h"
using namespace fragcore;

unsigned int Geometry::getVertexCount() {
	VKGeometryObject *geobj = (VKGeometryObject *)this->pdata;

	return geobj->desc.numVerticecs;
}

unsigned int Geometry::getIndicesCount(void) {
	VKGeometryObject *geobj = (VKGeometryObject *)this->pdata;

	return geobj->desc.numIndices;
}

intptr_t Geometry::getNativePtr(void) const { return 0; }