#pragma once

namespace nft {
namespace utils {

class IEnergyDistribution {
public:
  virtual double GetRandomEnergy() const = 0;
  virtual ~IEnergyDistribution(){}
};

} // namespace utils
} // namespace nft
