#include<FragCore.h>
#include"Renderer/IRenderer.h"
#include"Renderer/Buffer.h"
#include"Renderer/vulkan/internal_object_type.h"
using namespace fragcore;

Buffer::~Buffer(void) {}

void Buffer::bind() {
	VKBufferObject *bufobj = (VKBufferObject *) this->pdata;
	vkBindBufferMemory(bufobj->vulkanCore->device, bufobj->buffer, bufobj->vertexBufferMemory, 0);
}

void Buffer::bind(unsigned int offset, unsigned int size) {
	VKBufferObject *bufobj = (VKBufferObject *) this->pdata;
	vkBindBufferMemory(bufobj->vulkanCore->device, bufobj->buffer, bufobj->vertexBufferMemory, offset);
}


void Buffer::bindBase(unsigned int base) {
	VKBufferObject *bufobj = (VKBufferObject *) this->pdata;
}

void Buffer::subData(const void *data, unsigned int offset, unsigned int size) {
	VKBufferObject *bufobj = (VKBufferObject *) this->pdata;
}

void *Buffer::getData(unsigned int offset, unsigned int size) {
	VKBufferObject *bufobj = (VKBufferObject *) this->pdata;

}

bool Buffer::isValid(void) {
	VKBufferObject *bufobj = (VKBufferObject *) this->pdata;
}

long int Buffer::getSize(void) {
	VKBufferObject *bufobj = (VKBufferObject *) this->pdata;
	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(bufobj->vulkanCore->device, bufobj->buffer, &requirements);
	return requirements.size;
}

void *Buffer::mapBuffer(Buffer::MapTarget target) {
	void *pbuf;
	VkResult result;
	VkMemoryRequirements memRequirements;
	VkMemoryMapFlags mapTarget = 0;

	VKBufferObject *bufobj = (VKBufferObject *) this->pdata;

	/*  */
	vkGetBufferMemoryRequirements(bufobj->vulkanCore->device, bufobj->buffer, &memRequirements);

	/*  */
	result = vkMapMemory(bufobj->vulkanCore->device, bufobj->vertexBufferMemory, 0, (VkDeviceSize) memRequirements.size,
	                     mapTarget, &pbuf);
	if (result != VK_SUCCESS)
		throw RuntimeException("");
	return pbuf;
}

void *Buffer::mapBuffer(MapTarget target, unsigned long int offset, unsigned long int length) {}

void Buffer::flush(unsigned long int offset, unsigned long int length) {}

void Buffer::unMapBuffer(void) {
	VKBufferObject *bufobj = (VKBufferObject *) this->pdata;

	vkUnmapMemory(bufobj->vulkanCore->device->getHandle(), bufobj->vertexBufferMemory);
}

void Buffer::setName(const std::string &name) {
	Object::setName(name);
}

intptr_t Buffer::getNativePtr(void) const {
	return 0;
}
