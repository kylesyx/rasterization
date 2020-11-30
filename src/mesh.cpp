#include "mesh.h"

Mesh::Mesh(const std::string &filename) {
	// Load a mesh from a file (assuming this is a .off file), and create a bvh
	load_off(filename, vertices, facets);
	bvh = AABBTree(vertices, facets);
}

std::vector<VertexAttributes> Mesh::get_triangles_vertices() {
	std::vector<VertexAttributes> vertices;
	for (int i = 0; i < this->facets.rows(); i++) {
		// v1 v2 v3
		Eigen::Vector3i facet = this->facets.row(i);
		// Compute normal per triangle
		Eigen::Vector3d v1 = this->vertices.row(facet[0]);
		Eigen::Vector3d v2 = this->vertices.row(facet[1]);
		Eigen::Vector3d v3 = this->vertices.row(facet[2]);
		Eigen::Vector3d normal = (v2 - v1).cross(v3 - v1).normalized();
		for (int j = 0; j < facet.size(); j++) {
			Eigen::Vector3d vertex = this->vertices.row(facet[j]);
			vertices.push_back(VertexAttributes(vertex[0], vertex[1], vertex[2], 1, normal[0], normal[1], normal[2], 1));
		}
	}
	return vertices;
}

std::vector<VertexAttributes> Mesh::get_lines_vertices() {
	std::vector<VertexAttributes> vertices;
	for (int i = 0; i < this->facets.rows(); i++) {
		// v1 v2 v3
		Eigen::VectorXi facet = this->facets.row(i);
		for (int j = 0; j < facet.size(); j++) {
			// two endpoints of the edge
			Eigen::VectorXd vertex1 = this->vertices.row(facet[j % facet.size()]);
			Eigen::VectorXd vertex2 = this->vertices.row(facet[(j + 1) % facet.size()]);
			vertices.push_back(VertexAttributes(vertex1[0], vertex1[1], vertex1[2]));
			vertices.push_back(VertexAttributes(vertex2[0], vertex2[1], vertex2[2]));
		}
	}
	return vertices;
}

std::vector<VertexAttributes> get_meshes_vertices(const Scene &scene, const std::string &type) {
	std::vector<VertexAttributes> vertices;
	for (auto &mesh : scene.meshes) {
		std::vector<VertexAttributes> mesh_vertices = type == "lines" ? mesh->get_lines_vertices() : mesh->get_triangles_vertices();
		vertices.insert(vertices.end(), mesh_vertices.begin(), mesh_vertices.end());
	}

	return vertices;
}