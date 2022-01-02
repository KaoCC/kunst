#pragma once

#include <array>
#include <memory>
#include <opencv2/core/mat.hpp>

namespace kunst {

class quadtree final {
 public:
  quadtree(const cv::Mat& input_image);
  [[nodiscard]] auto create_images(size_t steps, bool is_animated, size_t sample_period) const noexcept
      -> std::vector<cv::Mat>;

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

  auto build_tree(const cv::Mat& input_image, int start_row_index, int start_col_index, int height, int width) noexcept
      -> std::unique_ptr<node>;

  static void set_image_color(cv::Mat& image, const node& node) noexcept;

  // Row
  int max_height{};

  // Column
  int max_width{};

  std::unique_ptr<node> root{};
};

}  // namespace kunst