#include "ProceduralGeometry.h"
#include "Core/Math3D.h"
#include <generator/BoxMesh.hpp>
#include <generator/CircleShape.hpp>
#include <generator/HelixPath.hpp>
#include <generator/MeshVertex.hpp>
#include <generator/PlaneMesh.hpp>
#include <generator/SphereMesh.hpp>
#include <generator/TeapotMesh.hpp>
#include <generator/TorusMesh.hpp>

using namespace fragcore;
using namespace generator;

static inline glm::vec3 tangent(const glm::vec3 &normal) noexcept {

	glm::vec3 tangent = glm::cross(normal, glm::vec3(0.0f, 1.0f, 0.0f));

	if (tangent.length() <= Math::Epsilon) {
		tangent = glm::cross(normal, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	return tangent;
}

void ProceduralGeometry::generatePlan(float scale, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices,
									  int segmentsX, int segmentsY) {

	PlaneMesh mesh({1.0 * scale, 1.0 * scale}, glm::ivec2(segmentsX, segmentsY));

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

void ProceduralGeometry::createFrustum(std::vector<Vertex> &vertices, const Matrix4x4 &projection) {

	BoxMesh BoxMesh({1, 1, 1}, {1, 1, 1});

	// std::vector<Vector3> vertices;

	auto mesh_vertices = BoxMesh.vertices();

	while (!mesh_vertices.done()) {
		MeshVertex vertex = mesh_vertices.generate();

		Vector4 projectedVertex =
			(projection.inverse() * Vector4(vertex.position.x, vertex.position.y, vertex.position.z, 1.0f));

		projectedVertex *= (1.0f / projectedVertex.w());

		ProceduralGeometry::Vertex frustum_vertex;
		// frustum_vertex.vertex = projectedVertex.head<3>();
		vertices.emplace_back(frustum_vertex);

		mesh_vertices.next();
	}
}

void ProceduralGeometry::createFrustum(std::vector<Vertex> &vertices, const float fov, const float aspect,
									   const float near, const float far) {

	Matrix4x4 projectMatrix;
	const float theta = fov * 0.5f;
	const float range = far - near;
	const float invtan = 1. / std::tan(theta);

	projectMatrix(0, 0) = invtan / aspect;
	projectMatrix(1, 1) = invtan;
	projectMatrix(2, 2) = -(near + far) / range;
	projectMatrix(3, 2) = -1;
	projectMatrix(2, 3) = -2 * near * far / range;
	projectMatrix(3, 3) = 0;

	return ProceduralGeometry::createFrustum(vertices, projectMatrix);
}

void ProceduralGeometry::generateSphere(float radius, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices,
										int slices, int segments) {

	SphereMesh SphereMesh(radius, slices, segments);

	auto mesh_vertices = SphereMesh.vertices();

	while (!mesh_vertices.done()) {
		const MeshVertex vertex = mesh_vertices.generate();
		// Do something with vertex

		Vertex ver;
		ver.vertex[0] = vertex.position.x * radius;
		ver.vertex[1] = vertex.position.y * radius;
		ver.vertex[2] = vertex.position.z * radius;

		ver.normal[0] = vertex.normal.x;
		ver.normal[1] = vertex.normal.y;
		ver.normal[2] = vertex.normal.z;

		const glm::vec3 tangent = ::tangent(vertex.normal);

		ver.tangent[0] = tangent.x;
		ver.tangent[1] = tangent.y;
		ver.tangent[2] = tangent.z;

		ver.uv[0] = vertex.texCoord.x;
		ver.uv[1] = vertex.texCoord.y;

		vertices.push_back(ver);

		mesh_vertices.next();
	}

	/*	*/
	auto mesh_triangles = SphereMesh.triangles();

	while (!mesh_triangles.done()) {
		const generator::Triangle triangle = mesh_triangles.generate();
		// Do something with vertex

		indices.push_back(triangle.vertices.x);
		indices.push_back(triangle.vertices.y);
		indices.push_back(triangle.vertices.z);

		mesh_triangles.next();
	}
}

void ProceduralGeometry::generateCube(const float scale, std::vector<Vertex> &vertices,
									  std::vector<unsigned int> &indices) {

	BoxMesh BoxMesh;

	auto mesh_vertices = BoxMesh.vertices();

	while (!mesh_vertices.done()) {
		MeshVertex vertex = mesh_vertices.generate();
		// Do something with vertex

		Vertex v;
		v.vertex[0] = vertex.position.x * scale;
		v.vertex[1] = vertex.position.y * scale;
		v.vertex[2] = vertex.position.z * scale;

		v.normal[0] = vertex.normal.x;
		v.normal[1] = vertex.normal.y;
		v.normal[2] = vertex.normal.z;

		const glm::vec3 tangent = ::tangent(vertex.normal);

		v.tangent[0] = tangent.x;
		v.tangent[1] = tangent.y;
		v.tangent[2] = tangent.z;

		v.uv[0] = vertex.texCoord.x;
		v.uv[1] = vertex.texCoord.y;

		vertices.push_back(v);

		mesh_vertices.next();
	}

	auto mesh_triangles = BoxMesh.triangles();
	while (!mesh_triangles.done()) {
		generator::Triangle triangle = mesh_triangles.generate();
		// Do something with vertex

		indices.push_back(triangle.vertices.x);
		indices.push_back(triangle.vertices.y);
		indices.push_back(triangle.vertices.z);

		mesh_triangles.next();
	}
}

void ProceduralGeometry::generateWireCube(const float scale, std::vector<Vertex> &vertices,
										  [[maybe_unused]] std::vector<unsigned int> &indices) {
	BoxMesh BoxMesh({1, 1, 1}, {1, 1, 1});

	auto mesh_vertices = BoxMesh.vertices();

	while (!mesh_vertices.done()) {
		MeshVertex vertex = mesh_vertices.generate();

		Vertex v;
		v.vertex[0] = vertex.position.x * scale;
		v.vertex[1] = vertex.position.y * scale;
		v.vertex[2] = vertex.position.z * scale;

		vertices.push_back(v);

		mesh_vertices.next();
	}
}

void ProceduralGeometry::generateTorus(float scale, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices) {
	TorusMesh SphereMesh;

	auto mesh_vertices = SphereMesh.vertices();

	while (!mesh_vertices.done()) {
		MeshVertex vertex = mesh_vertices.generate();
		// Do something with vertex

		Vertex v;
		v.vertex[0] = vertex.position.x * scale;
		v.vertex[1] = vertex.position.y * scale;
		v.vertex[2] = vertex.position.z * scale;

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

	auto mesh_triangles = SphereMesh.triangles();
	while (!mesh_triangles.done()) {
		generator::Triangle triangle = mesh_triangles.generate();
		// Do something with vertex

		indices.push_back(triangle.vertices.x);
		indices.push_back(triangle.vertices.y);
		indices.push_back(triangle.vertices.z);

		mesh_triangles.next();
	}
}