/** @file
 * @ingroup matching
 *
 * Wrapper of ICP in PCL
 *
 * There are a few parameters that may be changed specific to this algorithm.
 * They can be set in the json config file.
 *
 * - max_corr: correspondences behind this many distance-units are
 * discarded
 * - max_iter: Limits number of ICP iterations
 * - t_eps: Criteria to stop iterating. If the difference between consecutive
 * transformations is less than this, stop.
 * - fit_eps: Criteria to stop iterating. If the cost function does not improve
 * by more than this quantity, stop.
 */

#pragma once

#include <pcl/filters/voxel_grid.h>
#include <pcl/registration/icp.h>

#include <beam_matching/Matcher.h>
#include <beam_utils/pointclouds.h>

namespace beam_matching {
/** @addtogroup matching
 *  @{ */

class IcpMatcher : public Matcher<PointCloudPtr> {
public:

  struct Params {
    enum CovarMethod : int { LUM, CENSI, LUMold };
    
    Params(const std::string& param_config);
    Params() {}

    /// Maximum distance to correspond points for icp
    double max_corr{3};

    /// Maximum iterations of ICP
    int max_iter{100};

    /// Transformation epsilon. Stopping criteria. If the transform changes by
    /// less than this amount, stop
    double t_eps{1e-8};

    /// Stopping criteria, if cost function decreases by less than this, stop
    double fit_eps{1e-2};

    /// Angular variance for lidar sensor model. Used if Censi covariance
    /// estimation is set
    double lidar_ang_covar{7.78e-9};

    /// Linear variance for lidar sensor model. Used if Censi covariance
    /// estimation is set
    double lidar_lin_covar{2.5e-4};

    /// When set to more than 0, each match is performed multiple times from
    /// a coarse to fine scale (in terms of voxel downsampling).
    /// Each step doubles the resolution
    int multiscale_steps{0};

    /// Voxel side length for downsampling. If set to 0, downsampling is
    /// not performed. If multiscale matching is set, this is the resolution
    /// of the final, fine-scale match
    float res{0.1};

    CovarMethod covar_estimator = CovarMethod::LUM;
  };

  IcpMatcher() = default;
  /**
   * @brief This constructor takes an argument in order to adjust how much
   * downsampling is done before matching is attempted. Pointclouds are
   * downsampled using a voxel filter, the argument is the edge length of
   * each voxel. If resolution is non-positive, no downsampling is used.
   */
  IcpMatcher(Params params);

  ~IcpMatcher();

  /**
   * @brief sets the parameters for the matcher
   * @param params - IcpMatcher Params
   */
  void SetParams(Params params);

  /**
   * @brief sets the reference pointcloud for the matcher
   * @param ref - Pointcloud
   */
  void SetRef(const PointCloudPtr& ref);

  /**
   * @brief sets the target (or scene) pointcloud for the matcher
   * @param target - Pointcloud
   */
  void SetTarget(const PointCloudPtr& target);

  /**
   * @brief runs the matcher, blocks until finished.
   * @return true if successful
   */
  bool Match() override;

  /**
   * @brief estimates the covariance using the method expressed in params
   * (covar_estimator)
   */
  void CalculateCovariance() override;

  /**
   * @brief gets the parameters for the matcher
   * @return IcpMatcher Params
   */
  Params GetParams() { return params_; }

  /**
   * @brief see matcher.h for details
   */
  void SaveResults(const std::string& output_dir,
                   const std::string& prefix = "cloud") override;

private:
  /**
   * @brief Calculates a covariance estimate based on Lu and Milios Scan
   * Matching
   */
  void EstimateLUM();
  void EstimateLUMold();

  /**
   * @brief Calculates a covariance estimate based on Censi
   */
  void EstimateCensi();

  /**
   * @brief configures icp matcher with the parameters
   */
  void SetIcpParams();

  /** An instance of the ICP class from PCL */
  pcl::IterativeClosestPoint<pcl::PointXYZ, pcl::PointXYZ> icp_;

  /** An instance of a PCL voxel filter. It is used to downsample input. */
  pcl::VoxelGrid<pcl::PointXYZ> filter_;

  /**
   * Pointers to the reference and target pointclouds. The "final" pointcloud
   * is not exposed. PCL's ICP class creates an aligned verison of the target
   * pointcloud after matching, so the "final" member is used as a sink for
   * it.
   */
  PointCloudPtr ref_;
  PointCloudPtr target_;
  PointCloudPtr final_;
  PointCloudPtr downsampled_ref_;
  PointCloudPtr downsampled_target_;

  Params params_;
};

using IcpMatcherParams = IcpMatcher::Params;

/** @} group matching */
} // namespace beam_matching
