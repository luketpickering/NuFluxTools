#include "geom/RandomConeDirection.hxx"

#include "TFile.h"
#include "TTree.h"

int main() {
  nft::geom::RandomConeDirection alongZ(TVector3(0, 0, 1), 0.25 * M_PI);
  nft::geom::RandomConeDirection alongX(TVector3(1, 0, 0), 0.5 * M_PI);
  nft::geom::RandomConeDirection alongY(TVector3(0, 1, 0), 0);

  TFile *fout = TFile::Open("RandomDirectionTest.root", "RECREATE");
  TTree *tout = new TTree("rdt", "");

  TVector3 *rdz = nullptr;
  tout->Branch("rdz", &rdz);
  TVector3 *rdx = nullptr;
  tout->Branch("rdx", &rdx);
  TVector3 *rdy = nullptr;
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
