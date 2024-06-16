#include "VKRenderInterface.h"
#include "Core/IConfig.h"

#include "../RenderDesc.h"

#include "VKBuffer.h"
#include "VKCommandList.h"
#include "VKRenderWindow.h"
#include "VKSampler.h"
#include "VKShader.h"
#include "internal_object_type.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_vulkan.h>
#include <SDLWindowManager.h>

#include <VKHelper.h>
#include <VKUtil.h>
#include <climits>
#include <fmt/core.h>
#include <iostream>
#include <vector>

using namespace fragcore;
using namespace fvkcore;

#define RENDER_VULKAN_MAJOR 0
#define RENDER_VULKAN_MINOR 1
#define RENDER_VULKAN_PATCH 0

static bool validate_object_memeber(VKRenderInterface *renderer, RenderObject *object) {
	return renderer == object->getRenderer();
}

VKRenderInterface::VKRenderInterface(IConfig *config) {

	/*	Minimun requirements.	*/
	std::vector<const char *> instanceExtensionNames = {
		/*	*/
		VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME,
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
		VK_KHR_DISPLAY_EXTENSION_NAME,
	};
	const std::vector<const char *> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation",
	};

	VulkanCore *vulkancore = nullptr;
	// IConfig setupConfig;
	/*	Default config parameters.	*/
	if (config == nullptr) {
		// setupConfig.set("debug", true);
		// setupConfig.set("debug-tracer", true);
		// setupConfig.set("gamma-correction", true);
	} else {
		// setupConfig = *config
	}

	/*  Initialize video support.   */
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
		throw RuntimeException(fmt::format("SDL_InitSubSystem failed, %s.\n", SDL_GetError()));
	}

	/*  Store reference with the object. */
	std::unordered_map<const char *, bool> required_instance_extensions = {{VK_KHR_SURFACE_EXTENSION_NAME, true},
																		   {"VK_KHR_xlib_surface", true}};

	std::unordered_map<const char *, bool> required_instance_layer = {{"VK_LAYER_KHRONOS_validation", true}};
	std::unordered_map<const char *, bool> required_device_extensions = {{VK_KHR_SWAPCHAIN_EXTENSION_NAME, true}};

	/*	*/
	this->core = std::make_shared<VulkanCore>(required_instance_extensions, required_instance_layer);
	std::vector<std::shared_ptr<PhysicalDevice>> devices = core->createPhysicalDevices();
	this->device = std::make_shared<VKDevice>(devices, required_device_extensions);

	this->languageSupport = SPIRV;
	this->getCapability(&this->capability);

	//this->queue = getDevice()->getDefaultGraphicQueue();
}

VKRenderInterface::~VKRenderInterface() {
	/*  Release all vulkan resources.   */
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void VKRenderInterface::OnInitialization() {}

void VKRenderInterface::OnDestruction() {}

Texture *VKRenderInterface::createTexture(TextureDesc *desc) {

	VkPhysicalDevice physicalDevice = nullptr;

	unsigned int texWidth, texHeight, internal, type, format;
	unsigned long pixelSize;
	void *pixels;

	VkDeviceSize imageSize = pixelSize;
	VkPhysicalDeviceMemoryProperties memProperties;
	VkCommandPool commandPool;
	VkImage textureImage;
	// TODO replace with buffer object.
	VkDeviceMemory textureImageMemory;

	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	if (!pixels) {
		throw cxxexcept::RuntimeException("failed to load texture image!");
	}

	VkCommandBuffer cmd = VKHelper::beginSingleTimeCommands(getDevice()->getHandle(), commandPool);

	/*	*/
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VKHelper::createBuffer(getDevice()->getHandle(), imageSize, memProperties, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
						   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, stagingBuffer, stagingBufferMemory);

	void *stageData;
	vkMapMemory(getDevice()->getHandle(), stagingBufferMemory, 0, imageSize, 0, &stageData);
	memcpy(stageData, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(getDevice()->getHandle(), stagingBufferMemory);

	/*	Create staging buffer.	*/
	VKHelper::createImage(getDevice()->getHandle(), texWidth, texHeight, 1, VK_FORMAT_B8G8R8A8_SRGB,
						  VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
						  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memProperties, textureImage, textureImageMemory);
	/*	*/
	VKHelper::transitionImageLayout(cmd, textureImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	VKHelper::copyBufferToImageCmd(cmd, stagingBuffer, textureImage,
								   {static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight), 1});

	/*	*/
	VKHelper::transitionImageLayout(cmd, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
									VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	/*	*/
	VKHelper::endSingleTimeCommands(getDevice()->getHandle(), queue, cmd, commandPool);

	vkDestroyBuffer(getDevice()->getHandle(), stagingBuffer, nullptr);
	vkFreeMemory(getDevice()->getHandle(), stagingBufferMemory, nullptr);

	free(pixels);

	VKTexture *texture = new VKTexture();
	return texture;
}

void VKRenderInterface::deleteTexture(Texture *texture) { delete texture; }

Sampler *VKRenderInterface::createSampler(SamplerDesc *desc) {

	if (desc == nullptr)
		throw InvalidArgumentException("Invalid sampler description pointer object.");

	/*  */
	VKSampler *sampler = new VKSampler();
	VkSampler vkSampler;
	VKHelper::createSampler(getDevice()->getHandle(), vkSampler);
	// sampler->pdata= samplerObject;
	return sampler;
}

void VKRenderInterface::deleteSampler(Sampler *sampler) {}

RenderPipeline *VKRenderInterface::createRenderPipeline(const RenderPipelineDesc *desc) {

	//    desc.
	return nullptr;
}

void VKRenderInterface::deleteRenderPipeline(RenderPipeline *obj) {}

Shader *VKRenderInterface::createShader(ShaderDesc *desc) {

	/*  Create shader interface object. */
	VKShader *shader = new VKShader();
	// VKShaderObject *shaobj = new VKShaderObject();
	//	shader->pdata = shaobj;
	// shaobj->vulkanCore = vulkanCore;

	VkResult result;
	VkShaderModule vertShaderModule = nullptr;
	VkShaderModule fragShaderModule = nullptr;
	VkShaderModule geomShaderModule = nullptr;
	VkShaderModule tessCShaderModule = nullptr;
	VkShaderModule tessEShaderModule = nullptr;

	/*  */
	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	VkPipelineShaderStageCreateInfo geomShaderStageInfo = {};
	VkPipelineShaderStageCreateInfo tessEShaderStageInfo = {};
	VkPipelineShaderStageCreateInfo tessChaderStageInfo = {};

	/*  */
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

	// Validate the shader language.
	ShaderLanguage supportedLanguage = (ShaderLanguage)(~this->getShaderLanguage());
	if (desc->vertex.language & supportedLanguage)
		throw RuntimeException("Vertex shader language is not supported");
	if (desc->fragment.language & supportedLanguage)
		throw RuntimeException("Fragment shader language is not supported");
	if (desc->geometry.language & supportedLanguage)
		throw RuntimeException("Geometry shader language is not supported");
	if (desc->tessellationControl.language & supportedLanguage)
		throw RuntimeException("Tessellation Control shader language is not supported");
	if (desc->tessellationEvolution.language & supportedLanguage)
		throw RuntimeException("Tessellation Evolution shader language is not supported");
	if (desc->Compute.language & supportedLanguage)
		throw RuntimeException("Compute shader language is not supported");

	/*  Create shader module code containers.   */
	// TODO fix the code for the shader desc.
	//	if(desc->vertex.vertexBinary && desc->vertex.language == SPIRV){
	if (1) {
		// vertShaderModule = createShaderModule(device->getHandle(), (const char*)desc->vertex.vertexBinary,
		// desc->vertex.size);
		// vertShaderModule = createShaderModule(device->getHandle(), (const char *)nullptr, 0); // FIXME

		/*  */
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertShaderModule;
		vertShaderStageInfo.pName = "main";

		shaderStages.push_back(vertShaderStageInfo);
	}
	//	if(desc->fragment.fragmentBinary && desc->fragment.language == SPIRV){
	if (1) {
		// fragShaderModule = createShaderModule(device->getHandle(),  (const char*)desc->fragment.fragmentBinary,
		// desc->fragment.size);
		// fragShaderModule = createShaderModule(device->getHandle(), (const char *)nullptr, 0); // FIXME

		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragShaderModule;
		fragShaderStageInfo.pName = "main";

		shaderStages.push_back(fragShaderStageInfo);
	}
	//	if(desc->geometry.geometryBinary && desc->geometry.language == SPIRV){
	//		geomShaderModule = createShaderModule(device->getHandle(), desc->vertexsource[0],
	// strlen(desc->vertexsource[0]));
	//
	//		geomShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	//		geomShaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
	//		geomShaderStageInfo.module = geomShaderModule;
	//		geomShaderStageInfo.pName = "main";
	//
	//		shaderStages.push_back(geomShaderStageInfo);
	//	}
	//	if(desc->numtesco > 0){
	//		tessCShaderModule = createShaderModule(device->getHandle(), desc->vertexsource[0],
	// strlen(desc->vertexsource[0]));
	//
	//		tessChaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	//		tessChaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	//		tessChaderStageInfo.module = tessCShaderModule;
	//		tessChaderStageInfo.pName = "main";
	//
	//		shaderStages.push_back(tessChaderStageInfo);
	//	}
	//	if(desc->numtesev> 0){
	//		tessEShaderModule = createShaderModule(device->getHandle(), desc->vertexsource[0],
	// strlen(desc->vertexsource[0]));
	//
	//		tessEShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	//		tessEShaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
	//		tessEShaderStageInfo.module = tessEShaderModule;
	//		tessEShaderStageInfo.pName = "main";
	//
	//		shaderStages.push_back(tessEShaderStageInfo);
	//	}

	// If using compute shader.
	if (desc->Compute.language == SPIRV) {

		VkDescriptorSetLayoutBinding bindings[2] = {{0}};
		bindings[0].binding = 0;
		bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		bindings[0].descriptorCount = 1;
		bindings[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
		bindings[1].binding = 1;
		bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		bindings[1].descriptorCount = 1;
		bindings[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

		//        VkComputePipelineCreateInfo info = { VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO };
		//        info.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		//        info.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
		//        info.stage.module = loadShaderModule(device->getHandle(), "shaders/particle.comp.spv");
		//        info.stage.pName = "main";
		//        info.layout = computePipeline.pipelineLayout;

		VkComputePipelineCreateInfo computePipelineCreateInfo = {};
		// result = vkCreateComputePipelines(gpu, nullptr, 1, &computePipelineCreateInfo, nullptr,
		// &shaobj->graphicsPipeline);
	}

	/*  */
	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &samplerLayoutBinding;

	VkDescriptorSetLayout setLayout;
	result = vkCreateDescriptorSetLayout(device->getHandle(), &layoutInfo, nullptr, &setLayout);
	if (result != VK_SUCCESS) {
		throw RuntimeException("Failed to create descriptor set layout - {}", static_cast<int>(result));
	}

	/*  */
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;			  // Optional
	pipelineLayoutInfo.pSetLayouts = &setLayout;	  // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0;	  // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	// result = vkCreatePipelineLayout(device->getHandle(), &pipelineLayoutInfo, nullptr, &shaobj->pipelineLayout);
	// if (result != VK_SUCCESS)
	// 	throw RuntimeException(fmt::format("failed to create pipeline layout - %d!", result));

	VkAttachmentDescription colorAttachment = {};
	// colorAttachment.format = swapChain->swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

	/*  */
	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	/*  */
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkRenderPass renderPass;
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;

	if (vkCreateRenderPass(device->getHandle(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
		throw RuntimeException("failed to create render pass - {}", static_cast<int>(result));
	}

	/*  */
	VkVertexInputBindingDescription bindingDescription = {};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(float) * 3;
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	/**/
	VkVertexInputAttributeDescription attributeDescriptions = {0};
	attributeDescriptions.binding = 0;
	attributeDescriptions.location = 0;
	attributeDescriptions.format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions.offset = 0;

	/*  */
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; // Optional
	vertexInputInfo.vertexAttributeDescriptionCount = 1;
	vertexInputInfo.pVertexAttributeDescriptions = &attributeDescriptions; // Optional

	/*  */
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	/*  */
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;	 // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;			 // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;	 // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;			 // Optional

	/*  */
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.lineWidth = 1.0f;

	/*  */
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	/*  */
	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	/*  Enable dynamic variables in shader configuration.   */
	VkDynamicState dynamicStates[] = {
		VK_DYNAMIC_STATE_VIEWPORT,		  VK_DYNAMIC_STATE_LINE_WIDTH,	 VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
		VK_DYNAMIC_STATE_BLEND_CONSTANTS, VK_DYNAMIC_STATE_DEPTH_BOUNDS,
	};

	/*  */
	VkPipelineDynamicStateCreateInfo dynamicState = {};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = sizeof(dynamicStates) / sizeof(dynamicStates[0]);
	dynamicState.pDynamicStates = dynamicStates;

	/*  */
	VkViewport viewport = {};
	// viewport.x = 0.0f;
	// viewport.y = 0.0f;
	// viewport.width = (float) swapChain->chainExtend.width;
	// viewport.height = (float) swapChain->chainExtend.height;
	// viewport.minDepth = 0.0f;
	// viewport.maxDepth = 1.0f;

	/*  */
	VkRect2D scissor = {};
	// scissor.offset = {0, 0};
	// scissor.extent = swapChain->chainExtend;

	/*  */
	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	/*  Graphic pipeline descriptor.    */
	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = shaderStages.size();
	pipelineInfo.pStages = shaderStages.data();
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr; // Optional
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState; // Optional
	// pipelineInfo.layout = shaobj->pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1;			  // Optional

	/*  Create graphic pipeline.    */
	// result = vkCreateGraphicsPipelines(device->getHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr,
	// 								   &shaobj->graphicsPipeline);
	if (result != VK_SUCCESS) {
		throw RuntimeException("vkCreateGraphicsPipelines failed - {}", static_cast<int>(result));
	}

	/*  Release shader moudles once used.  */
	if (vertShaderModule)
		vkDestroyShaderModule(device->getHandle(), vertShaderModule, nullptr);
	if (fragShaderModule)
		vkDestroyShaderModule(device->getHandle(), fragShaderModule, nullptr);
	if (geomShaderModule)
		vkDestroyShaderModule(device->getHandle(), geomShaderModule, nullptr);
	if (tessCShaderModule)
		vkDestroyShaderModule(device->getHandle(), tessCShaderModule, nullptr);
	if (tessEShaderModule)
		vkDestroyShaderModule(device->getHandle(), tessEShaderModule, nullptr);

	return shader;
}

void VKRenderInterface::deleteShader(Shader *shader) {

	// VKShaderObject *shaobj = (VKShaderObject *)shader->pdata;

	// /*  validate object.   */
	// if (shader->getRenderer() != this)
	// 	throw InvalidArgumentException("");

	// if (shaobj->graphicsPipeline) {
	// 	// vkDestroyPipelineLayout(device->getHandle(), pipelineLayout, nullptr);
	// 	/*  Release all resources.  */
	// 	vkDestroyPipeline(device->getHandle(), shaobj->graphicsPipeline, nullptr);
	// } else
	// 	throw RuntimeException("");

	// //delete shader->pdata;
	// delete shader;
}

Buffer *VKRenderInterface::createBuffer(BufferDesc *desc) {

	VkResult result;
	VKBuffer *vkBufferObject = new VKBuffer();

	// VKHelper::createBuffer(device->getHandle(), desc->size, device->getPhysicalDevice(0)->getMemoryProperties(),
	// 					   (VkBufferUsageFlags)getBufferType((unsigned int)desc->type),
	// 					   VK_MEMORY_PROPERTY_HOST_CACHED_BIT, vkBufferObject->buffer,
	// 					   vkBufferObject->vertexBufferMemory);

	// /*  Buffer description. */
	// VkBufferCreateInfo bufferInfo = {};
	// bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	// bufferInfo.size = desc->size;
	// // bufferInfo.usage = (VkBufferUsageFlags)getBufferType((BufferDesc::BufferType)desc->type);
	// bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	// /*  Create buffer.  */
	// result = vkCreateBuffer(device->getHandle(), &bufferInfo, nullptr, &vkBufferObject->buffer);
	// if (result != VK_SUCCESS) {
	// 	throw RuntimeException(fmt::format("failed to create vertex buffer %d", result));
	// }

	// /*  */
	// VkMemoryRequirements memRequirements;
	// vkGetBufferMemoryRequirements(device->getHandle(), vkBufferObject->buffer, &memRequirements);

	// /*  */
	// VkPhysicalDeviceMemoryProperties memProperties;
	// vkGetPhysicalDeviceMemoryProperties(gpu, &memProperties);

	// /*  Allocate memory.    */
	// VkMemoryAllocateInfo allocInfo = {};
	// allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	// allocInfo.allocationSize = memRequirements.size;
	// // allocInfo.memoryTypeIndex =
	// // 	findMemoryType(physical_devices[0], memRequirements.memoryTypeBits,
	// // 				   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// /*  */
	// if (vkAllocateMemory(device->getHandle(), &allocInfo, nullptr, &vkBufferObject->vertexBufferMemory) !=
	// VK_SUCCESS) { 	throw RuntimeException(fmt::format("failed to allocate vertex buffer memory!"));
	// }

	// /*  */
	// result = vkBindBufferMemory(device->getHandle(), vkBufferObject->buffer, vkBufferObject->vertexBufferMemory, 0);
	// if (result != VK_SUCCESS)
	// 	throw RuntimeException(fmt::format("failed to allocate vertex buffer memory!"));

	return vkBufferObject;
}

void VKRenderInterface::deleteBuffer(Buffer *object) {

	// VKBufferObject *bufferObject = (VKBufferObject *)object->pdata;

	// // vkFreeMemory(device->getHandle(), vertexBufferMemory, nullptr);
	// if (bufferObject->buffer)
	// 	vkDestroyBuffer(device->getHandle(), bufferObject->buffer, nullptr);

	// /*  Release objects.    */
	// //delete object->pdata;
	// delete object;
}

ViewPort *VKRenderInterface::getView(unsigned int i) {

	/*  Validate the index. */
	if (i >= capability.sMaxViewPorts) {
		throw InvalidArgumentException(
			fmt::format("Does not support viewport index {}, max index {}.", i, capability.sMaxViewPorts));
	}

	//	// If the view does not exits. Create it.
	//	if(i == 0)
	//		return glcore->defaultViewport;
	//	return glcore->viewports[i - 1];
	return nullptr;
}

FrameBuffer *VKRenderInterface::createFrameBuffer(FrameBufferDesc *desc) { return nullptr; }

void VKRenderInterface::deleteFrameBuffer(FrameBuffer *obj) {}

QueryObject *VKRenderInterface::createQuery(QueryDesc *desc) { return nullptr; }
void VKRenderInterface::deleteQuery(QueryObject *query) {}

RendererWindow *VKRenderInterface::createWindow(int x, int y, int width, int height) {

	/*	*/
	Ref<VKRenderInterface> rendRef = Ref<VKRenderInterface>(this);
	RendererWindow *window = new VKRenderWindow(rendRef);

	/*	*/
	windows.push_back(window);
	return window;
}

void VKRenderInterface::setCurrentWindow(RendererWindow *window) { /*  */
}

FrameBuffer *VKRenderInterface::getDefaultFramebuffer(void *window) {
	static FrameBuffer *defaultFrambuffer = nullptr;
	/*  TODO add support.   */
	if (defaultFrambuffer == nullptr) {
		// defaultFrambuffer = new
		// FrameBuffer *frameBuffer = new FrameBuffer();
	}

	return defaultFrambuffer;
}

Sync *VKRenderInterface::createSync(SyncDesc *desc) { return nullptr; }
void VKRenderInterface::deleteSync(Sync *sync) {}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
													VkDebugUtilsMessageTypeFlagsEXT messageType,
													const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
													void *pUsinst) {
	return VK_FALSE;
}

void VKRenderInterface::setDebug(bool enable) {

	VkResult result;
	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT =
		(PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(core->getHandle(), "vkCreateDebugReportCallbackEXT");

	if (vkCreateDebugReportCallbackEXT) {
		VkDebugReportCallbackCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
		createInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)debugCallback;
		createInfo.pUserData = nullptr;

		// result = vkCreateDebugReportCallbackEXT(inst, &createInfo, nullptr, &debugReport);
		// if(result != VK_SUCCESS)
		// 	throw RuntimeException(fmt::format("Failed to create debug report callback - %d", result));
	}

	PFN_vkCreateDebugUtilsMessengerEXT CreateDebugUtilsMessengerEXT =
		(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(core->getHandle(), "vkCreateDebugUtilsMessengerEXT");
	if (CreateDebugUtilsMessengerEXT) {
		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
									 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
									 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
								 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
								 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;

		if (CreateDebugUtilsMessengerEXT(core->getHandle(), &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
			throw std::runtime_error("failed to set up debug messenger!");
		} else {
			// return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}
}

void VKRenderInterface::getSupportedTextureCompression(TextureDesc::Compression *pCompressions) {
	if (pCompressions == nullptr)
		throw InvalidArgumentException("pCompressions may not be a null pointer.");

	unsigned int compressions = 0;
	VkPhysicalDeviceFeatures deviceFeatures;

	// vkGetPhysicalDeviceFeatures(getDevice()-> gpu, &deviceFeatures);

	if (deviceFeatures.textureCompressionBC)
		compressions |= (unsigned int)TextureDesc::Compression::NoCompression;
	if (deviceFeatures.textureCompressionETC2)
		compressions |= (unsigned int)TextureDesc::Compression::ETC2;
	if (deviceFeatures.textureCompressionASTC_LDR)
		compressions |= (unsigned int)TextureDesc::Compression::ASTC_LDR;

	// Add support for default compression format.
	if (compressions != 0)
		compressions |= (unsigned int)TextureDesc::Compression::Compression;

	*pCompressions = (TextureDesc::Compression)compressions;
}

void VKRenderInterface::getCapability(Capability *capability) {

	assert(capability);

	if (capability == nullptr)
		throw InvalidArgumentException("Must not be a null pointer.");

	/*  */
	VkPhysicalDeviceProperties properties;

	/*  Propertie set.  */
	VkPhysicalDeviceRayTracingPropertiesNV deviceRayTracingPropertiesNv = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PROPERTIES_NV,
		.pNext = nullptr,
	};
	VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT advancedPropertiesExt = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_PROPERTIES_EXT,
		.pNext = &deviceRayTracingPropertiesNv,
	};
	VkPhysicalDeviceConservativeRasterizationPropertiesEXT conservativeRasterizationPropertiesExt = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONSERVATIVE_RASTERIZATION_PROPERTIES_EXT,
		.pNext = &advancedPropertiesExt,
	};
	VkPhysicalDeviceMaintenance3Properties deviceMaintenance3Properties = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES,
		.pNext = &conservativeRasterizationPropertiesExt,
	};
	VkPhysicalDevicePushDescriptorPropertiesKHR descriptorPropertiesKhr = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES_KHR,
		.pNext = &deviceMaintenance3Properties,
	};
	VkPhysicalDeviceSamplerFilterMinmaxPropertiesEXT deviceSamplerFilterMinmaxPropertiesExt = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_FILTER_MINMAX_PROPERTIES_EXT,
		.pNext = &descriptorPropertiesKhr,
	};
	VkPhysicalDeviceTransformFeedbackPropertiesEXT deviceTransformFeedbackPropertiesExt = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_PROPERTIES_EXT,
		.pNext = &deviceSamplerFilterMinmaxPropertiesExt,
	};
	VkPhysicalDeviceExternalMemoryHostPropertiesEXT deviceExternalMemoryHostPropertiesExt = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_HOST_PROPERTIES_EXT,
		.pNext = &deviceTransformFeedbackPropertiesExt,
		.minImportedHostPointerAlignment = 0};
	VkPhysicalDeviceMultiviewProperties deviceMultiviewProperties = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES,
		.pNext = &deviceExternalMemoryHostPropertiesExt,
	};

	/*  Feature set.    */
	VkPhysicalDeviceMultiviewFeatures deviceMultiviewFeatures = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES,
		.pNext = nullptr,
	};
	VkPhysicalDeviceTransformFeedbackFeaturesEXT deviceTransformFeedbackFeaturesExt = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_FEATURES_EXT,
		.pNext = &deviceMultiviewFeatures,
	};
	VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV deviceRepresentativeFragmentTestFeaturesNv = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_REPRESENTATIVE_FRAGMENT_TEST_FEATURES_NV,
		.pNext = &deviceTransformFeedbackFeaturesExt,
	};
	VkPhysicalDeviceConditionalRenderingFeaturesEXT conditionalRenderingFeaturesExt = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONDITIONAL_RENDERING_FEATURES_EXT,
		.pNext = &deviceRepresentativeFragmentTestFeaturesNv,
	};

	VkPhysicalDeviceProperties2 deviceProperties2 = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2, .pNext = &deviceMultiviewProperties, .properties = {}};
	VkPhysicalDeviceFeatures2 deviceFeatures2 = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
		.pNext = &conditionalRenderingFeaturesExt,
	};
	this->device->getPhysicalDevice(0)->checkFeature<VkPhysicalDeviceConditionalRenderingFeaturesEXT>(
		VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONDITIONAL_RENDERING_FEATURES_EXT, conditionalRenderingFeaturesExt);

	capability->sDepthStencil = true;
	capability->sFramebuffer = true;
	capability->sFrameBufferMSAA = true;
	capability->sCubeMap = true;
	capability->sCubeMapArray = deviceFeatures2.features.imageCubeArray;
	capability->sQueryOcclusion = true;
	capability->sShaderImageLoadStorage = true;
	capability->sShaderStorageBuffer = true;
	capability->sUniformBuffer = true;
	capability->sMapBuffer = true;

	capability->sGeometryShader = deviceFeatures2.features.geometryShader;
	capability->sTessellation = deviceFeatures2.features.tessellationShader;
	capability->sTransformFeedback = deviceTransformFeedbackPropertiesExt.transformFeedbackDraw;
	capability->sIndirectMultiDraw = deviceFeatures2.features.multiDrawIndirect;
	capability->sIndirectDraw = deviceFeatures2.features.drawIndirectFirstInstance;

	capability->sVertexShader = true;
	capability->sFragmentShader = true;
	capability->sShadow = true;
	capability->sInstancing = true;
	capability->sTextureCompression = deviceFeatures2.features.textureCompressionASTC_LDR |
									  deviceFeatures2.features.textureCompressionETC2 |
									  deviceFeatures2.features.textureCompressionBC;
	capability->sVirtualViewPort = deviceFeatures2.features.multiViewport;
	capability->sConditionalRendering = conditionalRenderingFeaturesExt.conditionalRendering;
	// maxDrawIndirectCount

	//	deviceFeatures.samplerAnisotropy;
	//	deviceFeatures.shaderUniformBufferArrayDynamicIndexing;
	// deviceFeatures.shaderStorageImageMultisample;

	// TODO resolve.
	capability->sMaxViewPorts = properties.limits.maxViewports;
	capability->sMaxViewPortDims = properties.limits.maxViewportDimensions[0];
	capability->sViewPortBoundRange[0] = properties.limits.viewportBoundsRange[0];
	capability->sViewPortBoundRange[1] = properties.limits.viewportBoundsRange[1];
	capability->sMaxUniformBlockSize = properties.limits.maxUniformBufferRange;
	capability->sMaxFrameBufferWidth = properties.limits.maxFramebufferWidth;
	capability->sMaxFrameBufferHeight = properties.limits.maxFramebufferHeight;
	capability->sMaxFrameBufferLayers = properties.limits.maxFramebufferLayers;

	capability->lineWidthRange[0] = properties.limits.lineWidthRange[0];
	capability->lineWidthRange[1] = properties.limits.lineWidthRange[1];
	capability->lineWidthGranularity = properties.limits.lineWidthGranularity;
	// properties.limits.strictLines;
	// properties.limits.maxCullDistances;
	// properties.limits.maxTexelOffset;
	// properties.limits.maxUniformBufferRange;
	// properties.limits.maxImageDimension2D;

	/*  Compute shader.*/
	capability->sMaxWorKGroupSize[0] = properties.limits.maxComputeWorkGroupCount[0];
	capability->sMaxWorKGroupSize[1] = properties.limits.maxComputeWorkGroupCount[1];
	capability->sMaxWorKGroupSize[2] = properties.limits.maxComputeWorkGroupCount[2];

	capability->sMaxLocalWorkGroupSize[0] = properties.limits.maxComputeWorkGroupSize[0];
	capability->sMaxLocalWorkGroupSize[1] = properties.limits.maxComputeWorkGroupSize[1];
	capability->sMaxLocalWorkGroupSize[2] = properties.limits.maxComputeWorkGroupSize[2];

	capability->sWorkGroupDimensions = 3;
	capability->sComputeShader = true;

	// properties.limits.maxColorAttachments;
	// properties.
	capability->sMaxAnisotropy = properties.limits.maxSamplerAnisotropy;
	capability->rayTracing = deviceRayTracingPropertiesNv.maxTriangleCount > 0;
}

void VKRenderInterface::getFeatures(Features *features) {

	assert(features);

	if (features == nullptr)
		throw InvalidArgumentException("Must not be a null pointer.");
}

const char *VKRenderInterface::getShaderVersion(ShaderLanguage language) const {
	static char shaderversion[64];

	return "";
}

ShaderLanguage VKRenderInterface::getShaderLanguage() const { return languageSupport; }

const char *VKRenderInterface::getAPIVersion() const {
	const VkPhysicalDeviceProperties &devicePropertie = this->device->getPhysicalDevice(0)->getProperties();
	/*	Get API version.	*/
	static char apiversion[64];
	sprintf(apiversion, "%d.%d.%d", VK_VERSION_MAJOR(devicePropertie.apiVersion),
			VK_VERSION_MINOR(devicePropertie.apiVersion), VK_VERSION_PATCH(devicePropertie.apiVersion));
	return (const char *)apiversion;
}

const char *VKRenderInterface::getVersion() const {
	return FV_STR_VERSION(RENDER_VULKAN_MAJOR, RENDER_VULKAN_MINOR, RENDER_VULKAN_PATCH);
}

void VKRenderInterface::getStatus(MemoryInfo *memoryInfo) {}

CommandList *VKRenderInterface::createCommandBuffer() {
	Ref<VKRenderInterface> ref = Ref<VKRenderInterface>(this);

	return new VKCommandList(ref);
}

void VKRenderInterface::submittCommand(Ref<CommandList> &list) {

	VKCommandList *commandBuffer = static_cast<VKCommandList *>(*list);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer->cmdBuffer;

	VKS_VALIDATE(vkQueueSubmit(this->queue, 1, &submitInfo, VK_NULL_HANDLE));
}

void VKRenderInterface::execute(CommandList *list) {}

void *VKRenderInterface::getData() const { return nullptr; }

extern "C" IRenderer *createInternalRenderer(IConfig *config) { return new VKRenderInterface(config); }