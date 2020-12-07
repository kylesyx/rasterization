#pragma once

#include <Eigen/Core>
#include "types.h"

class VertexAttributes
{
	public:
	VertexAttributes(float x = 0, float y = 0, float z = 0, float w = 1, 
	float nx = 2, float ny = 0, float nz = 0, float nw = 1)
	{
		position << x, y, z, w;
		color << 1, 1, 1, 1;
		normal << nx, ny, nz, w;
	}

	// Interpolates the vertex attributes
	static VertexAttributes interpolate(
			const VertexAttributes& a,
			const VertexAttributes& b,
			const VertexAttributes& c,
			const float alpha, 
			const float beta, 
			const float gamma
	) 
	{
		VertexAttributes r;
		r.position = alpha * a.position + beta * b.position + gamma * c.position;
		r.color = alpha * a.color + beta * b.color + gamma * c.color;
		r.normal = alpha * a.normal + beta * b.normal + gamma * c.normal;
		return r;
	}

	Eigen::Vector4d position;
	Eigen::Vector4d color;
	Eigen::Vector4d normal;
};

class FragmentAttributes
{
	public:
	FragmentAttributes(float r = 0, float g = 0, float b = 0, float a = 1)
	{
		color << r,g,b,a;
	}

	Eigen::Vector4d color;
	Eigen::Vector4d position;
};

class FrameBufferAttributes
{
	public:
	FrameBufferAttributes(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255)
	{
		color << r,g,b,a;
		// this value should be between -1 and 1
		// 2 is further than the visible range
		depth = -2; 
	}

	Eigen::Matrix<uint8_t,4,1> color;
	float depth;
};

class UniformAttributes
{
	public:
	bool force_color = false;
	Eigen::Vector4d color;
	Camera camera;
	std::vector<Light> lights;
	Material material;
	Vector3d background_color;
	Vector3d ambient_light;
	Matrix4d translate_matrix;
	Matrix4d camera_transformation;
	Matrix4d orth_projection;
	Matrix4d M;
	Matrix4d rotation_matrix;
	double rotation_angle;
};