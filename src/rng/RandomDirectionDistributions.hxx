#pragma once

#include "rng/IRandomVectors.hxx"

#include "TRandom3.h"
#include "Math/AxisAngle.h"

namespace nft {
namespace rng {
class RandomConeDirection : public IDirectionDistribution {
  ROOT::Math::Polar3DVector fCenterAxis;
  double fCosThetaUniformRange;
  bool fHasExtent;

public:
  template <typename DirVect = ROOT::Math::XYZVector>
  RandomConeDirection(DirVect const &Center = ROOT::Math::XYZVector(0, 0, 1),
                      double HalfOpeningAngle_rads = 0)
      : fCosThetaUniformRange(0) {
    fCenterAxis = Center.Unit();
    fHasExtent = (HalfOpeningAngle_rads > 0);
    if (fHasExtent) {
      double abs_open = fabs(HalfOpeningAngle_rads);
      double ztpi_open = abs_open > M_PI ? fmod(abs_open, M_PI) : abs_open;
      fCosThetaUniformRange = 1 - cos(abs_open);
    }
  }

  ROOT::Math::Polar3DVector GetRandomDirection() const {
    if (!fHasExtent) {
      return fCenterAxis;
    }
    ROOT::Math::Polar3DVector rdir = fCenterAxis;

    double dtheta = acos(1 - (gRandom->Uniform() * fCosThetaUniformRange));

    rdir.SetTheta(rdir.Theta() + dtheta);
    double phi = gRandom->Uniform() * 2 * M_PI;
    ROOT::Math::AxisAngle rot(fCenterAxis, phi);

    return (rot * rdir);
  }
};

} // namespace rng
} // namespace nft
