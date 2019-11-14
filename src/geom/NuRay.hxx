#pragma once

#include "utils/Particle.hxx"

namespace nft {
namespace geom {
struct NuRay : public utils::Particle {
  NuRay() : utils::Particle(), fWeight_cm2(1) {}
  NuRay(utils::Particle const &part) : utils::Particle(part), fWeight_cm2(1) {}
  NuRay(utils::Particle const &part, double wgt)
      : utils::Particle(part), fWeight_cm2(wgt) {}
  NuRay(utils::Particle &&part)
      : utils::Particle(std::move(part)), fWeight_cm2(1) {}
  NuRay(utils::Particle &&part, double wgt)
      : utils::Particle(std::move(part)), fWeight_cm2(wgt) {}
  // All non POT weights.
  double fWeight_cm2;
};
} // namespace geom
} // namespace nft
