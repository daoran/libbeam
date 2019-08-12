/** @file
 * @ingroup mapping
 */

#include "beam_calibration/TfTree.h"
#include "beam_mapping/Poses.h"
#include "beam_utils/math.hpp"

// PCL specific headers
#include <pcl/common/transforms.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

// ROS headers
#include <rosbag/bag.h>
#include <rosbag/view.h>

#pragma once

namespace beam_mapping {
/** @addtogroup mapping
 *  @{ */

using filter_params_type = std::pair<std::string, std::vector<double>>;
using PointT = pcl::PointXYZI;
using PointCloud = pcl::PointCloud<PointT>;

/**
 * @brief class for map builder
 */
class MapBuilder {
public:
  /**
   * @brief constructor which sets some defaults
   * @param config_file full path to configuration file
   */
  MapBuilder(const std::string& config_file);

  /**
   * @brief Default destructor
   */
  ~MapBuilder() = default;

  /**
   * @brief Method for getting the filter params
   * @param filter
   * @return filter_parameters
   */
  filter_params_type GetFilterParams(const auto& filter);

  /**
   * @brief for overriding the bag file specified in the config file
   * @param bag_file full path to new bag file
   */
  void OverrideBagFile(const std::string& bag_file);

  /**
   * @brief for overriding the poses file specified in the config file
   * @param poses_file full path to new poses file
   */
  void OverridePoseFile(const std::string& poses_file);

  /**
   * @brief performs the map building
   */
  void BuildMap();

private:
  /**
   * @brief method to load poses from json and extrinsics
   * @param poses_file full path to poses file
   */
  void LoadTree(const std::string& poses_file);

  /**
   * @brief method to load configuration from json
   * @param config_file full path to configuration file
   */
  void LoadConfigFromJSON(const std::string& config_file);

  /**
   * @brief checks the rotation and translation change between current pose and
   * last pose and outputs bool of whether it was greater than specified the
   * specified thresholds
   * @return save_scan whether or not to save the current scan
   */
  bool CheckPoseChange();

  /**
   * @brief processes a point cloud message by first checking if the pose has
   * changed more than the threshold, if so convert it and add to the scans and
   * timestamp vectors
   * @param iter rosbag iterator
   */
  void ProcessPointCloudMsg(rosbag::View::iterator &iter);

  /**
   * @brief loads all the scans from a specific lidar and builds the scans and
   * timestamps vector.
   * @param lidar_number
   */
   void LoadScans(uint8_t lidar_number);

  // From Config file
  std::string pose_file_path_, bag_file_path_, bag_file_name_, save_dir_,
  config_file_, extrinsics_file_;
  int intermediary_map_size_;
  double min_translation_, min_rotation_deg_;
  bool combine_lidar_scans_;
  std::vector<std::string> lidar_topics_, lidar_frames_;
  std::vector<std::vector<double>> lidar_cropbox_min_, lidar_cropbox_max_;
  std::vector<bool> lidar_cropbox_bool_;
  std::vector<filter_params_type> input_filters_, intermediary_filters_,
  output_filters_;

  // New objects
  beam_mapping::Poses poses_;
  beam_calibration::TfTree tree_;
  PointCloud::Ptr aggregate_;
  std::vector<PointCloud::Ptr> scans_;
  std::vector<ros::Time> time_stamps_;
  Eigen::Affine3d scan_pose_last_, scan_pose_current_;
};

/** @} group mapping */

} // namespace beam_mapping