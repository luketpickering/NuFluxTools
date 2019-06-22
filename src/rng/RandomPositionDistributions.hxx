#pragma once

#include "Math/Vector2D.h"
#include "geom/BoundingCuboid.hxx"

#include "rng/IRandomVectors.hxx"

#include "TRandom3.h"

namespace nft {
namespace rng {

// Random axis-aligned cuboid volume
class RandomCuboidVolume : public IPostionDistribution {
  geom::BoundingCuboid fBB;
  bool fHasExtent[4];

public:
  template <typename PosVect = ROOT::Math::XYZPoint,
            typename Rotation = ROOT::Math::Rotation3D>
  RandomCuboidVolume(PosVect const &Center = ROOT::Math::XYZPoint(0, 0, 0),
                     ROOT::Math::XYZVector const &XYZHalfWidth =
                         ROOT::Math::XYZVector(0, 0, 0),
                     Rotation const &MasterZToLocalZ = ROOT::Math::Rotation3D())
      : fBB(Center, XYZHalfWidth, MasterZToLocalZ) {

    fHasExtent[0] = (fBB.fLocalTopFwdLeftToBottomBkwdRight.X() > 0);
    fHasExtent[1] = (fBB.fLocalTopFwdLeftToBottomBkwdRight.Y() > 0);
    fHasExtent[2] = (fBB.fLocalTopFwdLeftToBottomBkwdRight.Z() > 0);
    // If we have no extent, just return the position each time
    fHasExtent[3] = (fBB.fLocalTopFwdLeftToBottomBkwdRight.Mag2() > 0);
  }

  ROOT::Math::XYZPoint GetRandomPosition() const {
    if (!fHasExtent[3]) {
      return fBB.fMasterCenter;
    }
    return fBB.LocalToMaster(
        fBB.fLocalBottomBkwdRight +
        ROOT::Math::XYZVector(
            fHasExtent[0]
                ? fBB.fLocalTopFwdLeftToBottomBkwdRight.X() * gRandom->Uniform()
                : 0,
            fHasExtent[1]
                ? fBB.fLocalTopFwdLeftToBottomBkwdRight.Y() * gRandom->Uniform()
                : 0,
            fHasExtent[2]
                ? fBB.fLocalTopFwdLeftToBottomBkwdRight.Z() * gRandom->Uniform()
                : 0));
  }
};

class RandomAACuboidVolume : public RandomCuboidVolume {
public:
  template <typename PosVect = ROOT::Math::XYZPoint>
  RandomAACuboidVolume(PosVect const &Center = ROOT::Math::XYZPoint(0, 0, 0),
                       ROOT::Math::XYZVector const &XYZHalfWidth =
                           ROOT::Math::XYZVector(0, 0, 0))
      : RandomCuboidVolume(Center, XYZHalfWidth) {}
};

class RandomXYRectangularPlane : public RandomAACuboidVolume {
public:
  template <typename PosVect = ROOT::Math::XYZPoint>
  RandomXYRectangularPlane(
      PosVect const &Center = ROOT::Math::XYZPoint(0, 0),
      ROOT::Math::XYVector const &XYHalfWidth = ROOT::Math::XYVector(0, 0))
      : RandomAACuboidVolume(
            ROOT::Math::XYZPoint(Center),
            ROOT::Math::XYZVector(XYHalfWidth.X(), XYHalfWidth.Y(), 0)) {}
};

} // namespace rng
} // namespace nft
