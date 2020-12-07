#include "transformation.h"

// Translate all meshes in the scene by some offsets
void translate_scene(Scene &scene, double offset_x, double offset_y, double offset_z) {
  for (MeshPtr &mesh : scene.meshes) {
    std::cout << mesh->vertices << std::endl;
    for (int i = 0; i < mesh->vertices.rows(); i++) {
      auto vertex = mesh->vertices.row(i);
      vertex(0) += offset_x;
      vertex(1) += offset_y;
      vertex(2) += offset_z;
    }
    std::cout << "after: \n" << mesh->vertices << std::endl;
  }
}