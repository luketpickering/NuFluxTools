#pragma once

#include "geom/IRandomVectors.hxx"

#include "TRandom3.h"
#include "TRotation.h"
#include "TVector3.h"

namespace nft {
namespace geom {
class RandomConeDirection : public IDirectionDistribution {
  TVector3 fCenterAxis;
  TVector3 fCenterAxis_UZ;
  double fCosThetaUniformRange;
  bool fHasExtent;
  bool fNotZ;
  TRotation ZToUZ;

public:
  RandomConeDirection(TVector3 Center = TVector3(0, 0, 1),
                      double HalfOpeningAngle_rads = 0)
      : fCosThetaUniformRange(0) {
    fCenterAxis = Center.Unit();
    fNotZ = (fCenterAxis[2] != 1);
    if (fNotZ) {
      ZToUZ.SetZAxis(fCenterAxis);
    }
    fHasExtent = (HalfOpeningAngle_rads > 0);
    if (fHasExtent) {
      double abs_open = fabs(HalfOpeningAngle_rads);
      double ztpi_open = abs_open > M_PI ? fmod(abs_open, M_PI) : abs_open;
      fCosThetaUniformRange = 1 - cos(abs_open);
    }
  }

  TVector3 GetRandomDirection() const {
    if (!fHasExtent) {
      return fCenterAxis;
    }
    TVector3 RandomAboutZ;

    double theta = acos(1 - (gRandom->Uniform() * fCosThetaUniformRange));
    double phi = gRandom->Uniform() * 2 * M_PI;
    RandomAboutZ.SetMagThetaPhi(1, theta, phi);

    return fNotZ ? ZToUZ * RandomAboutZ : RandomAboutZ;
  }
};
} // namespace geom
} // namespace nft
