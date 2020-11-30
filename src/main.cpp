// C++ include
#include <iostream>
#include <string>
#include <vector>

// Utilities for the Assignment
#include "raster.h"

// Image writing library
#define STB_IMAGE_WRITE_IMPLEMENTATION // Do not include this line twice in your project!
#include "stb_image_write.h"

#include <gif.h>

using namespace std;

int main() 
{

	// The Framebuffer storing the image rendered by the rasterizer
	Eigen::Matrix<FrameBufferAttributes,Eigen::Dynamic,Eigen::Dynamic> frameBuffer(500,500);

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

		FragmentAttributes out(uniform.triangle_color[0],uniform.triangle_color[1],uniform.triangle_color[2],uniform.triangle_color[3]);
		out.position = va.position;
		return out;
	};

	// The blending shader converts colors between 0 and 1 to uint8
	program.BlendingShader = [](const FragmentAttributes& fa, const FrameBufferAttributes& previous)
	{
		float alpha = fa.color[3];

		Eigen::Vector4f outcolor = fa.color.array() * alpha + (previous.color.cast<float>().array() / 255.0) * (1 - alpha);
		
		return FrameBufferAttributes(outcolor[0]*255,outcolor[1]*255,outcolor[2]*255,outcolor[3]*255);
		
	};

	// One triangle in the center of the screen
	vector<VertexAttributes> vertices_1;
	vertices_1.push_back(VertexAttributes(-1,-1,0));
	vertices_1.push_back(VertexAttributes(1,-1,0));
	vertices_1.push_back(VertexAttributes(-1,1,0));

	vector<VertexAttributes> vertices_2;
	vertices_2.push_back(VertexAttributes(-1,-1,0.5));
	vertices_2.push_back(VertexAttributes(1,-1,0.5));
	vertices_2.push_back(VertexAttributes(1,1,0.5));

	uniform.triangle_color << 1,0,0,1;
	rasterize_triangles(program,uniform,vertices_1,frameBuffer);

	uniform.triangle_color << 0,0,1,0.5;
	rasterize_triangles(program,uniform,vertices_2,frameBuffer);


	vector<uint8_t> image;
	framebuffer_to_uint8(frameBuffer,image);
	stbi_write_png("triangle.png", frameBuffer.rows(), frameBuffer.cols(), 4, image.data(), frameBuffer.rows()*4);
	
	return 0;
}
