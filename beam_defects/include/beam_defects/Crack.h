/** @file
 * @ingroup defects
 */

#pragma once
#include "beam_defects/Defect.h"

namespace beam_defects {
/** @addtogroup defects
 *  @{ */

/**
 * @brief Derived class for crack defects
 */
class Crack : public Defect {
public:
  // Inherit base class constructors
  using Defect::Defect;

  /**
   * @brief Default constructor
   */
  ~Crack() override = default;

  /**
   * @brief Get the type of defect
   * @return Returns type as one of defects specified in the enum DefectType
   */
  DefectType GetType() const override { return DefectType::CRACK; };

  /**
   * @brief Get the size of the defect
   * @return Returns the size of the defect in whatever units are relevant for
   * this class of defect
   */
  double GetSize() override;

  DefectOSIMSeverity GetOSIMSeverity() override;

private:
  /**
   * @brief Method for calculating the size of the defect
   * @return Returns the size
   */
  double CalculateSize();

  double crack_size_ = 0; // Variable to store crack size
};

/** @} group defects */
} // namespace beam_defects
