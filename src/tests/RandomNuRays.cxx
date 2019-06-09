#include "flux/NuRayGun.hxx"

#include "geom/RandomConeDirection.hxx"
#include "geom/RandomCuboidVolume.hxx"

#include "utils/PDGUtils.hxx"
#include "utils/UniformEnergyDistribution.hxx"

#include "TFile.h"
#include "TLorentzVector.h"
#include "TTree.h"

int main() {
  nft::flux::NuRayGun NRG(
      14,
      std::unique_ptr<nft::utils::IEnergyDistribution>(
          new nft::utils::UniformEnergyDistribution(500, 1000)),
      std::unique_ptr<nft::geom::IPostionDistribution>(
          new nft::geom::RandomCuboidVolume()),
      std::unique_ptr<nft::geom::IDirectionDistribution>(
          new nft::geom::RandomConeDirection(TVector3(-1, 0, 0), 0.1 * M_PI)));

  TFile *fout = TFile::Open("RandomNuRaysTest.root", "RECREATE");
  TTree *tout = new TTree("nrt", "");

  TLorentzVector *fm = nullptr;
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
