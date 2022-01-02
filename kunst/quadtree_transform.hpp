
#pragma once

#include <opencv2/core/mat.hpp>

namespace kunst {
[[nodiscard]] auto quadtree_transform(const cv::Mat& input_image, bool is_animated, size_t steps,
                                      size_t sample_period) noexcept -> std::vector<cv::Mat>;

}