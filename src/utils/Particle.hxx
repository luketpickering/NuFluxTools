#pragma once

#include "utils/PDGUtils.hxx"

#include "Math/Vector4D.h"
#include "Math/Point3D.h"

namespace nft {
namespace utils {
struct Particle {
  ROOT::Math::XYZPoint fThreePos_lab;
  ROOT::Math::PxPyPzEVector fFourMom_lab;
  PDG_t fPDG;
  double Mass() { return fFourMom_lab.M(); }
  double GammaFactor() { return fFourMom_lab.Gamma(); }
  double BetaFactor() { return fFourMom_lab.Beta(); }
};
} // namespace utils
} // namespace nft
