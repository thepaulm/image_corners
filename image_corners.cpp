#include <stdio.h>
#include <string.h>
#include <math.h>

#include <opencv2/opencv.hpp>

using namespace cv;

void syntax() {
  printf("image_corners -i input_image -r radius\n");
}

double cart_distance(int y1, int x1, int y2, int x2) {
  return (double)sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void operate(unsigned char *pdst, int width, int row, int col, int radius, int center_x, int center_y) {
    double d = cart_distance(row, col, center_y, center_x);
    if (d > radius) {
      pdst[col + row * width] = 0;
      if (d - radius < 1.0) {
        int val = 255 - ((d - (double)radius) * 255);
        pdst[col + row * width] = val;
      }
    }
}

int work_on(const char *image, int radius) {
  Mat src;
  Mat dst;
  src = imread(image);

  if (src.empty()) {
    printf("No image named %s\n", image);
    return -1;
  }

  Size size = src.size();
  printf("Image dims: %d width X %d height\n", size.width, size.height);
  cvtColor(src, dst, COLOR_RGB2RGBA);

  std::vector<cv::Mat>channels(4);
  cv::split(dst, channels);

  int center_x, center_y;

  unsigned char *pdst = channels[3].ptr<unsigned char>();
  for (int row = 0; row < size.height; row++) {
    for (int col = 0; col < size.width; col++) {

      /* top left */
      center_x = center_y = radius;
      if (row < center_y && col < center_x) {
        operate(pdst, size.width, row, col, radius, center_x, center_y);
      }

      /* top right */
      center_x = size.width - radius;
      center_y = radius;
      if (row < center_y && col > center_x) {
        operate(pdst, size.width, row, col, radius, center_x, center_y);
      }

      /* bottom left */
      center_x = radius;
      center_y = size.height - radius;
      if (row > center_y && col < center_x) {
        operate(pdst, size.width, row, col, radius, center_x, center_y);
      }

      /* bottom right */
      center_x = size.width - radius;
      center_y = size.height - radius;
      if (row > center_y && col > center_x) {
        operate(pdst, size.width, row, col, radius, center_x, center_y);
      }
    }
  }

  merge(channels, dst);

  std::string outname = std::string(image) + std::string("_") + std::to_string(radius) + std::string(".png");
  printf("Writing %s ...\n", outname.c_str());
  imwrite(outname, dst);
  printf("done.\n");
  return 0;
}

int main(int argc, char *argv[]) {
  const char *input_image = NULL;
  int radius = 100;

  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-i")) {
      i++;
      if (i == argc) {
        syntax();
        return -1;
      }
      input_image = argv[i];
    } else if (!strcmp(argv[i], "-r")) {
      i++;
      if (i == argc) {
        syntax();
        return -1;
      }
      radius = atoi(argv[i]);
    }
  }
  if (input_image == NULL) {
    syntax();
    return 0;
  }
  return work_on(input_image, radius);
  return 0;
}
