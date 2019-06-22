#pragma once

namespace nft {
namespace rng {

class IEnergyDistribution {
public:
  virtual double GetRandomEnergy() const = 0;
  virtual ~IEnergyDistribution(){}
};

} // namespace utils
} // namespace nft
