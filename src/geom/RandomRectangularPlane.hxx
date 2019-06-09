#pragma once

#include "geom/IRandomVectors.hxx"

#include "TRandom3.h"
#include "TVector2.h"
#include "TVector3.h"

namespace nft {
namespace geom {
class RandomRectangularPlane : public IPostionDistribution {
  TVector3 fTopLeft;
  TVector2 fToBottomRight;
  bool fHasExtent;

public:
  RandomRectangularPlane(TVector3 Center, TVector2 XYHalfWidth) : fToBottomRight(0,0) {
    fTopLeft[0] = Center[0] - fabs(XYHalfWidth[0]);
    fTopLeft[1] = Center[1] - fabs(XYHalfWidth[1]);
    fTopLeft[2] = Center[2];

    fHasExtent = (XYHalfWidth.Mag() > 0);
    if (fHasExtent) {
      fToBottomRight[0] = 2 * fabs(XYHalfWidth[0]);
      fToBottomRight[1] = 2 * fabs(XYHalfWidth[1]);
    }
  }

  /// Get a random point in the plane
  TVector3 GetRandomPosition() const {
    if (!fHasExtent) {
      return fTopLeft;
    }
    return fTopLeft + TVector3(fToBottomRight[0] * gRandom->Uniform(),
                               fToBottomRight[1] * gRandom->Uniform(), 0);
  }
};
} // namespace geom
} // namespace nft
