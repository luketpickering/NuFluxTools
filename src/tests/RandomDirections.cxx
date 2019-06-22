#include "rng/RandomDirectionDistributions.hxx"

#include "TFile.h"
#include "TTree.h"

int main() {
  nft::rng::RandomConeDirection alongZ(ROOT::Math::XYZVector(0, 0, 1),
                                        0.25 * M_PI);
  nft::rng::RandomConeDirection alongX(ROOT::Math::XYZVector(1, 0, 0),
                                        0.5 * M_PI);
  nft::rng::RandomConeDirection alongY(ROOT::Math::XYZVector(0, 1, 0), 0);

  TFile *fout = TFile::Open("RandomDirectionTest.root", "RECREATE");
  TTree *tout = new TTree("rdt", "");

  ROOT::Math::XYZVector *rdz = nullptr;
  tout->Branch("rdz", &rdz);
  ROOT::Math::XYZVector *rdx = nullptr;
  tout->Branch("rdx", &rdx);
  ROOT::Math::XYZVector *rdy = nullptr;
  tout->Branch("rdy", &rdy);

  size_t N = 1E5;
  for (size_t i = 0; i < N; ++i) {
    (*rdz) = alongZ.GetRandomDirection();
    (*rdx) = alongX.GetRandomDirection();
    (*rdy) = alongY.GetRandomDirection();
    tout->Fill();
  }

  fout->Write();
  fout->Close();
}
