
#include "quadtree.hpp"

namespace {

auto average_color(const cv::Vec3f& input_color, const int area) noexcept { return input_color / area; }

template <typename callback>
auto accumulate(callback&& transform_function, const cv::Mat& input_image, const int start_row_index,
                const int start_col_index, const int height, const int width) noexcept {
  cv::Vec3f sum{};
  for (auto i = start_row_index; i < start_row_index + height; ++i) {
    for (auto j = start_col_index; j < start_col_index + width; ++j) {
      const auto color{input_image.at<cv::Vec3b>(i, j)};

      sum += std::invoke(std::forward<callback>(transform_function), color);
    }
  }

  return sum;
}

}  // namespace

namespace kunst {

quadtree::quadtree(const cv::Mat& input_image)
    : max_height{input_image.size().height}, max_width{input_image.size().width} {
  constexpr auto start_row_index{0};
  constexpr auto start_col_index{0};

  root = build_tree(input_image, start_row_index, start_col_index, max_height, max_width);
}

std::unique_ptr<quadtree::node> quadtree::build_tree(const cv::Mat& input_image, const int start_row_index,
                                                     int start_col_index, const int height, const int width) noexcept {
  auto current_node = std::make_unique<node>();
  current_node->start_row_index = start_row_index;
  current_node->start_col_index = start_col_index;
  current_node->height = height;
  current_node->width = width;

  const auto area{height * width};

  if (height == 1 || width == 1) {
    const auto sum_color{accumulate([](const auto& color) noexcept { return cv::Vec3f{color}; }, input_image,
                                    start_row_index, start_col_index, height, width)};

    const auto mean{average_color(sum_color, area)};
    current_node->color = static_cast<cv::Vec3b>(mean);

    const auto diff_rbg = accumulate(
        [&mean](const auto& color) noexcept {
          const auto delta{cv::Vec3f{color} - mean};
          return cv::Vec3f{delta[0] * delta[0], delta[1] * delta[1], delta[2] * delta[2]};
        },
        input_image, start_row_index, start_col_index, height, width);

    current_node->diff = diff_rbg[0] + diff_rbg[1] + diff_rbg[2];

    return current_node;
  }

  const auto half_height{height / 2};
  const auto half_width{width / 2};

  current_node->children[index(quadrant::top_left)] =
      build_tree(input_image, start_row_index, start_col_index, half_height, half_width);
  current_node->children[index(quadrant::top_right)] =
      build_tree(input_image, start_row_index, start_col_index + half_width, half_height, width - half_width);
  current_node->children[index(quadrant::bottom_left)] =
      build_tree(input_image, start_row_index + half_height, start_col_index, height - half_height, half_width);
  current_node->children[index(quadrant::bottom_right)] =
      build_tree(input_image, start_row_index + half_height, start_col_index + half_width, height - half_height,
                 width - half_width);

  cv::Vec3f sum_color{};
  for (const auto& child : current_node->children) {
    const auto child_area{child->height * child->width};
    sum_color += (child->color * child_area);
  }

  const auto mean{average_color(sum_color, area)};
  current_node->color = static_cast<cv::Vec3b>(mean);

  const auto diff_rbg = accumulate(
      [&mean](const auto& color) noexcept {
        const auto delta{cv::Vec3f{color} - mean};
        return cv::Vec3f{delta[0] * delta[0], delta[1] * delta[1], delta[2] * delta[2]};
      },
      input_image, start_row_index, start_col_index, height, width);

  current_node->diff = diff_rbg[0] + diff_rbg[1] + diff_rbg[2];

  return current_node;
}

}  // namespace kunst