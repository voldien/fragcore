
#include "../IRenderer.h"
#include "../RenderPrerequisites.h"
#include "VKBuffer.h"
#include "VKRenderInterface.h"
#include "internal_object_type.h"
#include <FragCore.h>

using namespace fragcore;

VKBuffer::~VKBuffer() {}

void VKBuffer::bind() {
	VKBufferObject *bufobj = static_cast<VKBufferObject *>(this->pdata);

	//vkBindBufferMemory(bufobj->vulkanCore->device, bufobj->buffer, bufobj->vertexBufferMemory, 0);
}

void VKBuffer::bind(unsigned int offset, unsigned int size) {
	VKBufferObject *bufobj = static_cast<VKBufferObject *>(this->pdata);
	// vkBindBufferMemory(bufobj->vulkanCore->device, bufobj->buffer, bufobj->vertexBufferMemory, offset);
}

void VKBuffer::bindBase(unsigned int base) { VKBufferObject *bufobj = (VKBufferObject *)this->pdata; }

void VKBuffer::subData(const void *data, unsigned int offset, unsigned int size) {
	VKBufferObject *bufobj = (VKBufferObject *)this->pdata;
}

void *VKBuffer::getData(unsigned int offset, unsigned int size) {
	VKBufferObject *bufobj = (VKBufferObject *)this->pdata;
}

bool VKBuffer::isValid() { VKBufferObject *bufobj = (VKBufferObject *)this->pdata; }

long int VKBuffer::getSize() {
	VKBufferObject *bufobj = (VKBufferObject *)this->pdata;
	VkMemoryRequirements requirements;
	// vkGetBufferMemoryRequirements(bufobj->vulkanCore->device, bufobj->buffer, &requirements);
	return requirements.size;
}

void *VKBuffer::mapBuffer(VKBuffer::MapTarget target) {
	void *pbuf;
	VkResult result;
	VkMemoryRequirements memRequirements;
	VkMemoryMapFlags mapTarget = 0;

	VKBufferObject *bufobj = (VKBufferObject *)this->pdata;

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

void *VKBuffer::mapBuffer(MapTarget target, unsigned long int offset, unsigned long int length) {}

void VKBuffer::flush(unsigned long int offset, unsigned long int length) {}

void VKBuffer::unMapBuffer() {
	VKBufferObject *bufobj = (VKBufferObject *)this->pdata;

	vkUnmapMemory(this->getRenderer<VKRenderInterface>()->getDevice()->getHandle(), bufobj->vertexBufferMemory);
}

void VKBuffer::setName(const std::string &name) { Object::setName(name); }

intptr_t VKBuffer::getNativePtr() const { return 0; }
