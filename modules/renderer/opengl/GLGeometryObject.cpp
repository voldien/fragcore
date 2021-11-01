#include "../Buffer.h"
#include "../Geometry.h"
#include "../IRenderer.h"
#include "internal_object_type.h"
#include <GL/glew.h>
using namespace fragcore;

unsigned int Geometry::getVertexCount() {
	GLGeometryObject *geobj = (GLGeometryObject *)this->pdata;

	return geobj->desc.numVerticecs;
}

unsigned int Geometry::getIndicesCount() {
	GLGeometryObject *geobj = (GLGeometryObject *)this->pdata;

	return geobj->desc.numIndices;
}

intptr_t Geometry::getNativePtr() const { return 0; }