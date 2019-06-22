#pragma once

#include "geom/NuRay.hxx"

#include "material_weighting/DensityWeighter.hxx"
#include "material_weighting/IMaterialWeighter.hxx"
#include "material_weighting/WeightedRayPath.hxx"

#include "TGeoManager.h"
#include "TGeoNavigator.h"
#include "TList.h"

#include <iostream>
#include <map>
#include <memory>

namespace nft {
namespace geom {
class NuRayPropagator {

  size_t const kMaxNullSteps = 1;

  std::map<TGeoMaterial const *, Material> fMaterialMap;
  std::unique_ptr<mw::IMaterialWeighter> fMaterialWeighter;
  NuRay fRay;

  size_t const fSafeStep = 1E6;
  size_t fNSequentialNullSteps;
  size_t fNNonNullSteps;
  size_t fSteps;
  double fStepLength;

  mw::WeightedRayPath fPath;
  ROOT::Math::XYZPoint fPrevPosition;
  TGeoMaterial *fPrevMaterial;

public:
  /// NuRay Propagator
  NuRayPropagator(NuRay ray,
                  std::unique_ptr<mw::IMaterialWeighter> &&material_weighter =
                      std::make_unique<mw::DensityWeighter>(),
                  double step_length = 0.1)
      : fMaterialWeighter(std::move(material_weighter)),
        fStepLength(step_length),
        fPath(ray.fFourPos_lab.Vect(), ray.fFourMom_lab.Vect()) {
    fRay = std::move(ray);

    TList *mat_list = gGeoManager->GetListOfMaterials();
    size_t NMats = mat_list->GetEntries();
    for (size_t m_it = 0; m_it < NMats; ++m_it) {
      TGeoMaterial *mat = dynamic_cast<TGeoMaterial *>(mat_list->At(m_it));
      fMaterialMap.emplace(mat, geom::Material(mat));
    }
  }

  void AddSegment(double step_length) {
    if (fPrevMaterial) { // Add the final segement
      double weight = fMaterialWeighter->GetMaterialWeight(
          fMaterialMap.at(fPrevMaterial), fRay.fFourMom_lab.E(), fRay.fPDG);
      fPath.AddSegment(step_length, weight);
#ifdef TREADLOUDLY
      std::cout << "\t[PATH]: Step " << fSteps << ", Traversed " << step_length
                << " in " << fPrevMaterial->GetName() << " (weight = " << weight
                << ", weighted_step = " << (step_length * weight) << "/"
                << fPath.GetPathWeight() << ")" << std::endl;
#endif
    } else {
      if (step_length) {
        fPath.AddSegment(step_length, 0); // Vacuuum
#ifdef TREADLOUDLY
        std::cout << "\t[PATH]: Step " << fSteps << ", Traversed "
                  << step_length << " m in Vacuum" << std::endl;
#endif
      }
    }
  }

  void NullStep(double step_length) {
    if (fNNonNullSteps) { // Passed through volume and back out again
      AddSegment(step_length);
    }
    fNSequentialNullSteps++;
  }

  void NonNullStep(double step_length) {

    AddSegment(step_length);

    fNSequentialNullSteps = 0;
    fNNonNullSteps++;
  }

  bool Step(TGeoNavigator *Navigator) {
    Navigator->FindNextBoundaryAndStep(fSafeStep);

    // Make sure you've descended into the new node
    TGeoNode *curr_node = Navigator->SearchNode();

    bool isNull = !bool(curr_node);

    double const *cp = Navigator->GetCurrentPoint();
    ROOT::Math::XYZPoint CurrentPosition(cp[0],cp[1],cp[2]);
    double step_length = sqrt((CurrentPosition - fPrevPosition).Mag2());

    if (curr_node) {
      NonNullStep(step_length);
      fPrevMaterial = curr_node->GetVolume()->GetMaterial();
    } else {
      NullStep(step_length);
    }

    fPrevPosition = CurrentPosition;

    if (isNull && fNNonNullSteps) { // out the back again
      return false;
    }
    return true; // Still inside
  }

  void Propagate(TGeoNavigator *Navigator = nullptr) {

    fPath.clear();

    if (!Navigator) {
      Navigator = gGeoManager->GetCurrentNavigator();
    }

    fNSequentialNullSteps = 0;
    fNNonNullSteps = 0;
    fSteps = 0;

    // Set up the navigator
    Navigator->SetCurrentPoint(fRay.fFourPos_lab.X(), fRay.fFourPos_lab.Y(),
                               fRay.fFourPos_lab.Z());
    ROOT::Math::XYZVector diru = fRay.fFourMom_lab.Vect().Unit();
    Navigator->SetCurrentDirection(diru.X(), diru.Y(), diru.Z());
    Navigator->SetStep(fStepLength);

    TGeoNode *curr_node = Navigator->SearchNode();

    fPrevPosition = fRay.fFourPos_lab.Vect();
    fPrevMaterial =
        bool(curr_node) ? curr_node->GetVolume()->GetMaterial() : nullptr;

    while (fNSequentialNullSteps <= kMaxNullSteps) {
      if (!Step(Navigator)) {
        fSteps++;
        return;
      }
      fSteps++;
    }
  }

  ROOT::Math::XYZPoint ChoosePosition() { return fPath.ChoosePosition(); }
  ROOT::Math::XYZPoint GetLastPosition() { return fPrevPosition; }
  mw::WeightedRayPath const &GetPath() { return fPath; }
}; // namespace geom
} // namespace geom
} // namespace nft
