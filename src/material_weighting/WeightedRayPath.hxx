#pragma once

#include "exception/exception.hxx"

#include "utils/PDGUtils.hxx"

#include "TRandom3.h"
#include "TVector3.h"

#include <iostream>

// #define WeightedRayPath_DEBUG

namespace nft {
namespace mw {
class WeightedRayPath {
  struct path_seg {
    double length;
    double weighted_length;

    path_seg operator+(path_seg const &r) {
      return path_seg{this->length + r.length,
                      this->weighted_length + r.weighted_length};
    }
  };

  std::vector<path_seg> fPath;
  TVector3 fStart;
  TVector3 fDirection;

public:
  WeightedRayPath(TVector3 start, TVector3 const &direction) {
    fPath.push_back(path_seg{0, 0});
    fStart = std::move(start);
    fDirection = direction.Unit();
  }

  void clear() {
    fPath.clear();
    fPath.push_back(path_seg{0, 0});
  }
  void reserve(size_t N) { fPath.reserve(N); }

  void AddSegment(double length, double weight) {
    fPath.push_back(fPath.back() + path_seg{length, length * weight});
  }

  double GetPathWeight() const { return fPath.back().weighted_length; }

  TVector3 ChoosePosition() {
    if (!GetPathWeight()) {
      return fStart;
    }
    double chosen_weighted_distance = gRandom->Uniform() * GetPathWeight();

#ifdef WeightedRayPath_DEBUG
    std::cout << "[WRP]: Chose weighted distance " << chosen_weighted_distance
              << "/" << GetPathWeight() << std::endl;
#endif

    // Should binary search
    size_t Npp = fPath.size();
    for (size_t seg_it = 0; seg_it < Npp; ++seg_it) {

#ifdef WeightedRayPath_DEBUG
      std::cout << "[WRP]: \tSeg weighted end " << fPath[seg_it].weighted_length
                << ", leg end: " << fPath[seg_it].length << std::endl;
#endif

      if (chosen_weighted_distance < fPath[seg_it].weighted_length) {
        double seg_low_edge = fPath[seg_it - 1].weighted_length;

        double seg_weighted_length =
            fPath[seg_it].weighted_length - seg_low_edge;
        double sub_seg_weighted_length =
            chosen_weighted_distance - seg_low_edge;
        double seg_fraction = sub_seg_weighted_length / seg_weighted_length;

        double prev_seg_end = fPath[seg_it - 1].length;
        double seg_length = fPath[seg_it].length - prev_seg_end;

#ifdef WeightedRayPath_DEBUG
        std::cout << "[WRP]: \tSeg fraction:  " << seg_fraction
                  << ", seg length = " << seg_length << std::endl;
#endif

        double NDir = (prev_seg_end + seg_fraction * seg_length);

        return fStart + NDir * fDirection;
      }
    }
    throw internal_error() << "[ERROR]: Threw random distance "
                           << chosen_weighted_distance
                           << " but expected maximum to be "
                           << fPath.back().weighted_length;
  }
};
} // namespace mw
} // namespace nft
