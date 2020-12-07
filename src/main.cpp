// C++ include
#include <iostream>
#include <string>
#include <vector>

// Utilities for the Assignment
#include "raster.h"
#include "types.h"
#include "mesh.h"
#include "load.h"
#include "transformation.h"
#include "shading_wireframe.cpp"
#include "shading_flat.cpp"
#include "shading_basic.cpp"
#include "shading_per_vertex.cpp"
#include "render_animation.cpp"

// Image writing library
#define STB_IMAGE_WRITE_IMPLEMENTATION // Do not include this line twice in your project!
#include "stb_image_write.h"

using namespace std;

void render_scene(Scene &scene, int shading_option) {
	// The Framebuffer storing the image rendered by the rasterizer
	Eigen::Matrix<FrameBufferAttributes,Eigen::Dynamic,Eigen::Dynamic> frameBuffer(50,50);

	switch (shading_option) {
		// Wireframe
		case 1: {
			render_wireframe(scene, frameBuffer);
			break;
		}
		// Flat shading
		case 2: {
			render_flat(scene, frameBuffer);
			break;
		}
		// Per-vertex shading
		case 3: {
			render_per_vertex(scene, frameBuffer);
			break;
		}
		// Animation
		case 4: {
			render_animation(scene, frameBuffer);
			return;
			break;
		}
		// Basic
		default: {
			render_basic(scene, frameBuffer);
			break;
		}
	}

	vector<uint8_t> image;
	framebuffer_to_uint8(frameBuffer,image);
	stbi_write_png("result.png", frameBuffer.rows(), frameBuffer.cols(), 4, image.data(), frameBuffer.rows()*4);
}

int main(int argc, char *argv[]) 
{
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " scene.json" << "option: 0 | 1 | 2 | 3" << std::endl;
		return 1;
	}
	Scene scene = load_scene(argv[1]);
	int option = stoi(argv[2], nullptr, 10);
	option = option >= 0 && option < 5 ? option : 1;

	render_scene(scene, option);
	return 0;
}
