#include"Renderer/IRenderer.h"
#include"Renderer/Geometry.h"
#include"Renderer/Buffer.h"
#include"Renderer/opengl/internal_object_type.h"
#include<GL/glew.h>
using namespace fragcore;

unsigned int Geometry::getVertexCount() {
	GLGeometryObject *geobj = (GLGeometryObject *) this->pdata;

	return geobj->desc.numVerticecs;
}

unsigned int Geometry::getIndicesCount(void) {
	GLGeometryObject *geobj = (GLGeometryObject *) this->pdata;

	return geobj->desc.numIndices;
}


intptr_t Geometry::getNativePtr(void) const {
	return 0;
}