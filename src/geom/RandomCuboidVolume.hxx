#pragma once

#include "geom/IRandomVectors.hxx"

#include "TRandom3.h"
#include "TVector3.h"

namespace nft {
namespace geom {
class RandomCuboidVolume : public IPostionDistribution {
  TVector3 fTopLeft;
  TVector3 fToBottomRight;
  bool fHasExtent;

public:
  RandomCuboidVolume(TVector3 Center = TVector3(0, 0, 0),
                     TVector3 XYZHalfWidth = TVector3(0, 0, 0))
      : fToBottomRight(0, 0, 0) {
    fTopLeft[0] = Center[0] - fabs(XYZHalfWidth[0]);
    fTopLeft[1] = Center[1] - fabs(XYZHalfWidth[1]);
    fTopLeft[2] = Center[2] - fabs(XYZHalfWidth[2]);
    fHasExtent = (XYZHalfWidth.Mag() > 0);
    if (fHasExtent) {
      fToBottomRight[0] = 2 * fabs(XYZHalfWidth[0]);
      fToBottomRight[1] = 2 * fabs(XYZHalfWidth[1]);
      fToBottomRight[2] = 2 * fabs(XYZHalfWidth[2]);
    }
  }

  TVector3 GetRandomPosition() const {
    if (!fHasExtent) {
      return fTopLeft;
    }
    return fTopLeft + TVector3(fToBottomRight[0] * gRandom->Uniform(),
                               fToBottomRight[1] * gRandom->Uniform(),
                               fToBottomRight[2] * gRandom->Uniform());
  }
};
} // namespace geom
} // namespace nft
