#include "raster.h"
#include "mesh.h"

void render_per_vertex(const Scene &scene, FrameBuffer& frameBuffer, UniformAttributes &uniform) {
  // Basic rasterization program
	Program program;

	// The vertex shader is the identity
	program.VertexShader = [](const VertexAttributes& va, const UniformAttributes& uniform)
	{
		VertexAttributes out = va;

		Vector3d ambient_color = uniform.material.ambient_color.array() * uniform.ambient_light.array();
		Vector3d lights_color(0, 0, 0);

		for (const Light &light : uniform.lights) {
			Vector3d Li = (light.position - va.position.head<3>()).normalized();
			Vector3d N = va.normal.head<3>();

			Vector3d diffuse = uniform.material.diffuse_color * std::max(Li.dot(N), 0.0);
			Vector3d specular(0, 0, 0);
			Vector3d D = light.position - va.position.head<3>();
			lights_color += (diffuse + specular).cwiseProduct(light.intensity) /  D.squaredNorm();
		}
		Vector3d C = ambient_color + lights_color;
		out.color = Vector4d(C(0), C(1), C(2), 1);
		out.position = va.position;
		return out;
	};

	// The fragment shader
	program.FragmentShader = [](const VertexAttributes& va, const UniformAttributes& uniform)
	{
		FragmentAttributes out(va.color(0),va.color(1),va.color(2),va.color(3));
		out.position = va.position;
		return out;
	};

	// The blending shader converts colors between 0 and 1 to uint8
	program.BlendingShader = [](const FragmentAttributes& fa, const FrameBufferAttributes& previous)
	{
		if (fa.position[2] > previous.depth) {
			FrameBufferAttributes out(fa.color[0]*255, fa.color[1]*255, fa.color[2]*255, fa.color[3]*255);
			out.depth = fa.position[2];
			return out;
		} else {
			return previous;
		}
	};

	rasterize_triangles(program, uniform, get_meshes_vertices(scene, 1), frameBuffer);
}