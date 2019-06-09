#pragma once

#include "geom/NuRay.hxx"

namespace nft {
namespace flux {
class INuRaySource {
public:
  virtual geom::NuRay Shoot() = 0;
  virtual ~INuRaySource(){}
};
} // namespace flux
} // namespace nft
