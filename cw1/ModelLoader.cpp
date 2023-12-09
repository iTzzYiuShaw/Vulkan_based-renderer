
#include "ModelLoader.hpp"

#include <limits>

#include <cstring> // for std::memcpy()

#include "../labutils/error.hpp"
#include "../labutils/vkutil.hpp"
#include "../labutils/to_string.hpp"
namespace lut = labutils;


TexturedMesh create_texture_mesh(labutils::VulkanContext const& aContext, labutils::Allocator const& aAllocator, 
	SimpleMeshInfo meshInfo, SimpleMaterialInfo materialInfo,
	SimpleModel model)
{

	std::uint32_t vertexStart = meshInfo.vertexStartIndex;
	std::uint32_t verticeCount = meshInfo.vertexCount;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> texcoord;
	std::vector<glm::vec3> diffuse;
	for (int i = vertexStart; i < verticeCount + vertexStart; i++)
	{
		glm::vec3 coor = model.dataTextured.positions[i];
		positions.push_back(coor);


		glm::vec2 tCoor = model.dataTextured.texcoords[i];
		texcoord.push_back(glm::vec3(tCoor.x,tCoor.y,-2));

		glm::vec3 diffuseColor = materialInfo.diffuseColor;
		diffuse.push_back(diffuseColor);
	}

	
	//static float const positions[] = {
	//-1.f, 0.f, -6.f, // v0 
	//-1.f, 0.f, +6.f, // v1 
	//+1.f, 0.f, +6.f, // v2 

	//-1.f, 0.f, -6.f, // v0 
	//+1.f, 0.f, +6.f, // v2 
	//+1.f, 0.f, -6.f  // v3 
	//};

	//static float const texcoord[] = {
	//	0.f, -6.f,0.0f, // t0
	//	0.f, +6.f,0.0f,// t1 
	//	1.f, +6.f,0.0f, // t2 

	//	0.f, -6.f,0.0f, // t0 
	//	1.f, +6.f, 0.0f,// t2 
	//	1.f, -6.f,0.0f, // t3
	//};

	//positions.push_back(glm::vec3(-1.f,0.f,-6.f));
	//positions.push_back(glm::vec3(-1.f, 0.f, 6.f));
	//positions.push_back(glm::vec3(1.f, 0.f, 6.f));

	//positions.push_back(glm::vec3(-1.f, 0.f, -6.f));
	//positions.push_back(glm::vec3(1.f, 0.f, 6.f));
	//positions.push_back(glm::vec3(1.f, 0.f, -6.f));


	//texcoord.push_back(glm::vec3(0.f, -6.f, 0.f));
	//texcoord.push_back(glm::vec3(0.f, 6.f, 0.f));
	//texcoord.push_back(glm::vec3(1.f, 6.f, 0.f));

	//texcoord.push_back(glm::vec3(0.f, -6.f, 0.f));
	//texcoord.push_back(glm::vec3(1.f, 6.f, 0.f));
	//texcoord.push_back(glm::vec3(1.f, -6.f, 0.f));


	//std::uint32_t vertexStart = 0;
	//std::uint32_t verticeCount = 6;



	// Create final position and color buffers
	lut::Buffer vertexPosGPU = lut::create_buffer(
		aAllocator,
		 positions.size() * sizeof(glm::vec3),
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VMA_MEMORY_USAGE_GPU_ONLY
	);
	lut::Buffer vertexColGPU = lut::create_buffer(
		aAllocator,
		 texcoord.size()*sizeof(glm::vec3) ,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VMA_MEMORY_USAGE_GPU_ONLY
	);

	lut::Buffer vertexDifGPU = lut::create_buffer(
		aAllocator,
		diffuse.size() * sizeof(glm::vec3),
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VMA_MEMORY_USAGE_GPU_ONLY
	);



	lut::Buffer posStaging = lut::create_buffer(
		aAllocator,
		positions.size() * sizeof(glm::vec3),
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VMA_MEMORY_USAGE_CPU_TO_GPU
	);

	lut::Buffer colStaging = lut::create_buffer(
		aAllocator,
		texcoord.size() * sizeof(glm::vec3),
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VMA_MEMORY_USAGE_CPU_TO_GPU
	);

	lut::Buffer difStaging = lut::create_buffer(
		aAllocator,
		diffuse.size() * sizeof(glm::vec3),
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VMA_MEMORY_USAGE_CPU_TO_GPU
	);




	void* posPtr = nullptr;
	if (auto const res = vmaMapMemory(aAllocator.allocator, posStaging.allocation, &posPtr); VK_SUCCESS != res)
	{
		throw lut::Error("Mapping memory for writing\n"
			"vmaMapMemory() returned %s", lut::to_string(res).c_str());
	}
	std::memcpy(posPtr, positions.data(), positions.size()*sizeof(glm::vec3));
	vmaUnmapMemory(aAllocator.allocator, posStaging.allocation);

	void* colPtr = nullptr;
	if (auto const res = vmaMapMemory(aAllocator.allocator, colStaging.allocation, &colPtr); VK_SUCCESS != res)
	{
		throw lut::Error("Mapping memory for writing\n"
			"vmaMapMemory() returned %s", lut::to_string(res).c_str());
	}
	std::memcpy(colPtr, texcoord.data(), texcoord.size() * sizeof(glm::vec3));
	vmaUnmapMemory(aAllocator.allocator, colStaging.allocation);


	void* difPtr = nullptr;
	if (auto const res = vmaMapMemory(aAllocator.allocator, difStaging.allocation, &difPtr); VK_SUCCESS != res)
	{
		throw lut::Error("Mapping memory for writing\n"
			"vmaMapMemory() returned %s", lut::to_string(res).c_str());
	}
	std::memcpy(difPtr, diffuse.data(), diffuse.size() * sizeof(glm::vec3));
	vmaUnmapMemory(aAllocator.allocator, difStaging.allocation);

	// We need to ensure that the Vulkan resources are alive until all the
	// transfers have completed. For simplicity, we will just wait for the
	// operations to complete with a fence. A more complex solution might want
	// to queue transfers, let these take place in the background while
	// performing other tasks.
	lut::Fence uploadComplete = create_fence(aContext);
	// Queue data uploads from staging buffers to the final buffers
	// This uses a separate command pool for simplicity.
	lut::CommandPool uploadPool = create_command_pool(aContext);
	VkCommandBuffer uploadCmd = alloc_command_buffer(aContext, uploadPool.handle);
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;


	if (auto const res = vkBeginCommandBuffer(uploadCmd, &beginInfo); VK_SUCCESS != res)
	{
		throw lut::Error("Beginning command buffer recording\n"
			"vkBeginCommandBuffer() returned %s", lut::to_string(res).c_str());
	}
	VkBufferCopy pcopy{};
	pcopy.size = positions.size() * sizeof(glm::vec3);
	vkCmdCopyBuffer(uploadCmd, posStaging.buffer, vertexPosGPU.buffer, 1, &pcopy);
	lut::buffer_barrier(uploadCmd,
		vertexPosGPU.buffer,
		VK_ACCESS_TRANSFER_WRITE_BIT,
		VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
	);


	VkBufferCopy ccopy{};
	ccopy.size = texcoord.size() * sizeof(glm::vec3);
	vkCmdCopyBuffer(uploadCmd, colStaging.buffer, vertexColGPU.buffer, 1, &ccopy);
	lut::buffer_barrier(uploadCmd,
		vertexColGPU.buffer,
		VK_ACCESS_TRANSFER_WRITE_BIT,
		VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
	);

	VkBufferCopy dcopy{};
	dcopy.size = diffuse.size() * sizeof(glm::vec3);
	vkCmdCopyBuffer(uploadCmd, difStaging.buffer, vertexDifGPU.buffer, 1, &dcopy);
	lut::buffer_barrier(uploadCmd,
		vertexDifGPU.buffer,
		VK_ACCESS_TRANSFER_WRITE_BIT,
		VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
	);


	if (auto const res = vkEndCommandBuffer(uploadCmd); VK_SUCCESS != res)
	{
		throw lut::Error("Ending command buffer recording\n"
			"vkEndCommandBuffer() returned %s", lut::to_string(res).c_str());
	}

	// Submit transfer commands
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &uploadCmd;
	if (auto const res = vkQueueSubmit(aContext.graphicsQueue, 1, &submitInfo, uploadComplete.handle); VK_SUCCESS != res)
	{
		throw lut::Error("Submitting commands\n"
			"vkQueueSubmit() returned %s", lut::to_string(res).c_str());
	}
	// Wait for commands to finish before we destroy the temporary resources
	// required for the transfers (staging buffers, command pool, ...)
	//
	// The code doesn’t destory the resources implicitly – the resources are
	// destroyed by the destructors of the labutils wrappers for the various
	// objects once we leave the function’s scope.
	if (auto const res = vkWaitForFences(aContext.device, 1, &uploadComplete.handle, VK_TRUE, std::numeric_limits<std::uint64_t>::max()); VK_SUCCESS != res)
	{
		throw lut::Error("Waiting for upload to complete\n"
			"vkWaitForFences() returned %s", lut::to_string(res).c_str());
	}


	return TexturedMesh{
		std::move(vertexPosGPU),
		std::move(vertexColGPU),
		std::move(vertexDifGPU),
		verticeCount,
		materialInfo.diffuseTexturePath,
		materialInfo.diffuseColor// three floats per position
	};
}

UnTexturedMesh create_unTextured_mesh(labutils::VulkanContext const& aContext, labutils::Allocator const& aAllocator,
	SimpleMeshInfo meshInfo, SimpleMaterialInfo materialInfo, SimpleModel model)
{
	// Vertex data
	std::uint32_t vertexStart = meshInfo.vertexStartIndex;
	std::uint32_t verticeCount = meshInfo.vertexCount;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> constant;
	for (int i = vertexStart; i < verticeCount + vertexStart; i++)
	{
		glm::vec3 coor = model.dataUntextured.positions[i];
		positions.push_back(coor);


		glm::vec3 color = materialInfo.diffuseColor;
		colors.push_back(color);


		glm::vec3 constantC = glm::vec3(-1.f, -1.f, -1.f);
		constant.push_back(constantC);
	}

	lut::Buffer vertexPosGPU = lut::create_buffer(
		aAllocator,
		positions.size() * sizeof(glm::vec3),
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VMA_MEMORY_USAGE_GPU_ONLY
	);

	lut::Buffer vertexColGPU = lut::create_buffer(
		aAllocator,
		colors.size() * sizeof(glm::vec3),
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VMA_MEMORY_USAGE_GPU_ONLY
	);

	lut::Buffer vertexConsGPU = lut::create_buffer(
		aAllocator,
		constant.size() * sizeof(glm::vec3),
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VMA_MEMORY_USAGE_GPU_ONLY
	);


	lut::Buffer posStaging = lut::create_buffer(
		aAllocator,
		positions.size() * sizeof(glm::vec3),
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VMA_MEMORY_USAGE_CPU_TO_GPU
	);

	lut::Buffer colStaging = lut::create_buffer(
		aAllocator,
		colors.size() * sizeof(glm::vec3),
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VMA_MEMORY_USAGE_CPU_TO_GPU);

	lut::Buffer consStaging = lut::create_buffer(
		aAllocator,
		constant.size() * sizeof(glm::vec3),
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VMA_MEMORY_USAGE_CPU_TO_GPU);

	/*The staging buffers are CPU/host-visible.
	We can fill them by mapping the buffers to retrieve a normal C/C++
	pointer to their contents, copying the data to this pointer
	and then unmapping the buffers again*/

	void* posPtr = nullptr;
	if (auto const res = vmaMapMemory(aAllocator.allocator, posStaging.allocation, &posPtr); VK_SUCCESS != res)
	{

		throw lut::Error("Mapping memory for writing\n" "vmaMapMemory() returned %s", lut::to_string(res).c_str());

	}
	std::memcpy(posPtr, positions.data(), positions.size() * sizeof(glm::vec3));

	vmaUnmapMemory(aAllocator.allocator, posStaging.allocation);


	void* colPtr = nullptr;
	if (auto const res = vmaMapMemory(aAllocator.allocator, colStaging.allocation, &colPtr); VK_SUCCESS != res)
	{

		throw lut::Error("Mapping memory for writing\n" "vmaMapMemory() returned %s", lut::to_string(res).c_str());

	}
	std::memcpy(colPtr, colors.data(), colors.size() * sizeof(glm::vec3));
	vmaUnmapMemory(aAllocator.allocator, colStaging.allocation);


	void* consPtr = nullptr;
	if (auto const res = vmaMapMemory(aAllocator.allocator, consStaging.allocation, &consPtr); VK_SUCCESS != res)
	{

		throw lut::Error("Mapping memory for writing\n" "vmaMapMemory() returned %s", lut::to_string(res).c_str());

	}
	std::memcpy(consPtr, constant.data(), constant.size() * sizeof(glm::vec3));
	vmaUnmapMemory(aAllocator.allocator, consStaging.allocation);

	// We need to ensure that the Vulkan resources are alive until all the 
	// transfers have completed. For simplicity, we will just wait for the 
	// operations to complete with a fence. A more complex solution might want 
	// to queue transfers, let these take place in the background while 
	// performing other tasks. 
	lut::Fence uploadComplete = create_fence(aContext);

	// Queue data uploads from staging buffers to the final buffers 8
	// This uses a separate command pool for simplicity. 9
	lut::CommandPool uploadPool = create_command_pool(aContext);
	VkCommandBuffer uploadCmd = alloc_command_buffer(aContext, uploadPool.handle);


	//We then record the copy commands into the command buffer
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	if (auto const res = vkBeginCommandBuffer(uploadCmd, &beginInfo); VK_SUCCESS != res)
	{

		throw lut::Error("Beginning command buffer recording\n" "vkBeginCommandBuffer() returned %s", lut::to_string(res).c_str());

	}

	VkBufferCopy pcopy{};
	pcopy.size = positions.size() * sizeof(glm::vec3);

	vkCmdCopyBuffer(uploadCmd, posStaging.buffer, vertexPosGPU.buffer, 1, &pcopy);

	lut::buffer_barrier(uploadCmd, vertexPosGPU.buffer, VK_ACCESS_TRANSFER_WRITE_BIT,
		VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
	);


	VkBufferCopy ccopy{};
	ccopy.size = colors.size() * sizeof(glm::vec3);

	vkCmdCopyBuffer(uploadCmd, colStaging.buffer, vertexColGPU.buffer, 1, &ccopy);

	lut::buffer_barrier(uploadCmd,
		vertexColGPU.buffer,
		VK_ACCESS_TRANSFER_WRITE_BIT,
		VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_VERTEX_INPUT_BIT);


	VkBufferCopy conscopy{};
	conscopy.size = constant.size() * sizeof(glm::vec3);

	vkCmdCopyBuffer(uploadCmd, consStaging.buffer, vertexConsGPU.buffer, 1, &conscopy);

	lut::buffer_barrier(uploadCmd,
		vertexConsGPU.buffer,
		VK_ACCESS_TRANSFER_WRITE_BIT,
		VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_VERTEX_INPUT_BIT);


	if (auto const res = vkEndCommandBuffer(uploadCmd); VK_SUCCESS != res)
	{
		throw lut::Error("Ending command buffer recording\n" "vkEndCommandBuffer() returned %s", lut::to_string(res).c_str());
	}


	// Submit transfer commands 
	//Once recorded, we submit the command buffer for execution, and wait for the commands to finish
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &uploadCmd;

	if (auto const res = vkQueueSubmit(aContext.graphicsQueue, 1, &submitInfo, uploadComplete.handle); VK_SUCCESS != res)
	{

		throw lut::Error("Submitting commands\n""vkQueueSubmit() returned %s", lut::to_string(res).c_str());

	}

	// Wait for commands to finish before we destroy the temporary resources 
	// required for the transfers (staging buffers, command pool, ...) 
	// 
	// The code doesn’t destory the resources implicitly – the resources are
	// destroyed by the destructors of the labutils wrappers for the various 
	// objects once we leave the function’s scope. 
	if (auto const res = vkWaitForFences(aContext.device, 1, &uploadComplete.handle, VK_TRUE, std::numeric_limits<std::uint64_t>::max()); VK_SUCCESS != res)
	{
		throw lut::Error("Waiting for upload to complete\n" "vkWaitForFences() returned %s", lut::to_string(res).c_str());

	}


	return UnTexturedMesh{
		std::move(vertexPosGPU),
		std::move(vertexColGPU),
		std::move(vertexConsGPU),
		verticeCount // two floats per position 
	};
}
