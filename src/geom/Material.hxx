#pragma once

#include "TGeoMaterial.h"

namespace nft {
namespace geom {

// Generalized material abstraction for components that do not need to interact
// directly with the TGeoMaterial instance;
struct Material {
  size_t avg_A;
  size_t avg_Z;

  double density;

  TGeoMaterial *fMat;

  Material(TGeoMaterial *mat) {
    fMat = mat;
    avg_A = fMat->GetA();
    avg_Z = fMat->GetZ();
    density = fMat->GetDensity();
  }
};
} // namespace geom
} // namespace nft
