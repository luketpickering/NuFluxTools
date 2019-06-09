#pragma once

#include "exception/exception.hxx"

#include "TRandom3.h"
#include "TVector3.h"

#include <iostream>

// #define DEBUG_UTILS_RANDOM_TOOLS

namespace nft {
namespace utils {
inline double
ThrowWeightedDistance(std::vector<std::pair<double, double>> const &path) {
  if (!path.back().second) {
    return 0;
  }
  double density_weighted_distance = gRandom->Uniform() * path.back().second;
#ifdef DEBUG_UTILS_RANDOM_TOOLS
  std::cout << "Max: " << path.back().second << std::endl;
  std::cout << "Thrown: " << density_weighted_distance << std::endl;
#endif

  // Should binary search
  size_t Npp = path.size();
  for (size_t i = 0; i < Npp; ++i) {
#ifdef DEBUG_UTILS_RANDOM_TOOLS
    std::cout << "\tseg: " << i << ", end: " << path[i].second << std::endl;
#endif
    if (density_weighted_distance < path[i].second) {
      double last = path[i - 1].second;
#ifdef DEBUG_UTILS_RANDOM_TOOLS
      std::cout << "\t\t"
                << "last " << last << std::endl;
#endif

      double this_less_last = path[i].second - last;
      double throw_less_last = density_weighted_distance - last;
#ifdef DEBUG_UTILS_RANDOM_TOOLS
      std::cout << "\t\t"
                << "this_less_last " << this_less_last << std::endl;
      std::cout << "\t\t"
                << "throw_less_last " << throw_less_last << std::endl;

      std::cout << "\t\t"
                << "last_m " << path[i - 1].first << std::endl;
      std::cout << "\t\t"
                << "frac_weight " << (throw_less_last / this_less_last)
                << std::endl;
      std::cout << "\t\t"
                << "seg_m " << (path[i].first - path[i - 1].first) << std::endl;
      std::cout << "\t\t"
                << "choose_m "
                << (path[i - 1].first + (throw_less_last / this_less_last) *
                                            (path[i].first - path[i - 1].first))
                << std::endl;
#endif

      return (path[i - 1].first + (throw_less_last / this_less_last) *
                                      (path[i].first - path[i - 1].first));
    }
  }
  throw internal_error() << "[ERROR]: Threw random distance "
                         << density_weighted_distance
                         << " but expected maximum to be "
                         << path.back().second;
}

inline TVector3
ThrowWeightedPosition(TVector3 const &Pos, TVector3 const &Dir,
                      std::vector<std::pair<double, double>> const &path) {
  double NDir = ThrowWeightedDistance(path);
  return Pos + NDir * Dir.Unit();
}

} // namespace utils
} // namespace nft
