#pragma once

#include "utils/PDGUtils.hxx"

#include "TLorentzVector.h"

namespace nft {
namespace utils {
struct Particle {
  TLorentzVector fFourPos_lab;
  TLorentzVector fFourMom_lab;
  PDG_t fPDG;
  double Mass() { return fFourMom_lab.M(); }
  double GammaFactor() { return fFourMom_lab.Gamma(); }
  double BetaFactor() { return fFourMom_lab.Beta(); }
};
} // namespace utils
} // namespace nft
