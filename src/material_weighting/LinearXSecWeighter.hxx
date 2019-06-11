#pragma once

#include "material_weighting/IMaterialWeighter.hxx"

namespace nft {
namespace mw {
class LinearXSecWeighter : public IMaterialWeighter {
  double ZScaling;
  double NScaling;
  double EnergyScaling;
  double EMin;

public:
  /// A simple linear cross-section provider that can be used for interaction
  /// position determination
  LinearXSecWeighter(double emin = 0.2, double XSec_E_Gradient = 1E-3,
                     double zscaling = 1, double nscaling = 1) {
    EnergyScaling = XSec_E_Gradient;
    EMin = emin;
    ZScaling = zscaling;
    NScaling = nscaling;
  }

  double GetMaterialWeight(geom::Material const &mat, double Enu, PDG_t) const {
    if (Enu < EMin) {
      return 0;
    }

    double scaled_grad = EnergyScaling * (ZScaling * mat.avg_Z +
                                          NScaling * (mat.avg_A - mat.avg_Z)) * mat.density;

    // y = m*x + c, c = y(0) - m*x(EGradient*EMin)
    double XSecIntersect = -EMin * scaled_grad;

    return Enu * scaled_grad + XSecIntersect;
  }
};
} // namespace mw
} // namespace nft
