#pragma once

#include "rng/IRandomVectors.hxx"
#include "rng/RandomDirectionDistributions.hxx"
#include "rng/RandomPositionDistributions.hxx"

namespace nft {
namespace rng {
class RandomUncorrelatedPositionDirection
    : public IPositionDirectionDistribution {

  std::unique_ptr<IPostionDistribution> fPositionDistribution;
  std::unique_ptr<IDirectionDistribution> fDirectionDistribution;

public:
  RandomUncorrelatedPositionDirection(
      std::unique_ptr<IPostionDistribution> &&PositionDistribution,
      std::unique_ptr<IDirectionDistribution> &&DirectionDistribution) {
    fPositionDistribution = std::move(PositionDistribution);
    fDirectionDistribution = std::move(DirectionDistribution);
  }

  posdir_t GetRandomPositionDirection() const {
    return {fPositionDistribution->GetRandomPosition(),
            fDirectionDistribution->GetRandomDirection()};
  }
};

class RandomAACuboidCone : public IPositionDirectionDistribution {

  RandomAACuboidVolume fPositionDistribution;
  RandomConeDirection fDirectionDistribution;

public:
  template <typename PosVect = ROOT::Math::XYZPoint,
            typename DirVect = ROOT::Math::Polar3DVector>
  RandomAACuboidCone(
      PosVect const &VolumeCenter = ROOT::Math::XYZPoint(0, 0, 0),
      ROOT::Math::XYZVector const &XYZHalfWidth = ROOT::Math::XYZVector(0, 0,
                                                                        0),
      DirVect const &ConeAxis = ROOT::Math::Polar3DVector(1, 0, 0),
      double HalfOpeningAngle_rads = 0)
      : fPositionDistribution(VolumeCenter, XYZHalfWidth),
        fDirectionDistribution(ConeAxis, HalfOpeningAngle_rads) {}

  posdir_t GetRandomPositionDirection() const {
    return {fPositionDistribution.GetRandomPosition(),
            fDirectionDistribution.GetRandomDirection()};
  }
};

} // namespace rng
} // namespace nft
