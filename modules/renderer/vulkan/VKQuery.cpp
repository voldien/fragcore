#include "Renderer/Query.h"
#include "internal_object_type.h"
using namespace fragcore;

QueryObject::~QueryObject(void) {}

void QueryObject::begin(Target target, unsigned int index) {}

void QueryObject::end(Target target, unsigned int index) {}

long int QueryObject::getResult(int index) {}

void QueryObject::beginConditionalRenderer(Condition target) {}

void QueryObject::endConditionalRenderer(void) {}

intptr_t QueryObject::getNativePtr(void) const { return 0; }
