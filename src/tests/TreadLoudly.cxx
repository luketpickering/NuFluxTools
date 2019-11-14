#include "flux/NuRayGun.hxx"

#include "rng/RandomDirectionDistributions.hxx"
#include "rng/RandomPositionDistributions.hxx"
#include "rng/UniformEnergyDistribution.hxx"

// #define TREADLOUDLY

#include "geom/NuRayPropagator.hxx"

#include "material_weighting/LinearXSecWeighter.hxx"

#include "utils/FileSystemUtils.hxx"
#include "utils/PDGUtils.hxx"

#include "TFile.h"
#include "TGeoManager.h"
#include "TTree.h"

#include <iostream>
#include <sstream>

int main(int argc, char const *argv[]) {
  nft::flux::NuRayGun NRG(
      14,
      std::unique_ptr<nft::rng::IEnergyDistribution>(
          new nft::rng::UniformEnergyDistribution(1000, 1000)),
      std::unique_ptr<nft::rng::RandomAACuboidCone>(
          new nft::rng::RandomAACuboidCone(ROOT::Math::XYZPoint(0, -200, -400),
                                           ROOT::Math::XYZVector(1000, 600, 0),
                                           ROOT::Math::Polar3DVector(1, 0, 0),
                                           0)));

  TFile *fout = TFile::Open("TreadLoudlyTest.root", "RECREATE");
  TTree *tout = new TTree("rdt", "");

  double RayWeight;
  tout->Branch("RayWeight", &RayWeight, "RayWeight/D");
  double e_nu;
  tout->Branch("e_nu", &e_nu, "e_nu/D");
  ROOT::Math::XYZPoint *StartPos = nullptr;
  tout->Branch("StartPos", &StartPos);
  ROOT::Math::XYZVector *Dir = nullptr;
  tout->Branch("Dir", &Dir);
  ROOT::Math::XYZPoint *EndPos = nullptr;
  tout->Branch("EndPos", &EndPos);
  ROOT::Math::XYZPoint *DensityWeightedPos = nullptr;
  tout->Branch("DensityWeightedPos", &DensityWeightedPos);

  TTree *tmeta = new TTree("meta", "");
  double MaxDensityWeightedPath = 0;
  tmeta->Branch("MaxDensityWeightedPath", &MaxDensityWeightedPath,
                "MaxDensityWeightedPath/D");
  double RefDensityWeightedPath;
  tmeta->Branch("RefDensityWeightedPath", &RefDensityWeightedPath,
                "RefDensityWeightedPath/D");

  std::string nftdir = nft::utils::GetNFTDir();
  std::string test_gdml_loc =
      nftdir + "share/gdml/SimpleBoxes.geom.manual.gdml";

  if (argc > 1) {
    test_gdml_loc = argv[1];
  }

  gGeoManager->Import(test_gdml_loc.c_str());

  size_t N = 1E6;
  size_t NPresample = 0;
  RefDensityWeightedPath = NPresample ? 0 : 1;
  size_t ShoutEvery = N / 100;
  for (size_t i = 0; i < N; ++i) {
    if (ShoutEvery && !(i % ShoutEvery)) {
      std::cout << "[INFO]: Throw " << i << "/" << N << std::endl;
    }
    auto nr = NRG.Shoot();
    (*StartPos) = nr.fThreePos_lab;
    (*Dir) = nr.fFourMom_lab.Vect().Unit();
    e_nu = nr.fFourMom_lab.E();

#ifdef TREADLOUDLY
    std::cout << "[SHOOT " << i << "]: orig: " << ThreeArrToStr(nr.fThreePos_lab)
              << ", mom: " << ThreeArrToStr(nr.fFourMom_lab) << std::endl;
#endif

    nft::geom::NuRayPropagator nrp(
        nr, std::make_unique<nft::mw::LinearXSecWeighter>());

    nrp.Propagate();

    double DensityWeightedPathLength = nrp.GetPath().GetPathWeight();
    MaxDensityWeightedPath =
        std::max(MaxDensityWeightedPath, DensityWeightedPathLength);
    if (i < NPresample) {
      RefDensityWeightedPath =
          std::max(RefDensityWeightedPath, DensityWeightedPathLength);
      continue;
    } else if (i == NPresample) {
      std::cout << "[INFO]: Finished ray presampling, found reference ray "
                   "density-weighted path: "
                << RefDensityWeightedPath << std::endl;
    }

    (*DensityWeightedPos) = nrp.ChoosePosition();
    (*EndPos) = nrp.GetLastPosition();

    RayWeight = DensityWeightedPathLength / RefDensityWeightedPath;

    tout->Fill();
  }

  tmeta->Fill();

  fout->Write();
  fout->Close();
}
