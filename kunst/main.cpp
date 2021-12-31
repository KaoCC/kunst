
#include <iostream>
#include <opencv2/imgcodecs.hpp>

#include "quadtree_transform.hpp"

int main() {
  constexpr auto test_image{"test.jpg"};

  const auto image{cv::imread(test_image)};

  if (!image.data) {
    std::cerr << "No Data. Failed to load the image" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  const auto result{kunst::quadtree_transform(image, false, 1, 1)};

  return 0;
}