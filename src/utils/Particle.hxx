#pragma once

#include "utils/PDGUtils.hxx"

#include "Math/Vector4D.h"

namespace nft {
namespace utils {
struct Particle {
  ROOT::Math::XYZTVector fFourPos_lab;
  ROOT::Math::XYZTVector fFourMom_lab;
  PDG_t fPDG;
  double Mass() { return fFourMom_lab.M(); }
  double GammaFactor() { return fFourMom_lab.Gamma(); }
  double BetaFactor() { return fFourMom_lab.Beta(); }
};
} // namespace utils
} // namespace nft
