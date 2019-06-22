#pragma once

#include "Math/Point3D.h"
#include "Math/Transform3D.h"
#include "Math/Vector3D.h"

// #define DEBUG_BOUNDINGCUBOID

#ifdef DEBUG_BOUNDINGCUBOID
#include "utils/T2Str.hxx"
using namespace nft::utils;
#endif

namespace nft {
namespace geom {

struct BoundingCuboid {
  ROOT::Math::Transform3D fM2L;
  ROOT::Math::Transform3D fL2M;

  ROOT::Math::XYZPoint fMasterCenter;
  ROOT::Math::XYZPoint fLocalTopFwdLeft;
  ROOT::Math::XYZPoint fLocalBottomBkwdRight;
  ROOT::Math::XYZVector fLocalTopFwdLeftToBottomBkwdRight;

  /// Instantiate with the center point in 'master' coordinates, and the
  /// half-widths in local coordinates where the cuboid is axis aligned.
  template <typename PosVect, typename Rotation = ROOT::Math::Rotation3D>
  BoundingCuboid(PosVect const &Center,
                 ROOT::Math::XYZVector const &XYZHalfWidth,
                 Rotation const &MasterZToLocalZ = ROOT::Math::Rotation3D()) {

    fM2L = ROOT::Math::Transform3D(
        MasterZToLocalZ,
        ROOT::Math::Translation3D(-ROOT::Math::XYZVector(Center)));
    fL2M = fM2L.Inverse();

    fMasterCenter = Center;

    std::array<double, 3> half_widths;
    XYZHalfWidth.GetCoordinates(half_widths.begin(), half_widths.end());
    for (auto &d : half_widths) {
      d = 2 * fabs(d);
    }
    fLocalTopFwdLeftToBottomBkwdRight.SetCoordinates(half_widths.begin(),
                                                     half_widths.end());

    fLocalTopFwdLeft = 0.5 * fLocalTopFwdLeftToBottomBkwdRight;
    fLocalBottomBkwdRight = -0.5 * fLocalTopFwdLeftToBottomBkwdRight;

#ifdef DEBUG_BOUNDINGCUBOID
    std::cout << "[BC]: Center = " << Center << std::endl;
    std::cout << "[BC]: XYZHalfWidth = " << XYZHalfWidth << std::endl;
    std::cout << "[BC]: fMasterCenter = " << fMasterCenter << std::endl;
    std::cout << "[BC]: fLocalTopFwdLeft = " << fLocalTopFwdLeft << std::endl;
    std::cout << "[BC]: fLocalBottomBkwdRight = " << fLocalBottomBkwdRight
              << std::endl;
    std::cout << "[BC]: fLocalTopFwdLeftToBottomBkwdRight = "
              << fLocalTopFwdLeftToBottomBkwdRight << std::endl;
#endif
  }

  bool InsideLocal(ROOT::Math::XYZPoint const &local_pos) const {

    bool gt = true;
    if (local_pos.X() >= fLocalTopFwdLeft.X()) {
      gt = false;
    }
    if (local_pos.Y() >= fLocalTopFwdLeft.Y()) {
      gt = false;
    }
    if (local_pos.Z() >= fLocalTopFwdLeft.Z()) {
      gt = false;
    }

    bool lt = true;
    if (local_pos.X() <= fLocalBottomBkwdRight.X()) {
      lt = false;
    }
    if (local_pos.Y() <= fLocalBottomBkwdRight.Y()) {
      lt = false;
    }
    if (local_pos.Z() <= fLocalBottomBkwdRight.Z()) {
      lt = false;
    }

    return (gt && lt);
  }

  template <typename Vect> Vect MasterToLocal(Vect const &master) const {
    return fM2L * master;
  }
  template <typename Vect> Vect LocalToMaster(Vect const &local) const {
    return fL2M * local;
  }

  template <typename PosVect> bool Inside(PosVect const &master) const {
    return InsideLocal(MasterToLocal(master));
  }
};

} // namespace geom
} // namespace nft
