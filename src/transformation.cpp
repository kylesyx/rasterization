#include "transformation.h"

// Translate all meshes in the scene by some offsets
void translate(Scene &scene, double offset_x, double offset_y, double offset_z) {
  for (MeshPtr mesh : scene.meshes) {
    for (int i = 0; i < mesh->vertices.rows(); i++) {
      VectorXd vertex = mesh->vertices.row(i);
      vertex(0) += offset_x;
      vertex(1) += offset_y;
      vertex(2) += offset_z;
    }
  }
}