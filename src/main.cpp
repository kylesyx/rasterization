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
#define PI           3.14159265358979323846
#include "stb_image_write.h"

using namespace std;

void render_scene(Scene &scene, int shading_option) {
	// The Framebuffer storing the image rendered by the rasterizer
	Eigen::Matrix<FrameBufferAttributes,Eigen::Dynamic,Eigen::Dynamic> frameBuffer(500,500);

	UniformAttributes uniform;

	uniform.color << 1,1,1,1;
	uniform.camera = scene.camera;
	uniform.lights = scene.lights;
	uniform.background_color = scene.background_color;
	uniform.ambient_light = scene.ambient_light;
	uniform.material = scene.materials[0];

	Vector3d e = scene.camera.position;
	Vector3d w = -(scene.camera.gaze_direction.normalized());
	Vector3d u = (scene.camera.view_up.cross(w)).normalized();
	Vector3d v = w.cross(u).normalized();
	
	Matrix4d M;
	M <<
	u(0), v(0), w(0), e(0),
	u(1), v(1), w(1), e(1),
	u(2), v(2), w(2), e(2),
	0, 0, 0, 1;

	uniform.camera_transformation = M.inverse();

	double f = -9999;
	double n = -0.001;
	double t = std::abs(n) * std::tan(scene.camera.field_of_view / 2);
	double r = frameBuffer.rows() / frameBuffer.cols() * t;
	double b = -t;
	double l = -r;
	
	Matrix4d M_orth;
	M_orth <<
	2 / (r - l), 0, 0, -(r + l) / (r - l),
	0, 2 / (t - b), 0, -(t + b) / (t - b),
	0, 0, 2 / (n - f), -(n + f) / (n - f),
	0, 0, 0, 1;
	uniform.orth_projection = M_orth;

	Matrix4d M_perspective;
	M_perspective << 
	n, 0, 0, 0,
	0, n, 0, 0,
	0, 0, n + f, -(f * n),
	0, 0, 1, 0;
	uniform.perspective = M_perspective;

	Matrix4d M_translate;
	M_translate <<
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1;

	uniform.rotation_angle = 0;
	float c = cos(uniform.rotation_angle * PI / 180);
	float s = sin(uniform.rotation_angle * PI / 180);
	uniform.rotation_matrix <<
	c, 0, s, 0,
	0, 1, 0, 0,
	-s, 0, c, 0,
	0, 0, 0, 1;

	uniform.translate_matrix = M_translate;
	uniform.M = uniform.orth_projection * uniform.perspective * uniform.camera_transformation;


	switch (shading_option) {
		// 1: Wireframe
		case 1: {
			render_wireframe(scene, frameBuffer, uniform);
			break;
		}
		// 2: Flat shading
		case 2: {
			render_flat(scene, frameBuffer, uniform);
			break;
		}
		// 3: Per-vertex shading
		case 3: {
			render_per_vertex(scene, frameBuffer, uniform);
			break;
		}
		// 4: Animation
		case 4: {
			render_animation(scene, frameBuffer, uniform);
			return;
			break;
		}
		// 0: Basic
		default: {
			render_basic(scene, frameBuffer, uniform);
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
