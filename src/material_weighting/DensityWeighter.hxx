#pragma once

#include "material_weighting/IMaterialWeighter.hxx"

namespace nft {
namespace mw {
class DensityWeighter : public IMaterialWeighter {
public:
  double GetMaterialWeight(geom::Material const &mat, double Enu, PDG_t) const {
    return mat.density;
  }
};
} // namespace mw
} // namespace nft
