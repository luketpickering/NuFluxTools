#pragma once

#include "utils/PDGUtils.hxx"
#include "utils/Particle.hxx"

#include "geom/NuRay.hxx"

#include "Math/Point3D.h"
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"
#include "Math/Transform3D.h"

namespace nft {
namespace flux {
/// Minimal information required to make flux/event rate prediction from a beam
/// simulation
struct DecayParent : public utils::Particle {
  double fENu_com;
  PDG_t fDecayTo;

  // NPOT this neutrino represents
  double fPOT;
  // Weight to take flux prediction to flux/cm^2
  double fWindowWeight_cm2;
  // Any other MC weights (such as beamsim importance weights)
  double fMCWeight;

  //** Required to properly decay muons in flight
  // Four momentum of DecayParent when it was produced.
  ROOT::Math::PxPyPzEVector fMomAtProduction;
  // DecayParentDecayParent momentum
  ROOT::Math::PxPyPzEVector fParentMomAtDecay;

  geom::NuRay Shoot(ROOT::Math::XYZPoint const &) const;

  DecayParent CoordTransform(ROOT::Math::Transform3D const &t) {
    DecayParent rtn = *this;

    rtn.fThreePos_lab = t(this->fThreePos_lab);
    rtn.fFourMom_lab = t(this->fFourMom_lab);

    rtn.fMomAtProduction = t(this->fMomAtProduction);
    rtn.fParentMomAtDecay = t(this->fParentMomAtDecay);

    return rtn;
  }
};
} // namespace flux
} // namespace nft
