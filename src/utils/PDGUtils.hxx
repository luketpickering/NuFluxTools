#pragma once

#include "exception/exception.hxx"

#include <cstdlib>
#include <cmath>

NEW_NFT_EXCEPT(unsupported_pdg_mass);

namespace nft {
using PDG_t = int;
namespace utils {

static size_t const kGeV = 9;
static size_t const kMeV = 6;

template <size_t U = kGeV> inline double GetMass(PDG_t pdg) {

  static size_t const kDefaultUnits = 9; // in GeV
  static double const pimass = 0.13957;
  static double const kmass = 0.49368;
  static double const k0mass = 0.49767;
  static double const mumass = 0.105658389;

  constexpr double uconv = std::pow(10, U) / std::pow(10, kDefaultUnits);

  switch (pdg) {
  case 211:
  case -211: {
    return pimass * uconv;
  }
  case 321:
  case -321: {
    return kmass * uconv;
  }
  case 311:
  case 310:
  case 130: {
    return k0mass * uconv;
  }
  case 13:
  case -13: {
    return mumass * uconv;
  }
  default: {
    throw unsupported_pdg_mass() << "[ERROR]: Do not know the mass of " << pdg;
  }
  }
}

} // namespace utils
} // namespace nft
