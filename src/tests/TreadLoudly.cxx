#include "flux/NuRayGun.hxx"

#include "geom/RandomConeDirection.hxx"
#include "geom/RandomCuboidVolume.hxx"

// #define TREADLOUDLY

#include "geom/NuRayPropagator.hxx"

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

  double RayWeight;
  tout->Branch("RayWeight", &RayWeight, "RayWeight/D");
  TVector3 *StartPos = nullptr;
  tout->Branch("StartPos", &StartPos);
  TVector3 *Dir = nullptr;
  tout->Branch("Dir", &Dir);
  TVector3 *EndPos = nullptr;
  tout->Branch("EndPos", &EndPos);
  TVector3 *DensityWeightedPos = nullptr;
  tout->Branch("DensityWeightedPos", &DensityWeightedPos);

  std::string nftdir = nft::utils::GetNFTDir();
  std::string test_gdml_loc =
      nftdir + "share/gdml/SimpleBoxes.geom.manual.gdml";

  gGeoManager->Import(test_gdml_loc.c_str());

  size_t N = 1E6;
  size_t NPresample = 1E4;
  double RefDensityWeightedPath = NPresample ? 0 : 1;
  size_t ShoutEvery = N / 100;
  for (size_t i = 0; i < N; ++i) {
    if (ShoutEvery && !(i % ShoutEvery)) {
      std::cout << "[INFO]: Throw " << i << "/" << N << std::endl;
    }
    auto nr = NRG.Shoot();
    (*StartPos) = nr.fFourPos_lab.Vect();
    (*Dir) = nr.fFourMom_lab.Vect().Unit();

#ifdef TREADLOUDLY
    std::cout << "[SHOOT " << i << "]: orig: " << ThreeArrToStr(nr.fFourPos_lab)
              << ", mom: " << ThreeArrToStr(nr.fFourMom_lab) << std::endl;
#endif

    nft::geom::NuRayPropagator nrp(nr);

    nrp.Propagate();

    double DensityWeightedPathLength = nrp.GetPath().GetPathWeight();
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

  fout->Write();
  fout->Close();
}
