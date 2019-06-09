#include "geom/RandomCuboidVolume.hxx"

#include "TFile.h"
#include "TTree.h"

int main() {
  nft::geom::RandomCuboidVolume vol(TVector3(1, -2, 3), TVector3(-0.5, 1, 1.5));

  TFile *fout = TFile::Open("RandomPositionTest.root", "RECREATE");
  TTree *tout = new TTree("rpt", "");

  TVector3 *rp = nullptr;
  tout->Branch("rp", &rp);

  size_t N = 1E5;
  for(size_t i = 0; i < N; ++i){
    (*rp) = vol.GetRandomPosition();
    tout->Fill();
  }

  fout->Write();
  fout->Close();
}
