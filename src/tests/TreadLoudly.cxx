#include "flux/NuRayGun.hxx"

#include "geom/RandomConeDirection.hxx"
#include "geom/RandomCuboidVolume.hxx"

#include "utils/FileSystemUtils.hxx"
#include "utils/PDGUtils.hxx"
#include "utils/RandomTools.hxx"
#include "utils/UniformEnergyDistribution.hxx"

#include "TFile.h"
#include "TGeoManager.h"
#include "TLorentzVector.h"
#include "TTree.h"

#include <iostream>
#include <sstream>

#define TREADQUIETLY

template <typename ArrLike> inline std::string ThreeArrToStr(ArrLike d) {
  std::stringstream ss("");
  ss.precision(3);
  ss << "[" << d[0] << ", " << d[1] << ", " << d[2] << " ]";
  return ss.str();
}

int main() {
  nft::flux::NuRayGun NRG(
      14,
      std::unique_ptr<nft::utils::IEnergyDistribution>(
          new nft::utils::UniformEnergyDistribution(500, 1000)),
      std::unique_ptr<nft::geom::IPostionDistribution>(
          new nft::geom::RandomCuboidVolume(TVector3(0, 0, -160),
                                            TVector3(0, 50, 0))),
      std::unique_ptr<nft::geom::IDirectionDistribution>(
          new nft::geom::RandomConeDirection(TVector3(0, 0, 1))));

  TFile *fout = TFile::Open("TreadLoudlyTest.root", "RECREATE");
  TTree *tout = new TTree("rdt", "");
  double TotalDistanceAir_m;
  tout->Branch("TotalDistanceAir_m", &TotalDistanceAir_m,
               "TotalDistanceAir_m/D");
  double TotalDistanceScint_m;
  tout->Branch("TotalDistanceScint_m", &TotalDistanceScint_m,
               "TotalDistanceScint_m/D");
  double DensityWeightedPathLength;
  tout->Branch("DensityWeightedPathLength", &DensityWeightedPathLength,
               "DensityWeightedPathLength/D");
  double RayWeight;
  tout->Branch("RayWeight", &RayWeight, "RayWeight/D");
  TVector3 *RayDir = nullptr;
  tout->Branch("RayDir", &RayDir);
  TVector3 *StartPos = nullptr;
  tout->Branch("StartPos", &StartPos);
  TVector3 *EndPos = nullptr;
  tout->Branch("EndPos", &EndPos);
  TVector3 *EnterScint = nullptr;
  tout->Branch("EnterScint", &EnterScint);
  TVector3 *ExitScint = nullptr;
  tout->Branch("ExitScint", &ExitScint);

  TVector3 *DensityWeightedPos = nullptr;
  tout->Branch("DensityWeightedPos", &DensityWeightedPos);

  std::string nftdir = nft::utils::GetNFTDir();
  std::string test_gdml_loc =
      nftdir + "share/gdml/SimpleBoxes.geom.manual.gdml";

  gGeoManager->Import(test_gdml_loc.c_str());

  TGeoMaterial *mat_Air = gGeoManager->GetMaterial("Air");
  TGeoMaterial *mat_Scint = gGeoManager->GetMaterial("CScint");

  size_t N = 1E7;
  size_t NPresample = 1E5;
  double RefDensityWeightedPath = NPresample ? 0 : 1;
  size_t ShoutEvery = N / 100;
  for (size_t i = 0; i < N; ++i) {
    if (ShoutEvery && !(i % ShoutEvery)) {
      std::cout << "[INFO]: Throw " << i << "/" << N << std::endl;
    }
    auto nr = NRG.Shoot();
#ifndef TREADQUIETLY
    std::cout << "[SHOOT " << i << "]: orig: " << ThreeArrToStr(nr.fFourPos_lab)
              << ", mom: " << ThreeArrToStr(nr.fFourMom_lab) << std::endl;
#endif

    gGeoManager->SetCurrentPoint(nr.fFourPos_lab[0], nr.fFourPos_lab[1],
                                 nr.fFourPos_lab[2]);
    TVector3 diru = nr.fFourMom_lab.Vect().Unit();
    (*RayDir) = diru;
    gGeoManager->SetCurrentDirection(diru[0], diru[1], diru[2]);
    gGeoManager->SetStep(0.1);

    TGeoNode *n = gGeoManager->SearchNode();
#ifndef TREADQUIETLY
    std::cout << "[START]: p: " << ThreeArrToStr(gGeoManager->GetCurrentPoint())
              << ", d: " << ThreeArrToStr(gGeoManager->GetCurrentDirection())
              << " @ " << (n ? n->GetName() : "NULL")
              << ", {b: " << gGeoManager->IsOnBoundary()
              << ", out: " << gGeoManager->IsOutside() << "}" << std::endl;
#endif

    size_t NullSteps = 0;
    size_t NNonNullSteps = 0;
    double SafeMeters = 1E2; /*m*/
    size_t SafeStep = SafeMeters * 1E2 / gGeoManager->GetStep() /*cm*/;
    size_t NSteps = 0;
    TVector3 LastPosition(gGeoManager->GetCurrentPoint());
    (*StartPos) = LastPosition;

    TotalDistanceAir_m = 0;
    TotalDistanceScint_m = 0;
    std::vector<std::pair<double, double>> path = {{0, 0}};

    TGeoMaterial *lm = nullptr;

    if (n) {
      n->GetVolume()->GetMaterial();
    }

    while (NullSteps < 4) {
      gGeoManager->FindNextBoundaryAndStep(SafeStep);
      n = gGeoManager->SearchNode();
      bool isNull = !bool(n);

      TVector3 CurrentPosition(gGeoManager->GetCurrentPoint());
      double Distance_m = (CurrentPosition - LastPosition).Mag() * 1E-2;
#ifndef TREADQUIETLY
      std::cout << "[Step " << NSteps << " dist: " << Distance_m
                << " m]: p: " << ThreeArrToStr(gGeoManager->GetCurrentPoint())
                << " @ " << (isNull ? "NULL" : n->GetName())
                << ", {b: " << gGeoManager->IsOnBoundary()
                << ", out: " << gGeoManager->IsOutside() << "}" << std::endl;
#endif

      if (isNull) {
        NullSteps++;
        if (NNonNullSteps) { // Passed through volume and back out again

          if (lm) {
            if (lm == mat_Air) {
              TotalDistanceAir_m += Distance_m;
              path.emplace_back(path.back().first + Distance_m,
                                path.back().second +
                                    (Distance_m * lm->GetDensity()));
#ifndef TREADQUIETLY
              std::cout << "[PATH]: Traversed " << Distance_m
                        << " m in Air (density_w = "
                        << (Distance_m * lm->GetDensity()) << "/"
                        << path.back().second << ")" << std::endl;
#endif

            } else if (lm == mat_Scint) {
              TotalDistanceScint_m += Distance_m;
              path.emplace_back(path.back().first + Distance_m,
                                path.back().second +
                                    (Distance_m * lm->GetDensity()));
#ifndef TREADQUIETLY
              std::cout << "[PATH]: Traversed " << Distance_m
                        << " m in Scint (density_w = "
                        << (Distance_m * lm->GetDensity()) << "/"
                        << path.back().second << ")" << std::endl;
#endif
            }
          }

          LastPosition = CurrentPosition;
          break;
        }
        continue;
      }

      TGeoVolume *v = n->GetVolume();
      TGeoMaterial *m = v->GetMaterial();
#ifndef TREADQUIETLY
      std::cout << "\tMaterial " << m->GetName() << ": {A: " << m->GetA()
                << ", Z: " << m->GetZ() << ", density: " << m->GetDensity()
                << "}" << std::endl;
#endif

      if (lm) {
        if (lm == mat_Air) {
          TotalDistanceAir_m += Distance_m;
          path.emplace_back(path.back().first + Distance_m,
                            path.back().second +
                                (Distance_m * lm->GetDensity()));
#ifndef TREADQUIETLY
          std::cout << "[PATH]: Traversed " << Distance_m
                    << " m in Air (density_w = "
                    << (Distance_m * lm->GetDensity()) << "/"
                    << path.back().second << ")" << std::endl;
#endif

          if (m == mat_Scint) {
            (*EnterScint) = CurrentPosition;
          }

        } else if (lm == mat_Scint) {
          TotalDistanceScint_m += Distance_m;
          path.emplace_back(path.back().first + Distance_m,
                            path.back().second +
                                (Distance_m * lm->GetDensity()));
#ifndef TREADQUIETLY
          std::cout << "[PATH]: Traversed " << Distance_m
                    << " m in Scint (density_w = "
                    << (Distance_m * lm->GetDensity()) << "/"
                    << path.back().second << ")" << std::endl;
#endif

          if (m != mat_Scint) {
            (*ExitScint) = CurrentPosition;
          }
        }
      } else {
        path.emplace_back(path.back().first + Distance_m, 0); // Vacuuum
#ifndef TREADQUIETLY
        std::cout << "[PATH]: Traversed " << Distance_m << " m in Vacuum"
                  << std::endl;
#endif
      }

      if (m) {
        lm = m;
      }

      NullSteps = 0;
      NNonNullSteps++;

      NSteps++;
      LastPosition = CurrentPosition;
    }

    DensityWeightedPathLength = path.back().second;
    if (i < NPresample) {
      RefDensityWeightedPath =
          std::max(RefDensityWeightedPath, DensityWeightedPathLength);
      continue;
    } else if (i == NPresample) {
      std::cout << "[INFO]: Finished ray presampling, found reference ray "
                   "density-weighted path: "
                << RefDensityWeightedPath << std::endl;
    }

    (*DensityWeightedPos) =
        nft::utils::ThrowWeightedPosition(nr.fFourPos_lab.Vect() * 1E-2,
                                          nr.fFourMom_lab.Vect() * 1E-2, path) *
        1E2;
    (*EndPos) = LastPosition;

    RayWeight = DensityWeightedPathLength / RefDensityWeightedPath;

    tout->Fill();
  }

  fout->Write();
  fout->Close();
}
