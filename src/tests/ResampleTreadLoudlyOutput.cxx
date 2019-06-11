#include "TFile.h"
#include "TRandom3.h"
#include "TTree.h"

#include <iostream>

int main() {
  TFile *fin = TFile::Open("TreadLoudlyTest.root");
  TTree *tin;
  fin->GetObject("rdt", tin);
  TTree *tmeta;
  fin->GetObject("meta", tmeta);

  double RayWeight;
  tin->SetBranchAddress("RayWeight", &RayWeight);

  double MaxDensityWeightedPath;
  tmeta->SetBranchAddress("MaxDensityWeightedPath", &MaxDensityWeightedPath);
  double RefDensityWeightedPath;
  tmeta->SetBranchAddress("RefDensityWeightedPath", &RefDensityWeightedPath);
  tmeta->GetEntry(0);

  TFile *fout = TFile::Open("TreadLoudlyTest_resample.root", "RECREATE");
  TTree *tout = tin->CloneTree(0);
  tout->SetDirectory(fout);

  size_t nents = tin->GetEntries();
  size_t nsel = 0;
  for (size_t e_it = 0; e_it < nents; ++e_it) {
    tin->GetEntry(e_it);

    double orig_path_weight = RayWeight * RefDensityWeightedPath;
    double acc_throw = gRandom->Uniform() * MaxDensityWeightedPath;

    // std::cout << "[INFO]: " << acc_throw << "/" << orig_path_weight <<
    // std::endl;

    if (acc_throw < orig_path_weight) {
      tout->Fill();
      nsel++;
    }
  }
  std::cout << "[INFO]: Resample efficiency " << nsel << "/" << nents
            << " = " << double(nsel) / double(nents) << std::endl;
  fout->Write();
  fout->Close();
}
