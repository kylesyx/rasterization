#include <vector>
#include <gif.h>

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
    render_wireframe(scene, frameBuffer, uniform);
    framebuffer_to_uint8(frameBuffer,image);
    GifWriteFrame(&g, image.data(), frameBuffer.rows(), frameBuffer.cols(), delay);
  }

  GifEnd(&g);
}