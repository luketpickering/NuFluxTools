#pragma once

#include "Math/Point3D.h"
#include "Math/Vector3D.h"

#include <ostream>

namespace nft {
namespace utils {

inline std::ostream &operator<<(std::ostream &os,
                                ROOT::Math::XYZPoint const &p) {
  return os << "[ X: " << p.X() << ", Y: " << p.Y() << ", Z: " << p.Z() << " ]";
}

inline std::ostream &operator<<(std::ostream &os,
                                ROOT::Math::XYZVector const &v) {
  return os << "[ X: " << v.X() << ", Y: " << v.Y() << ", Z: " << v.Z() << " ]";
}

template <typename T, size_t N>
inline std::ostream &operator<<(std::ostream &os, std::array<T, N> const &a) {
  os << "[";

  if (N == 0) {
    return os << "]";
  }

  for (size_t i = 0; i < (N - 1); ++i) {
    os << a[i] << ", ";
  }
  return os << a.back() << "]";
}

} // namespace utils
} // namespace nft
