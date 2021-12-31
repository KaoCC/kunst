
#include "quadtree_transform.hpp"

#include "quadtree.hpp"

namespace kunst {
cv::Mat quadtree_transform(const cv::Mat& input_image, const bool is_animated, const size_t steps,
                           const size_t sample_period) noexcept {
  if (steps == 0UL || sample_period == 0UL) {
    return {};
  }

  quadtree tree(input_image);

  return {};
}
}  // namespace kunst