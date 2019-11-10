#pragma once

#include "Math/RotationZYX.h"
#include "Math/Transform3D.h"

#include "fhiclcpp/ParameterSet.h"

namespace nft {
namespace geom {

inline ROOT::Math::Transform3D
BuildTransformation(fhicl::ParameterSet const &ps) {

  std::vector<fhicl::ParameterSet> Rotations =
      ps.get<std::vector<fhicl::ParameterSet>>("rotations", {});

  ROOT::Math::Rotation3D rot;

  for (auto const &r : Rotations) {
    double z_deg = r.get<double>("z_deg", 0);
    double y_deg = r.get<double>("y_deg", 0);
    double x_deg = r.get<double>("x_deg", 0);

    rot *= ROOT::Math::RotationZYX(z_deg * M_PI / 180.0, y_deg * M_PI / 180.0,
                                   x_deg * M_PI / 180.0);
  }

  fhicl::ParameterSet translation =
      ps.get<fhicl::ParameterSet>("translation",{});
  double z_cm = translation.get<double>("z_cm", 0);
  double y_cm = translation.get<double>("y_cm", 0);
  double x_cm = translation.get<double>("x_cm", 0);

  return ROOT::Math::Transform3D(rot,
                                 ROOT::Math::Translation3D{x_cm, y_cm, z_cm});
}

} // namespace geom
} // namespace nft
