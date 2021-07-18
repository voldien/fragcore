#ifndef _DEBUG_TRACER_H_
#define _DEBUG_TRACER_H_ 1

namespace DebugMarker
{
	// Get function pointers for the debug report extensions from the device
	void setup(VkDevice device)

	// Sets the debug name of an object
	// All Objects in Vulkan are represented by their 64-bit handles which are passed into this function
	// along with the object type
	void setObjectName(VkDevice device, uint64_t object, VkDebugReportObjectTypeEXT objectType, const char *name)

	// Set the tag for an object
	void setObjectTag(VkDevice device, uint64_t object, VkDebugReportObjectTypeEXT objectType, uint64_t name, size_t tagSize, const void* tag)

	// Start a new debug marker region
	void beginRegion(VkCommandBuffer cmdbuffer, const char* pMarkerName, glm::vec4 color)

	// Insert a new debug marker into the command buffer
	void insert(VkCommandBuffer cmdbuffer, std::string markerName, glm::vec4 color)

	// End the current debug marker region
	void endRegion(VkCommandBuffer cmdBuffer)
};

#endif