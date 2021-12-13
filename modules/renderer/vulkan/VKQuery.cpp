#include "../Query.h"
#include "internal_object_type.h"
using namespace fragcore;

QueryObject::~QueryObject() {}

void QueryObject::begin(Target target, unsigned int index) {}

void QueryObject::end(Target target, unsigned int index) {}

long int QueryObject::getResult(int index) { return 0; }

void QueryObject::beginConditionalRenderer(Condition target) {}

void QueryObject::endConditionalRenderer() {}

intptr_t QueryObject::getNativePtr() const { return 0; }
