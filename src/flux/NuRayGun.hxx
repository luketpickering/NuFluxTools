#pragma once

#include "flux/INuRaySource.hxx"

#include "rng/IRandomVectors.hxx"
#include "rng/RandomPositionDirections.hxx"
#include "rng/UniformEnergyDistribution.hxx"

#include "utils/PDGUtils.hxx"

#include <memory>

namespace nft {
namespace flux {
class NuRayGun : public INuRaySource {
  PDG_t fNuPDG;

  std::unique_ptr<rng::IEnergyDistribution> fE;

  std::unique_ptr<rng::IPositionDirectionDistribution> fPosDir;

public:
  NuRayGun(nft::PDG_t NuPDG,
           std::unique_ptr<rng::IEnergyDistribution> &&E =
               std::make_unique<rng::UniformEnergyDistribution>(1, 1),
           std::unique_ptr<rng::IPositionDirectionDistribution> &&PosDir =
               std::make_unique<rng::RandomAACuboidCone>()) {
    fNuPDG = NuPDG;

    fE = std::move(E);

    fPosDir = std::move(PosDir);
  }

  geom::NuRay Shoot() {
    geom::NuRay nu;
    rng::posdir_t pd = fPosDir->GetRandomPositionDirection();
    nu.fThreePos_lab =
        ROOT::Math::XYZPoint(pd.Position.X(), pd.Position.Y(), pd.Position.Z());
    double E = fE->GetRandomEnergy();
    pd.Direction.SetR(E);
    nu.fFourMom_lab = ROOT::Math::XYZTVector(pd.Direction.X(), pd.Direction.Y(),
                                             pd.Direction.Z(), E);
    nu.fPDG = fNuPDG;
    return nu;
  }
};
} // namespace flux
} // namespace nft
