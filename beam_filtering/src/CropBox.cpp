#include "beam_filtering/CropBox.h"

namespace beam_filtering {

CropBox::CropBox() {
  T_box_cloud_.matrix().setIdentity();
}

Eigen::Vector3f CropBox::GetMinVector() {
  return min_vec_;
}

void CropBox::SetMinVector(Eigen::Vector3f& min_vec) {
  min_vec_ = min_vec;
  min_vec_set_ = true;
}

Eigen::Vector3f CropBox::GetMaxVector() {
  return max_vec_;
}

void CropBox::SetMaxVector(Eigen::Vector3f& max_vec) {
  max_vec_ = max_vec;
  max_vec_set_ = true;
}

Eigen::Affine3f CropBox::GetTransform() {
  return T_box_cloud_;
}

void CropBox::SetTransform(Eigen::Affine3f& T_box_cloud) {
  T_box_cloud_ = T_box_cloud;
}

void CropBox::Filter(pcl::PointCloud<pcl::PointXYZ>& input_cloud,
                     pcl::PointCloud<pcl::PointXYZ>& cropped_cloud) {
  // check that inputs are set:
  if (!min_vec_set_) {
    throw std::invalid_argument{"Attempting to crop without setting min_vec"};
  } else if (!max_vec_set_) {
    throw std::invalid_argument{"Attempting to crop without setting max_vec"};
  }

  // Clear points in output cloud
  cropped_cloud.clear();

  // perform filtering
  for (pcl::PointCloud<pcl::PointXYZ>::iterator it = input_cloud.begin();
       it != input_cloud.end(); ++it) {
    pcl::PointXYZ point = pcl::transformPoint(*it, T_box_cloud_);

    if (point.x < min_vec_[0] || point.y < min_vec_[1] ||
        point.z < min_vec_[2] || point.x > max_vec_[0] ||
        point.y > max_vec_[1] || point.z > max_vec_[2]) {
      continue;
    }
    cropped_cloud.push_back(*it);
  }
}

} // namespace beam_filtering