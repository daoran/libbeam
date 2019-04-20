/** @file
 * @ingroup utils
 *
 * Utility mathematical functions that do not fit anywhere else.
 *
 * The type definitions are shorthands for declaring `Eigen::Vector`,
 * `Eigen::Matrix` and `Eigen::Quaternion` objects. At current we assume all
 * slam code development will be using double precision floating points.
 */

#ifndef BEAM_UTILS_MATH_HPP
#define BEAM_UTILS_MATH_HPP

#include <iostream>
#include <vector>

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <beam_utils/time.hpp>
#include <unsupported/Eigen/MatrixFunctions>

namespace beam {
/** @addtogroup utils
 *  @{ */

#ifndef BEAM_EIGEN_TYPEDEF
#  define BEAM_EIGEN_TYPEDEF
typedef Eigen::Vector2d Vec2;
typedef Eigen::Vector3d Vec3;
typedef Eigen::Vector4d Vec4;
typedef Eigen::Matrix<double, 5, 1> Vec5;
typedef Eigen::Matrix<double, 6, 1> Vec6;
typedef Eigen::VectorXd VecX;

typedef Eigen::Matrix2d Mat2;
typedef Eigen::Matrix3d Mat3;
typedef Eigen::Matrix4d Mat4;
typedef Eigen::Matrix<double, 5, 5> Mat5;
typedef Eigen::Matrix<double, 6, 6> Mat6;
typedef Eigen::MatrixXd MatX;

typedef Eigen::Affine3d Affine3;

typedef Eigen::Quaterniond Quaternion;
#endif // BEAM_EIGEN_TYPEDEF

/**
 * Eigen vector comparator
 */
struct VecComparator {
  bool operator()(const VecX& a, const VecX& b) const {
    return std::lexicographical_compare(a.data(), a.data() + a.size(), b.data(),
                                        b.data() + b.size());
  }
};

/**
 * Eigen matrix comparator
 */
struct MatComparator {
  bool operator()(const MatX& a, const MatX& b) const {
    return std::lexicographical_compare(a.data(), a.data() + a.size(), b.data(),
                                        b.data() + b.size());
  }
};

/** Generates random integer with a upper bound `ub` and lower bound `lb` using
 * a uniform random distribution. */
int randi(int ub, int lb);

/** Generates random float with a upper bound `ub` and lower bound `lb` using a
 * uniform random distribution. */
double randf(double ub, double lb);

/** Compares two floating point numbers `f1` and `f2` with an error threshold
 * defined by `threshold`.
 * @return
 * - `0`: If `f1` == `f2` or the difference is less then `threshold`
 * - `1`: If `f1` > `f2`
 * - `-1`: If `f1` < `f2`
 */
int fltcmp(double f1, double f2, double threshold = 0.0001);

/** @return the median of `v`. */
double median(std::vector<double> v);

/** Reshapes a vector `x` to matrix `y` of size `rows` and `cols` */
void vec2mat(std::vector<double> x, int rows, int cols, MatX& y);

/** Reshapes a matrix to a vector*/
void mat2vec(MatX A, std::vector<double>& x);

/** Convert euler angle to rotation matrix **/
int euler2rot(Vec3 euler, int euler_seq, Mat3& R);

/** Convert euler angle to quaternion **/
int euler2quat(Vec3 euler, int euler_seq, Quaternion& q);

/** Convert quaternion to euler angles **/
int quat2euler(Quaternion q, int euler_seq, Vec3& euler);

/** Convert quaternion to rotation matrix **/
int quat2rot(Quaternion q, Mat3& R);

/** ENU to NWU coordinate system **/
void enu2nwu(const Vec3& enu, Vec3& nwu);

/** NED to ENU coordinate system **/
void ned2enu(const Vec3& ned, Vec3& enu);

/** NED to NWU coordinate system **/
void ned2nwu(const Quaternion& ned, Quaternion& enu);

/** NWU to ENU coordinate system **/
void nwu2enu(const Vec3& nwu, Vec3& enu);

/** NWU to NED coordinate system **/
void nwu2ned(const Quaternion& nwu, Quaternion& ned);

/** NWU to EDN coordinate system **/
void nwu2edn(const Vec3& nwu, Vec3& edn);

/**
 * @brief Round a matrix values to a certain precision.
 * @param precision = 100 would round to the second decimal point (i.e. 1.126
 *= 1.13)
 **/
MatX RoundMatrix(const MatX& M, int& precision);

/**
 * @brief check if a matrix is a valid transformation matrix
 * @param T tranformation
 **/
bool IsTransformationMatrix(Eigen::Matrix4d T);

/**
 * @brief check if a matrix is a valid rotation matrix
 * @param R rotation matrix
 **/
bool IsRotationMatrix(Eigen::Matrix3d R);

/**
 * @brief Convert from rotation matrix to its associated Lie Algebra
 * @param R rotation matrix
 * @return 3x1 vector representing R in Lie Algebra space
 **/
beam::Vec3 RToLieAlgebra(const beam::Mat3 R);

/**
 * @brief Convert from Lie Algebra to its associated rotation matrix
 * @param eps rotation in Lie Algebra space
 * @return rotation matrix
 **/
beam::Mat3 LieAlgebraToR(const beam::Vec3 eps);

/**
 * @brief Linear interpolation of transformations using a method in Tim
 *Barfoot's State Estimation textbook
 * @param m1 first transformation matrix
 * @param m2 second transformation matrix
 * @param t1 time point of first transform
 * @param t2 time point of second transform
 * @param t time point that you want to interpolate at
 * @return interpolated transformation matrix
 **/
beam::Mat4 InterpolateTransform(const beam::Mat4& m1, const beam::TimePoint& t1,
                                const beam::Mat4& m2, const beam::TimePoint& t2,
                                const beam::TimePoint& t);

/**
 * @brief Perform inverse of skew symmetric transform
 * @param M 3x3 skew symmetric matrix
 * @return 3x1 vector
 **/
beam::Vec3 invSkewTransform(const beam::Mat3 M);

/**
 * @brief Perform skew symmetric transform
 * @param V 3x1 vector
 * @return 3x3 skew symmetric matrix
 **/
beam::Mat3 skewTransform(const beam::Vec3 V);

/** @} group utils */
} // namespace beam

#endif // BEAM_UTILS_MATH_HPP