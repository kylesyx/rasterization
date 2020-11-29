// C++ include
#include <iostream>
#include <string>
#include <vector>

// Utilities for the Assignment
#include "raster.h"

// Image writing library
#define STB_IMAGE_WRITE_IMPLEMENTATION // Do not include this line twice in your project!
#include "stb_image_write.h"

using namespace std;

int main() 
{

	// The Framebuffer storing the image rendered by the rasterizer
	Eigen::Matrix<FrameBufferAttributes,Eigen::Dynamic,Eigen::Dynamic> frameBuffer(500,500);
	// Eigen::Matrix<FrameBufferAttributes,Eigen::Dynamic,Eigen::Dynamic> frameBuffer(50,50);

	// Global Constants (empty in this example)
	UniformAttributes uniform;

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
		return FragmentAttributes(0, 1, 0);
		// return FragmentAttributes(va.color[0], va.color[1], va.color[2]);
	};

	// The blending shader converts colors between 0 and 1 to uint8
	program.BlendingShader = [](const FragmentAttributes& fa, const FrameBufferAttributes& previous)
	{
		return FrameBufferAttributes(fa.color[0]*255,fa.color[1]*255,fa.color[2]*255,fa.color[3]*255);
	};

	// One triangle in the center of the screen
	vector<VertexAttributes> vertices;
	//Edge 1
	vertices.push_back(VertexAttributes(-1,-1,0));
	vertices.push_back(VertexAttributes(1,-1,0));

	// Edge 2
	vertices.push_back(VertexAttributes(1,-1,0));
	vertices.push_back(VertexAttributes(0,1,0));

	// Edge 3
	vertices.clear();
	vertices.push_back(VertexAttributes(-1,-1,0));
	vertices.push_back(VertexAttributes(0,1,0));

	// Change background color
	for (unsigned i = 0; i < frameBuffer.rows(); i++)
		for (unsigned j = 0; j < frameBuffer.cols(); j++)
			frameBuffer(i, j).color << 255, 255, 255, 255;

	vertices[0].color << 1, 0, 0, 1;
	vertices[1].color << 0, 0, 1, 1;
	vertices[2].color << 0, 1, 0, 1;

	rasterize_lines(program, uniform, vertices, 5, frameBuffer);


	vector<uint8_t> image;
	framebuffer_to_uint8(frameBuffer,image);
	stbi_write_png("triangle.png", frameBuffer.rows(), frameBuffer.cols(), 4, image.data(), frameBuffer.rows()*4);
	
	return 0;
}
