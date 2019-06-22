#include "rng/RandomPositionDistributions.hxx"

#include "Math/RotationZYX.h"

#include "TFile.h"
#include "TTree.h"

int main() {
  nft::rng::RandomAACuboidVolume vol(ROOT::Math::XYZPoint(1, -2, 3),
                                      ROOT::Math::XYZVector(-0.5, 1, 1.5));

  nft::rng::RandomXYRectangularPlane plane(ROOT::Math::XYZPoint(4, -3, 3),
                                            ROOT::Math::XYVector(-0.1, 3.1));

  nft::rng::RandomCuboidVolume rotvol(
      ROOT::Math::XYZPoint(1, -2, 3), ROOT::Math::XYZVector(-0.5, 1, 1.5),
      ROOT::Math::RotationZYX(M_PI * 0.25, M_PI * 0.25, 0));

  TFile *fout = TFile::Open("RandomPositionTest.root", "RECREATE");
  TTree *tout = new TTree("rpt", "");

  ROOT::Math::XYZPoint *rv = nullptr;
  tout->Branch("rv", &rv);

  ROOT::Math::XYZPoint *rp = nullptr;
  tout->Branch("rp", &rp);

  ROOT::Math::XYZPoint *rrv = nullptr;
  tout->Branch("rrv", &rrv);

  size_t N = 1E5;
  for (size_t i = 0; i < N; ++i) {
    (*rv) = vol.GetRandomPosition();
    (*rp) = plane.GetRandomPosition();
    (*rrv) = rotvol.GetRandomPosition();
    tout->Fill();
  }

  fout->Write();
  fout->Close();
}
