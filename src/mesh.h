#pragma once

#include "raster.h"
#include "load.h"

std::vector<VertexAttributes> get_meshes_vertices(const Scene &scene, const std::string &type = "triangles");