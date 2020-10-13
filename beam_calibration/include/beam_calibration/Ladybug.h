/** @file
 * @ingroup calibration
 */

#pragma once
#include "beam_calibration/CameraModel.h"

#include <ladybug/ladybug.h>
#include <ladybug/ladybuggeom.h>
#include <ladybug/ladybugrenderer.h>

namespace beam_calibration {

/**
 * @brief Derived class for camera model
 * Due to being a specific case ladybug cannot be instantiated in the
 * Create or LoadJSON methods and must be manually created using
 * the conf file from the ladybug SDK
 */
class Ladybug : public CameraModel {
public:
  /**
   * @brief Constructor
   * @param input_file path to input file
   */
  Ladybug(const std::string& file_path);

  /**
   * @brief Default destructor
   */
  ~Ladybug() override = default;

  /**
   * @brief Method for projecting a point into an image plane (continous)
   * @param point 3d point to be projected [x,y,z]^T
   * @param pixel reference to an optional vector with image coordinates after
   * point has been projected into the image plane [u,v]^T
   */
  opt<Eigen::Vector2d>
      ProjectPointPrecise(const Eigen::Vector3d& point) override;

  /**
   * @brief Method for projecting a point into an image plane
   * @return projected point
   * @param point 3d point to be projected [x,y,z]^T
   */
  opt<Eigen::Vector2i> ProjectPoint(const Eigen::Vector3d& point) override;

  /**
   * @brief Overload projection function for computing jacobian of projection
   * @return projected point
   * @param point 3d point to be projected [x,y,z]^T
   * @param J 2 x 3 projection jacobian.
   * For ProjectPoint: [u,v]^T = [P1(x, y, z), P2(x, y, z)]^T
   *                   J = | dP1/dx , dP1/dy, dP1/dz |
   *                       | dP2/dx , dP2/dy, dP2/dz |
   */
  opt<Eigen::Vector2i> ProjectPoint(const Eigen::Vector3d& point,
                                    Eigen::MatrixXd& J) override;

  /**
   * @brief Method back projecting
   * @return Returns bearing vector
   * @param point [u,v]
   */
  opt<Eigen::Vector3d> BackProject(const Eigen::Vector2i& pixel) override;

  /**
   * @brief Method for setting which camera to use
   * @param id of the camera to use
   */
  void SetCameraID(unsigned int id);

protected:
  /**
   * @brief Checks if recent api call caused an error
   */
  void LadybugCheckError();

  LadybugContext lb_context_;
  LadybugError lb_error_;

  unsigned int cam_id_ = 0;
  const unsigned int LB_FULL_WIDTH_ = 2048;
  const unsigned int LB_FULL_HEIGHT_ = 2464;

  double focal_length_;
  double cx_;
  double cy_;
};

} // namespace beam_calibration