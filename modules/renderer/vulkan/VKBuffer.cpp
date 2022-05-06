
#include "../RenderPrerequisites.h"
#include "VKBuffer.h"
#include "VKRenderInterface.h"
#include "internal_object_type.h"

using namespace fragcore;

VKBuffer::~VKBuffer() {}

void VKBuffer::bind() {

	// vkBindBufferMemory(bufobj->vulkanCore->device, bufobj->buffer, bufobj->vertexBufferMemory, 0);
}

void VKBuffer::bind(unsigned int offset, unsigned int size) {

	// vkBindBufferMemory(bufobj->vulkanCore->device, bufobj->buffer, bufobj->vertexBufferMemory, offset);
}

void VKBuffer::bindBase(unsigned int base) {}

void VKBuffer::subData(const void *data, unsigned int offset, unsigned int size) {}

void *VKBuffer::getData(unsigned int offset, unsigned int size) { return nullptr; }

bool VKBuffer::isValid() { return true; }

long int VKBuffer::getSize() {

	VkMemoryRequirements requirements;
	// vkGetBufferMemoryRequirements(bufobj->vulkanCore->device, bufobj->buffer, &requirements);
	return requirements.size;
}

void *VKBuffer::mapBuffer(VKBuffer::MapTarget target) {
	void *pbuf;
	VkResult result;
	VkMemoryRequirements memRequirements;
	VkMemoryMapFlags mapTarget = 0;

	/*  */
	// vkGetBufferMemoryRequirements(bufobj->vulkanCore->device, bufobj->buffer, &memRequirements);

	/*  */
	// result = vkMapMemory(bufobj->vulkanCore->device, bufobj->vertexBufferMemory, 0, (VkDeviceSize)
	// memRequirements.size,
	//                      mapTarget, &pbuf);
	if (result != VK_SUCCESS)
		throw RuntimeException("");
	return pbuf;
}

void *VKBuffer::mapBuffer(MapTarget target, unsigned long int offset, unsigned long int length) { return nullptr; }

void VKBuffer::flush(unsigned long int offset, unsigned long int length) {

	VkMappedMemoryRange stagingRange{};
	stagingRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	// stagingRange.memory = this->buffer;
	//->getMemoryBuffer();
	stagingRange.offset = 0;
	stagingRange.size = length;

	// vkFlushMappedMemoryRanges(getRenderer<VKRendererInterface>()->getDevice()->getHandle(), 1, &stagingRange);
}

void VKBuffer::unMapBuffer() {

	vkUnmapMemory(this->getRenderer<VKRenderInterface>()->getDevice()->getHandle(), this->vertexBufferMemory);
}

void VKBuffer::setName(const std::string &name) { Object::setName(name); }

intptr_t VKBuffer::getNativePtr() const { return 0; }
