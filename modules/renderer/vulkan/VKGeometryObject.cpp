#include "../Buffer.h"
#include "../Geometry.h"
#include "../IRenderer.h"
#include "internal_object_type.h"
using namespace fragcore;

unsigned int Geometry::getVertexCount() {
	VKGeometryObject *geobj = (VKGeometryObject *)this->pdata;

	return geobj->desc.numVerticecs;
}

unsigned int Geometry::getIndicesCount() {
	VKGeometryObject *geobj = (VKGeometryObject *)this->pdata;

	return geobj->desc.numIndices;
}

intptr_t Geometry::getNativePtr() const { return 0; }