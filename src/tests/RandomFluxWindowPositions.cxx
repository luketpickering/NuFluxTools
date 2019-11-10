#include "geom/FluxWindow.hxx"

#include "fhiclcpp/make_ParameterSet.h"

#include "TFile.h"
#include "TTree.h"

int main(int argc, char const *argv[]) {

  nft::geom::FluxWindow fw(
      fhicl::make_ParameterSet(argv[1]).get<fhicl::ParameterSet>(
          "flux_window"));

  TFile *fout = TFile::Open("FluxWindowTest.root", "RECREATE");
  TTree *tout = new TTree("rpt", "");

  ROOT::Math::XYZPoint *rv = nullptr;
  tout->Branch("rv", &rv);

  size_t N = 1E5;
  for (size_t i = 0; i < N; ++i) {
    (*rv) = fw.GetRandomPosition();
    tout->Fill();
  }

  fout->Write();
  fout->Close();
}
