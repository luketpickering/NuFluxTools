#pragma once

#include "utils/Particle.hxx"
#include "utils/PDGUtils.hxx"

#include "TVector3.h"

namespace nft {
namespace flux {
/// Minimal information required to make flux/event rate prediction from a beam
/// simulation
struct DecayParent : public utils::Particle {
  double fENu_com;
  PDG_t fDecayTo;

  double fPOTWeight_cm2;
  double fMCWeight;

  utils::Particle Shoot(TVector3 const &);
};
} // namespace flux
} // namespace nft
