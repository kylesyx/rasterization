// C++ include
#include <iostream>
#include <string>
#include <vector>

// Utilities for the Assignment
#include "raster.h"
#include "types.h"
#include "load.h"

// Image writing library
#define STB_IMAGE_WRITE_IMPLEMENTATION // Do not include this line twice in your project!
#include "stb_image_write.h"

using namespace std;

void render_scene(const Scene &scene) {
	
}

int main(int argc, char *argv[]) 
{
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " scene.json" << std::endl;
		return 1;
	}
	Scene scene = load_scene(argv[1]);
	render_scene(scene);

	// // The Framebuffer storing the image rendered by the rasterizer
	// Eigen::Matrix<FrameBufferAttributes,Eigen::Dynamic,Eigen::Dynamic> frameBuffer(500,500);

	// // Global Constants (empty in this example)
	// UniformAttributes uniform;

	// // Basic rasterization program
	// Program program;

	// // The vertex shader is the identity
	// program.VertexShader = [](const VertexAttributes& va, const UniformAttributes& uniform)
	// {
	// 	return va;
	// };

	// // The fragment shader uses a fixed color
	// program.FragmentShader = [](const VertexAttributes& va, const UniformAttributes& uniform)
	// {
	// 	return FragmentAttributes(1,0,0);
	// };

	// // The blending shader converts colors between 0 and 1 to uint8
	// program.BlendingShader = [](const FragmentAttributes& fa, const FrameBufferAttributes& previous)
	// {
	// 	return FrameBufferAttributes(fa.color[0]*255,fa.color[1]*255,fa.color[2]*255,fa.color[3]*255);
	// };

	// // One triangle in the center of the screen
	// vector<VertexAttributes> vertices;
	// vertices.push_back(VertexAttributes(-1,-1,0));
	// vertices.push_back(VertexAttributes(1,-1,0));
	// vertices.push_back(VertexAttributes(0,1,0));

	// rasterize_triangles(program,uniform,vertices,frameBuffer);

	// vector<uint8_t> image;
	// framebuffer_to_uint8(frameBuffer,image);
	// stbi_write_png("triangle.png", frameBuffer.rows(), frameBuffer.cols(), 4, image.data(), frameBuffer.rows()*4);
	
	return 0;
}
