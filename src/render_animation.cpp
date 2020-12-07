#include <vector>
#include <gif.h>
#define PI           3.14159265358979323846

using namespace std;

void render_animation(const Scene &scene, FrameBuffer& frameBuffer, UniformAttributes &uniform) {
  const char * fileName = "triangle.gif";
  vector<uint8_t> image;
  int delay = 25;
  GifWriter g;
  GifBegin(&g, fileName, frameBuffer.rows(), frameBuffer.cols(), delay);

  for (float i = 0; i < 1; i += 0.05)
  {
    frameBuffer.setConstant(FrameBufferAttributes());
    uniform.translate_matrix(2, 3) += 0.2;
    uniform.rotation_angle += 15;
    float c = cos(uniform.rotation_angle * PI / 180);
    float s = sin(uniform.rotation_angle * PI / 180);
    uniform.rotation_matrix <<
    c, 0, s, 0,
    0, 1, 0, 0,
    -s, 0, c, 0,
    0, 0, 0, 1;
    render_flat(scene, frameBuffer, uniform);
    framebuffer_to_uint8(frameBuffer,image);
    GifWriteFrame(&g, image.data(), frameBuffer.rows(), frameBuffer.cols(), delay);
  }

  GifEnd(&g);
}