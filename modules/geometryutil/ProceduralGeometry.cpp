#include "ProceduralGeometry.h"

#include <generator/CircleShape.hpp>
#include <generator/HelixPath.hpp>
#include <generator/MeshVertex.hpp>
#include <generator/PlaneMesh.hpp>
#include <generator/SphereMesh.hpp>

using namespace fragcore;
using namespace generator;

static inline glm::vec3 tangent(const glm::vec3 &normal) noexcept {

	glm::vec3 tangent = glm::cross(normal, glm::vec3(0.0f, 1.0f, 0.0f));

	if (tangent.length() <= Math::Epsilon) {
		tangent = glm::cross(normal, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	return tangent;
}

void ProceduralGeometry::generatePlan(float scale, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices) {

	PlaneMesh mesh;

	auto mesh_vertices = mesh.vertices();

	while (!mesh_vertices.done()) {
		MeshVertex vertex = mesh_vertices.generate();
		// Do something with vertex

		Vertex v;
		v.vertex[0] = vertex.position.x;
		v.vertex[1] = vertex.position.y;
		v.vertex[2] = vertex.position.z;

		v.normal[0] = vertex.normal.x;
		v.normal[1] = vertex.normal.y;
		v.normal[2] = vertex.normal.z;

		glm::vec3 tangent = ::tangent(vertex.normal);

		v.tangent[0] = tangent.x;
		v.tangent[1] = tangent.y;
		v.tangent[2] = tangent.z;

		v.uv[0] = vertex.texCoord.x;
		v.uv[1] = vertex.texCoord.y;

		vertices.push_back(v);

		mesh_vertices.next();
	}

	auto mesh_triangles = mesh.triangles();
	while (!mesh_triangles.done()) {
		generator::Triangle triangle = mesh_triangles.generate();
		// Do something with vertex

		indices.push_back(triangle.vertices.x);
		indices.push_back(triangle.vertices.y);
		indices.push_back(triangle.vertices.z);

		mesh_triangles.next();
	}
}

void ProceduralGeometry::generateCube(float scale, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices) {

	const std::vector<Vertex> vertice = {
		{-1.0f, -1.0f, -1.0f, 0, 0},							 // triangle 1 : begin
		{-1.0f, -1.0f, 1.0f, 0, 1},	 {-1.0f, 1.0f, 1.0f, 1, 1},	 // triangle 1 : end
		{1.0f, 1.0f, -1.0f, 1, 1},								 // triangle 2 : begin
		{-1.0f, -1.0f, -1.0f, 1, 0}, {-1.0f, 1.0f, -1.0f, 0, 0}, // triangle 2 : end
		{1.0f, -1.0f, 1.0f, 0, 0},	 {-1.0f, -1.0f, -1.0f, 0, 1}, {1.0f, -1.0f, -1.0f, 1, 1},
		{1.0f, 1.0f, -1.0f, 0, 0},	 {1.0f, -1.0f, -1.0f, 1, 1},  {-1.0f, -1.0f, -1.0f, 1, 0},
		{-1.0f, -1.0f, -1.0f, 0, 0}, {-1.0f, 1.0f, 1.0f, 0, 1},	  {-1.0f, 1.0f, -1.0f, 1, 1},
		{1.0f, -1.0f, 1.0f, 0, 0},	 {-1.0f, -1.0f, 1.0f, 1, 1},  {-1.0f, -1.0f, -1.0f, 0, 1},
		{-1.0f, 1.0f, 1.0f, 0, 0},	 {-1.0f, -1.0f, 1.0f, 0, 1},  {1.0f, -1.0f, 1.0f, 1, 1},
		{1.0f, 1.0f, 1.0f, 0, 0},	 {1.0f, -1.0f, -1.0f, 1, 1},  {1.0f, 1.0f, -1.0f, 1, 0},
		{1.0f, -1.0f, -1.0f, 0, 0},	 {1.0f, 1.0f, 1.0f, 0, 1},	  {1.0f, -1.0f, 1.0f, 1, 1},
		{1.0f, 1.0f, 1.0f, 0, 0},	 {1.0f, 1.0f, -1.0f, 1, 1},	  {-1.0f, 1.0f, -1.0f, 0, 1},
		{1.0f, 1.0f, 1.0f, 0, 0},	 {-1.0f, 1.0f, -1.0f, 0, 1},  {-1.0f, 1.0f, 1.0f, 1, 1},
		{1.0f, 1.0f, 1.0f, 0, 0},	 {-1.0f, 1.0f, 1.0f, 1, 1},	  {1.0f, -1.0f, 1.0f, 1, 0}};

	vertices = vertice;
}