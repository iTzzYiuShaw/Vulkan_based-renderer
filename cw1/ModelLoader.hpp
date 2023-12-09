
#include <string>
#include <vector>

#include <cstdint>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


#include "../labutils/vulkan_context.hpp"

#include "../labutils/vkbuffer.hpp"
#include "../labutils/allocator.hpp" 
#include "simple_model.hpp"
#include "load_model_obj.hpp"


struct TexturedMesh
{
	labutils::Buffer positions;
	labutils::Buffer texcoords;
	labutils::Buffer diffuseColor;

	std::uint32_t vertexCount;
	std::string texturePath;
	

	//// Default constructor
	TexturedMesh(labutils::Buffer pPosition, labutils::Buffer pCoordinate, labutils::Buffer pDiffuseColor, std::uint32_t pvertexCount, std::string ptexturePath, glm::vec3 Acolor) :
		positions(std::move(pPosition)), texcoords(std::move(pCoordinate)), vertexCount(std::move(pvertexCount)), texturePath(std::move(ptexturePath)), diffuseColor(std::move(pDiffuseColor))
	{

	};

	//TexturedMesh(const TexturedMesh&) = delete;
	//TexturedMesh& operator=(const TexturedMesh&) = delete;

	TexturedMesh(TexturedMesh&& other) noexcept
		: positions(std::move(other.positions))
		, texcoords(std::move(other.texcoords))
		, vertexCount(other.vertexCount)
		, texturePath(std::move(other.texturePath))
		, diffuseColor(std::move(other.diffuseColor))

	{

	}
};

struct UnTexturedMesh
{
	labutils::Buffer positions;
	labutils::Buffer colors;
	labutils::Buffer constant;
	std::uint32_t vertexCount;


	// Default constructor
	UnTexturedMesh(labutils::Buffer pPosition, labutils::Buffer pColor, labutils::Buffer pCons, std::uint32_t pvertexCount) :
		positions(std::move(pPosition)), colors(std::move(pColor)),constant(std::move(pCons)), vertexCount(std::move(pvertexCount))
	{

	};

	UnTexturedMesh(UnTexturedMesh&& other) noexcept
		: positions(std::move(other.positions))
		, colors(std::move(other.colors))
		, vertexCount(other.vertexCount)
		, constant(std::move(other.constant))
	{}

};

TexturedMesh create_texture_mesh(labutils::VulkanContext const&, labutils::Allocator const&,
	SimpleMeshInfo meshInfo,SimpleMaterialInfo materialInfo, SimpleModel model);

UnTexturedMesh create_unTextured_mesh(labutils::VulkanContext const&, labutils::Allocator const&,
	SimpleMeshInfo meshInfo, SimpleMaterialInfo materialInfo, SimpleModel model);