#pragma once

#include "utils/IEnergyDistribution.hxx"

#include "TRandom3.h"

#include <utility>

namespace nft {
namespace utils {

class UniformEnergyDistribution : public IEnergyDistribution {
  std::pair<double, double> fERange;
  bool fMonoE;

public:
  UniformEnergyDistribution(double min, double max) {
    fERange = std::make_pair(std::min(min, max), std::max(min, max));
    fERange = std::make_pair(fERange.first, fERange.second - fERange.first);
    fMonoE = (fERange.second == 0);
  }

  double GetRandomEnergy() const {
    double E = fERange.first;
    if (!fMonoE) {
      E += (gRandom->Uniform() * fERange.second);
    }
    return E;
  }
};

} // namespace utils
} // namespace nft
