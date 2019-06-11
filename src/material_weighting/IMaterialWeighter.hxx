#pragma once

#include "geom/Material.hxx"

#include "utils/PDGUtils.hxx"

namespace nft {
namespace mw {
class IMaterialWeighter {
public:

  /// Used for material weighters for which it may be more efficient to provide
  /// a single instance that can provide weights for any material;
  virtual double GetMaterialWeight(geom::Material const &, double Enu,
                           PDG_t pdg_nu) const = 0;
};
} // namespace mw
} // namespace nft
