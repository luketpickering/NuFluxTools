#pragma once

#include "TVector3.h"

namespace nft {
namespace geom {
class IDirectionDistribution {
public:
  virtual TVector3 GetRandomDirection() const = 0;
  virtual ~IDirectionDistribution() {}
};
class IPostionDistribution {
public:
  virtual TVector3 GetRandomPosition() const = 0;
  virtual ~IPostionDistribution() {}
};
} // namespace geom
} // namespace nft
