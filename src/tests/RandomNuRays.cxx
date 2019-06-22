#include "flux/NuRayGun.hxx"

#include "rng/RandomDirectionDistributions.hxx"
#include "rng/RandomPositionDistributions.hxx"
#include "rng/UniformEnergyDistribution.hxx"

#include "utils/PDGUtils.hxx"

#include "Math/Vector4D.h"

#include "TFile.h"
#include "TTree.h"

int main() {
  nft::flux::NuRayGun NRG(
      14,
      std::unique_ptr<nft::rng::IEnergyDistribution>(
          new nft::rng::UniformEnergyDistribution(500, 1000)),
      std::unique_ptr<nft::rng::RandomAACuboidCone>(
          new nft::rng::RandomAACuboidCone(
              ROOT::Math::XYZPoint(0, 0, 0), ROOT::Math::XYZVector(0, 0, 0),
              ROOT::Math::Polar3DVector(1, M_PI, M_PI), 0.1 * M_PI)));

  TFile *fout = TFile::Open("RandomNuRaysTest.root", "RECREATE");
  TTree *tout = new TTree("nrt", "");

  ROOT::Math::XYZTVector *fm = nullptr;
  tout->Branch("fm", &fm);
  nft::PDG_t pdg;
  tout->Branch("pdg", &pdg, "pdg/I");

  size_t N = 1E5;
  for (size_t i = 0; i < N; ++i) {
    auto nr = NRG.Shoot();
    (*fm) = nr.fFourMom_lab;
    pdg = nr.fPDG;
    tout->Fill();
  }

  fout->Write();
  fout->Close();
}
