#pragma once

#include "Math/Point3D.h"
#include "Math/Vector3D.h"

namespace nft {
namespace rng {
class IDirectionDistribution {
public:
  virtual ROOT::Math::Polar3DVector GetRandomDirection() const = 0;
  virtual ~IDirectionDistribution() {}
};

class IPostionDistribution {
public:
  virtual ROOT::Math::XYZPoint GetRandomPosition() const = 0;
  virtual ~IPostionDistribution() {}
};

struct posdir_t {
  ROOT::Math::XYZPoint Position;
  ROOT::Math::Polar3DVector Direction;
};

class IPositionDirectionDistribution {
public:
  virtual posdir_t GetRandomPositionDirection() const = 0;
};

} // namespace geom
} // namespace nft
