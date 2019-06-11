#include "TCanvas.h"
#include "TFile.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TTree.h"

#include <iostream>

int main() {
  TFile *fin = TFile::Open("TreadLoudlyTest.root");
  if (!fin) {
    std::cout << "[ERROR]: Failed to find file TreadLoudlyTest.root"
              << std::endl;
    return 1;
  }
  TTree *tin = nullptr;
  fin->GetObject("rdt", tin);

  if (!tin) {
    std::cout << "[ERROR]: Failed to find tree rdt in file TreadLoudlyTest.root"
              << std::endl;
    return 2;
  }

  TCanvas c1("c1", "");
  c1.Print("TreadLoudlyPlots.pdf[");

  // TH3D *StartPos = new TH3D("StartPosition", ";Z (cm);X (cm); Y (cm)", 125,
  //                           -250, 250, 125, -250, 250, 125, -250, 250);
  // tin->Draw("StartPos.X():StartPos.Y():StartPos.Z() >> StartPosition");
  // c1.Print("TreadLoudlyPlots.pdf");
  //
  // TH3D *EndPos = new TH3D("EndPosition", ";Z (cm);X (cm); Y (cm)", 125, -250,
  //                         250, 125, -250, 250, 125, -250, 250);
  // tin->Draw("EndPos.X():EndPos.Y():EndPos.Z() >> EndPosition");
  // c1.Print("TreadLoudlyPlots.pdf");
  //
  // TH3D *EnterScint = new TH3D("EnterScintPosition", ";Z (cm);X (cm); Y (cm)",
  //                             125, -250, 250, 125, -250, 250, 125, -250,
  //                             250);
  // tin->Draw(
  //     "EnterScint.X():EnterScint.Y():EnterScint.Z() >> EnterScintPosition");
  // c1.Print("TreadLoudlyPlots.pdf");
  //
  // TH3D *ExitScint = new TH3D("ExitScintPosition", ";Z (cm);X (cm); Y (cm)",
  // 125,
  //                            -250, 250, 125, -250, 250, 125, -250, 250);
  // tin->Draw("ExitScint.X():ExitScint.Y():ExitScint.Z() >>
  // ExitScintPosition"); c1.Print("TreadLoudlyPlots.pdf");
  //
  // TH3D *DensityWeightedPos =
  //     new TH3D("DensityWeightedPosition", ";Z (cm);X (cm); Y (cm)", 125,
  //     -250,
  //              250, 125, -250, 250, 125, -250, 250);
  // tin->Draw("DensityWeightedPos.X():DensityWeightedPos.Y():DensityWeightedPos."
  //           "Z() >> DensityWeightedPosition");
  // c1.Print("TreadLoudlyPlots.pdf");

  // TH2D *EnterScint_2d = new TH2D("EnterScintPosition_2d", ";Z (cm);Y (cm)",
  //                                500, -250, 250, 500, -250, 250);
  // tin->Draw("EnterScint.Y():EnterScint.Z() >>
  // EnterScintPosition_2d","","COLZ"); c1.Print("TreadLoudlyPlots.pdf");
  //
  // TH2D *ExitScint_2d = new TH2D("ExitScintPosition_2d", ";Z (cm);Y (cm)",
  // 500,
  //                               -250, 250, 500, -250, 250);
  // tin->Draw("ExitScint.Y():ExitScint.Z() >> ExitScintPosition_2d","","COLZ");
  // c1.Print("TreadLoudlyPlots.pdf");

  TH2D *DensityWeightedPos_2d =
      new TH2D("DensityWeightedPosition_2d", ";Z (cm);Y (cm)", 500, -250, 250,
               500, -250, 250);
  tin->Draw("DensityWeightedPos.Y():DensityWeightedPos."
            "Z() >> DensityWeightedPosition_2d",
            "", "COLZ");
  c1.Print("TreadLoudlyPlots.pdf");

  TH2D *DensityWeightedPos_2d_weighted =
      new TH2D("DensityWeightedPosition_2d_weighted", ";Z (cm);Y (cm)", 500,
               -250, 250, 500, -250, 250);
  tin->Draw("DensityWeightedPos.Y():DensityWeightedPos."
            "Z() >> DensityWeightedPosition_2d_weighted",
            "RayWeight", "COLZ");
  c1.Print("TreadLoudlyPlots.pdf");
  DensityWeightedPos_2d_weighted->GetZaxis()->SetRangeUser(
      1E-3, DensityWeightedPos_2d_weighted->GetMaximum());
  c1.SetLogz();
  c1.Print("TreadLoudlyPlots.pdf");

  // TH2D *EnterScint_2d_x = new TH2D("EnterScintPosition_2d_x", ";Z (cm);X
  // (cm)",
  //                                500, -250, 250, 500, -250, 250);
  // tin->Draw("EnterScint.X():EnterScint.Z() >>
  // EnterScintPosition_2d_x","","COLZ"); c1.Print("TreadLoudlyPlots.pdf");
  //
  // TH2D *ExitScint_2d_x = new TH2D("ExitScintPosition_2d_x", ";Z (cm);X (cm)",
  // 500,
  //                               -250, 250, 500, -250, 250);
  // tin->Draw("ExitScint.X():ExitScint.Z() >>
  // ExitScintPosition_2d_x","","COLZ"); c1.Print("TreadLoudlyPlots.pdf");
  //
  // TH2D *DensityWeightedPos_2d_x =
  //     new TH2D("DensityWeightedPosition_2d_x", ";Z (cm);X (cm)", 500, -250,
  //     250,
  //              500, -250, 250);
  // tin->Draw("DensityWeightedPos.X():DensityWeightedPos."
  //           "Z() >> DensityWeightedPosition_2d_x","","COLZ");
  // c1.Print("TreadLoudlyPlots.pdf");
  //
  // TH2D *DensityWeightedPos_2d_x_weighted =
  //     new TH2D("DensityWeightedPosition_2d_x_weighted", ";Z (cm);X (cm)",
  //     500, -250, 250,
  //              500, -250, 250);
  // tin->Draw("DensityWeightedPos.X():DensityWeightedPos."
  //           "Z() >>
  //           DensityWeightedPosition_2d_x_weighted","RayWeight","COLZ");
  // c1.Print("TreadLoudlyPlots.pdf");

  TFile *finres = TFile::Open("TreadLoudlyTest_resample.root");
  if (finres) {

    TTree *tinres = nullptr;
    finres->GetObject("rdt", tinres);

    if (!tinres) {
      std::cout
          << "[ERROR]: Failed to find tree rdt in file TreadLoudlyTest.root"
          << std::endl;
      return 2;
    }

    TH2D *DensityWeightedPos_2d =
        new TH2D("DensityWeightedPosition_2d", "rejsampled;Z (cm);Y (cm)", 500,
                 -250, 250, 500, -250, 250);
    tinres->Draw("DensityWeightedPos.Y():DensityWeightedPos."
                 "Z() >> DensityWeightedPosition_2d",
                 "", "COLZ");
    c1.SetLogz(false);

    c1.Print("TreadLoudlyPlots.pdf");
  }

  c1.Print("TreadLoudlyPlots.pdf]");
}
