#pragma once

#include "rng/IRandomVectors.hxx"

#include "Math/Point3D.h"
#include "Math/Vector3D.h"

#include "TRandom3.h"

#include "fhiclcpp/ParameterSet.h"

namespace nft {
namespace geom {
class FluxWindow : public rng::IPostionDistribution {

public:
  ROOT::Math::XYZPoint fBottomLeft;
  ROOT::Math::XYZVector fToTopLeft;
  ROOT::Math::XYZVector fToBottomRight;

  ROOT::Math::XYZPoint fCenter;
  ROOT::Math::XYZVector fUnitNormal;

  FluxWindow() {}
  FluxWindow(fhicl::ParameterSet const &ps) {

    std::array<double, 3> blarr =
        ps.get<std::array<double, 3>>("bottom_left_cm");
    fBottomLeft = ROOT::Math::XYZPoint{blarr[0], blarr[1], blarr[2]};
    std::array<double, 3> totlarr =
        ps.get<std::array<double, 3>>("bottom_left_to_top_left_cm");
    fToTopLeft = ROOT::Math::XYZVector{totlarr[0], totlarr[1], totlarr[2]};
    std::array<double, 3> tobrarr =
        ps.get<std::array<double, 3>>("bottom_left_to_bottom_right_cm");
    fToBottomRight = ROOT::Math::XYZVector{tobrarr[0], tobrarr[1], tobrarr[2]};

    ROOT::Math::XYZPoint fTopRight = fBottomLeft + fToTopLeft + fToBottomRight;

    fCenter = fBottomLeft + 0.5 * (fTopRight - fBottomLeft);
    fUnitNormal = fToTopLeft.Cross(fToBottomRight).Unit();
  }

  ROOT::Math::XYZPoint GetRandomPosition() const {
    return fBottomLeft + gRandom->Uniform(1) * fToTopLeft +
           gRandom->Uniform(1) * fToBottomRight;
  }
};
} // namespace geom
} // namespace nft
