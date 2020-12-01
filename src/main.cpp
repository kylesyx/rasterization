// C++ include
#include <iostream>
#include <string>
#include <vector>

// Utilities for the Assignment
#include "raster.h"
#include "types.h"
#include "mesh.h"
#include "load.h"

// Image writing library
#define STB_IMAGE_WRITE_IMPLEMENTATION // Do not include this line twice in your project!
#include "stb_image_write.h"

using namespace std;

void render_scene(const Scene &scene, int shading_option) {
	// The Framebuffer storing the image rendered by the rasterizer
	Eigen::Matrix<FrameBufferAttributes,Eigen::Dynamic,Eigen::Dynamic> frameBuffer(500,500);

	// Global Constants (empty in this example)
	UniformAttributes uniform;
	uniform.color << 1,1,1,1;
	uniform.camera = scene.camera;
	uniform.lights = scene.lights;

	// Basic rasterization program
	Program program;

	// The vertex shader is the identity
	program.VertexShader = [](const VertexAttributes& va, const UniformAttributes& uniform)
	{
		return va;
	};

	// The fragment shader uses a fixed color
	program.FragmentShader = [](const VertexAttributes& va, const UniformAttributes& uniform)
	{
		FragmentAttributes out(uniform.color(0),uniform.color(1),uniform.color(2),uniform.color(3));
		out.position = va.position;
		return out;
	};

	// The blending shader converts colors between 0 and 1 to uint8
	program.BlendingShader = [](const FragmentAttributes& fa, const FrameBufferAttributes& previous)
	{
		// Camera is at (0,0,-1)
		// z smaller, closer the object
		if (fa.position[2] < previous.depth) {
			FrameBufferAttributes out(fa.color[0]*255, fa.color[1]*255, fa.color[2]*255, fa.color[3]*255);
			out.depth = fa.position[2];
			return out;
		} else {
			return previous;
		}
	};

	switch (shading_option) {
		// Wireframe
		case 1: {
			rasterize_lines(program, uniform, get_meshes_vertices(scene, "lines"), 1, frameBuffer);
			break;
		}
		// Flat shading
		case 2: {
			rasterize_triangles(program, uniform, get_meshes_vertices(scene, "triangles"), frameBuffer);
			break;
		}
		// Per-vertex shading
		case 3: {
			break;
		}
		// Basic
		default: {
			rasterize_triangles(program, uniform, get_meshes_vertices(scene, "triangles"), frameBuffer);
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
	option = option >= 0 && option < 4 ? option : 1;

	render_scene(scene, option);
	return 0;
}
