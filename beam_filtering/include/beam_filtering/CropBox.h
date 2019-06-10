/** @file
 * @ingroup filtering
 */

// PCL specific headers
#include <pcl/common/transforms.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

#pragma once

// beam
#include "beam_utils/math.hpp"

namespace beam_filtering {
/** @addtogroup filtering
 *  @{ */

/**
 * @brief class for crop box filter
 */
class CropBox {
public:
  /**
   * @brief constructor which sets some defaults
   */
  CropBox();

  /**
   * @brief Default destructor
   */
  ~CropBox() = default;

  /**
   * @brief Method for retrieving minimum vector [xmin, ymin, zmin]
   * @return min_vec
   */
  Eigen::Vector3f GetMinVector();

  /**
   * @brief Method for setting minimum vector [xmin, ymin, zmin]
   * @param min_vec
   */
  void SetMinVector(Eigen::Vector3f &min_vec);

  /**
   * @brief Method for retrieving maximum vector [xmax, ymax, zmax]
   * @return max_vec
   */
  Eigen::Vector3f GetMaxVector();

  /**
   * @brief Method for setting maximum vector [xmax, ymax, zmax]
   * @param max_vec
   */
  void SetMaxVector(Eigen::Vector3f &max_vec);

  /**
   * @brief Method for getting the transformation used to transform the cropbox
   * @return T_box_cloud (from cloud frame to box frame)
   */
  Eigen::Affine3f GetTransform();

  /**
   * @brief Method for applying a transform to the cropbox
   * @param T_box_cloud (from cloud frame to box frame)
   */
  void SetTransform(Eigen::Affine3f &T_box_cloud);

  /**
   * @brief Method for applying the cropbox
   * @param input_cloud cloud to be cropped
   * @param cropped_cloud cloud to save to
   */
   void Filter(pcl::PointCloud<pcl::PointXYZ> &input_cloud,
               pcl::PointCloud<pcl::PointXYZ> &cropped_cloud);

private:
  Eigen::Vector3f  min_vec_, max_vec_;
  Eigen::Affine3f T_box_cloud_;
  bool min_vec_set_ = false, max_vec_set_ = false;
};

/** @} group filtering */

} // namespace beam_filtering