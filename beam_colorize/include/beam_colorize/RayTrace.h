/** @file
 * @ingroup colorizer
 */

#pragma once

#include <mutex>
#include <thread>
#include <tuple>

#include <pcl/point_cloud.h>

#include <beam_colorize/Colorizer.h>

namespace beam_colorize {

/** @addtogroup colorizer
 *  @{ */

/**
 * @brief Class which implements Colorizer interface and provides colorization
 * functionality using ray tracing
 */
class RayTrace : public Colorizer {
public:
  RayTrace();

  ~RayTrace() = default;

  /**
   * @brief see Colorizer.h
   */
  ProjectionMap CreateProjectionMap(
      const PointCloudCol::Ptr& cloud_in_camera_frame) const override;

  /**
   * @brief see Colorizer.h
   */
  ProjectionMap CreateProjectionMap(
      const DefectCloud::Ptr& cloud_in_camera_frame) const override;

protected:
  double hit_threshold_ = 0.01;
};

/** @} group colorizer */

} // namespace beam_colorize
