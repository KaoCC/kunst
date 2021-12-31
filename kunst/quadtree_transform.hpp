
#pragma once

#include <opencv2/core/mat.hpp>

namespace kunst {
cv::Mat quadtree_transform(const cv::Mat& input_image, const bool is_animated, const size_t steps,
                           const size_t sample_period) noexcept;

}