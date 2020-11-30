#include "mesh.h"

Mesh::Mesh(const std::string &filename) {
	// Load a mesh from a file (assuming this is a .off file), and create a bvh
	load_off(filename, vertices, facets);
	bvh = AABBTree(vertices, facets);
}

std::vector<VertexAttributes> get_mesh_vertices(const Scene &scene) {
	std::vector<VertexAttributes> vertices;
	for (auto &mesh : scene.meshes) {
		for (int i = 0; i < mesh->facets.rows(); i++) {
			Eigen::VectorXi facet = mesh->facets.row(i);
			for (int j = 0; j < facet.size(); j++) {
				Eigen::VectorXd vertex = mesh->vertices.row(facet[j]);
				vertices.push_back(VertexAttributes(vertex[0], vertex[1], vertex[2]));
			}
		}
	}

	return vertices;
}