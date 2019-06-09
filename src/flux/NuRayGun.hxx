#pragma once

#include "flux/INuRaySource.hxx"

#include "geom/IRandomVectors.hxx"
#include "geom/RandomConeDirection.hxx"
#include "geom/RandomCuboidVolume.hxx"

#include "utils/PDGUtils.hxx"
#include "utils/UniformEnergyDistribution.hxx"

#include <memory>

namespace nft {
namespace flux {
class NuRayGun : public INuRaySource {
  PDG_t fNuPDG;

  std::unique_ptr<utils::IEnergyDistribution> fE;

  std::unique_ptr<geom::IPostionDistribution> fOrigin;
  std::unique_ptr<geom::IDirectionDistribution> fDirection;

public:
  NuRayGun(nft::PDG_t NuPDG,
           std::unique_ptr<utils::IEnergyDistribution> &&E =
               std::make_unique<utils::UniformEnergyDistribution>(1, 1),
           std::unique_ptr<geom::IPostionDistribution> &&Origin =
               std::make_unique<geom::RandomCuboidVolume>(),
           std::unique_ptr<geom::IDirectionDistribution> &&Direction =
               std::make_unique<geom::RandomConeDirection>()) {
    fNuPDG = NuPDG;

    fE = std::move(E);

    fOrigin = std::move(Origin);
    fDirection = std::move(Direction);
  }

  geom::NuRay Shoot() {
    geom::NuRay nu;
    nu.fFourPos_lab = TLorentzVector(fOrigin->GetRandomPosition(), 0);
    TVector3 Dir = fDirection->GetRandomDirection();
    double E = fE->GetRandomEnergy();
    Dir.SetMag(E);
    nu.fFourMom_lab = TLorentzVector(Dir, E);
    nu.fPDG = fNuPDG;
    return nu;
  }
};
} // namespace flux
} // namespace nft
