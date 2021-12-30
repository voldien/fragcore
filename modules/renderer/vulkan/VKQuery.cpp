#include "VKQuery.h"
#include "internal_object_type.h"
using namespace fragcore;

VKQueryObject::~VKQueryObject() {}

void VKQueryObject::begin(Target target, unsigned int index) {}

void VKQueryObject::end(Target target, unsigned int index) {}

long int VKQueryObject::getResult(int index) { return 0; }

void VKQueryObject::beginConditionalRenderer(Condition target) {}

void VKQueryObject::endConditionalRenderer() {}

intptr_t VKQueryObject::getNativePtr() const { return 0; }
