
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>

#include "quadtree_transform.hpp"

auto main() -> int {
  constexpr auto test_image{"lenna.png"};

  const auto input_image{cv::imread(test_image)};

  if (input_image.data == nullptr) {
    std::cerr << "No Data. Failed to load the image" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::cout << "Input Image Size (height, width) : " << input_image.size() << std::endl;

  std::cout << "Quadtree Transform" << std::endl;

  constexpr auto enable_animation{true};
  constexpr auto step{10000UL};
  constexpr auto sample_period{10UL};

  const auto result{kunst::quadtree_transform(input_image, enable_animation, step, sample_period)};

  if (std::empty(result)) {
    std::cerr << "Failed to generate the result." << std::endl;
    return EXIT_FAILURE;
  }

  /// Create the image.

  std::cout << "Creating the image." << std::endl;

  constexpr auto output_image_file{"out.jpg"};

  try {
    cv::imwrite(output_image_file, result[std::size(result) - 1UL]);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }

  /// Create a video.

  std::cout << "Creating the animated video" << std::endl;

  constexpr auto output_video_file{"out.mp4"};

  std::cout << "1st result size: " << result[0UL].size() << std::endl;

  try {
    constexpr auto fps{10.0};

    const auto fourcc{cv::VideoWriter::fourcc('a', 'v', 'c', '1')};
    cv::VideoWriter video_writer(output_video_file, fourcc, fps, input_image.size());

    if (!video_writer.isOpened()) {
      std::cerr << "Could not open the output video for write: " << output_video_file << std::endl;
      std::exit(EXIT_FAILURE);
    }

    std::cout << "Start writing to video: " << output_video_file << std::endl;

    for (const auto &image : result) {
      video_writer.write(image);
    }

  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::cout << "Done !!" << std::endl;

  return 0;
}