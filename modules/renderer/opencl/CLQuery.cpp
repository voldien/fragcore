#include "Renderer/Query.h"
#include "Renderer/opencl/internal_object_type.h"
using namespace fragcore;

QueryObject::~QueryObject() {}

void QueryObject::begin(Target target, unsigned int index) {
	// GLQuery* query = (GLQuery*)obj;

	// glBeginQueryIndexed(getTarget(target), index, query->query);
}

void QueryObject::end(Target target, unsigned int index) {
	// GLQuery* query = (GLQuery*)obj;

	// glEndQueryIndexed(getTarget(target), index);

	// glBeginConditionalRender
}

intptr_t QueryObject::getNativePtr() const { return 0; }
