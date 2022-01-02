#pragma once

#include <array>
#include <memory>
#include <opencv2/core/mat.hpp>

namespace kunst {

class quadtree final {
 public:
  quadtree(const cv::Mat& input_image);
  std::vector<cv::Mat> create_images(const size_t steps, const bool is_animated,
                                     const size_t sample_period) const noexcept;

 private:
  enum class quadrant : size_t { top_left = 0UL, top_right = 1UL, bottom_left = 2UL, bottom_right = 3UL };

  using color_type = cv::Vec3b;

  struct node {
    int start_row_index{};
    int start_col_index{};
    int height{};
    int width{};

    // Color
    color_type color{};

    double diff{};

    std::array<std::unique_ptr<node>, 4UL> children{};
  };

  static constexpr auto index(quadrant quad) noexcept { return static_cast<size_t>(quad); }

  std::unique_ptr<node> build_tree(const cv::Mat& input_image, const int start_row_index, int start_col_index,
                                   const int height, const int width) noexcept;

  static void set_image_color(cv::Mat& image, const node& node) noexcept;

  // Row
  int max_height{};

  // Column
  int max_width{};

  std::unique_ptr<node> root{};
};

}  // namespace kunst