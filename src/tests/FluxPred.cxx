#include "flux/IDecayParentReader.hxx"

#include "plugins/Instantiate.hxx"

#include "geom/CoordinateTransformation.hxx"
#include "geom/FluxWindow.hxx"

#include "TFile.h"
#include "TH1.h"
#include "TTree.h"

#include "fhiclcpp/make_ParameterSet.h"

#include <cmath>

int main(int argc, char const *argv[]) {

  fhicl::ParameterSet ps = fhicl::make_ParameterSet(argv[1]);

  auto dk2nurdr = nft::plugins::Instantiate<IDecayParentReader>(
      ps.get<std::string>("input.reader"));

  if (!dk2nurdr->Initialize(ps.get<std::string>("input.file"))) {
    return 1;
  }

  nft::geom::FluxWindow fw(ps.get<fhicl::ParameterSet>("window"));

  auto tf = nft::geom::BuildTransformation(
      ps.get<fhicl::ParameterSet>("beam_to_det_transformation", {}));

  dk2nurdr->SetTransformation(tf.Inverse());

  std::cout << " Reading " << dk2nurdr->GetN() << std::endl;
  size_t i = 0;
  TFile *fout = new TFile("fout.root", "RECREATE");

  TTree *partree = new TTree("partree", "");
  partree->SetDirectory(fout);

  ROOT::Math::XYZPoint *bt_loca = nullptr;
  partree->Branch("bt_loca", &bt_loca);
  ROOT::Math::XYZVector *bt_p = nullptr;
  partree->Branch("bt_p", &bt_p);

  ROOT::Math::XYZPoint *at_loca = nullptr;
  partree->Branch("at_loca", &at_loca);
  ROOT::Math::XYZVector *at_p = nullptr;
  partree->Branch("at_p", &at_p);

  ROOT::Math::XYZPoint *nr_loca = nullptr;
  partree->Branch("nr_loca", &nr_loca);
  ROOT::Math::XYZVector *nr_p = nullptr;
  partree->Branch("nr_p", &nr_p);

  int nupdg;
  double wgt;
  partree->Branch("pdg", &nupdg);
  partree->Branch("wgt", &wgt);

  double total_pot = 0;

  TH1D *f = new TH1D("flux", "", 100, 0, 5);
  for (size_t i = 0; i < dk2nurdr->GetN(); ++i) {

    auto dk = dk2nurdr->Get_ToDetectorCoords(i);
    auto dk_bt = dk2nurdr->Get(i);

    (*bt_loca) = dk_bt.fThreePos_lab;
    (*bt_p) = dk_bt.fFourMom_lab.Vect();
    (*at_loca) = dk.fThreePos_lab;
    (*at_p) = dk.fFourMom_lab.Vect();

    total_pot += dk.fPOT;
    if (dk.fDecayTo != 14) {
      continue;
    }
    nft::geom::NuRay nuray = dk.Shoot(fw.GetRandomPosition());
    nuray.fWeight_cm2 *=
        std::abs(nuray.fFourMom_lab.Vect().Unit().Dot(fw.fUnitNormal));
    nupdg = dk.fDecayTo;
    wgt = nuray.fWeight_cm2;

    (*nr_loca) = nuray.fThreePos_lab;
    (*nr_p) = nuray.fFourMom_lab.Vect();

    partree->Fill();

    // std::cout << "[NURAY]: Chose: " << nuray.fPDG << ", through: ["
    //           << nuray.fThreePos_lab.X() << ", " << nuray.fThreePos_lab.Y()
    //           << ", " << nuray.fThreePos_lab.Z() << "] with energy "
    //           << nuray.fFourMom_lab.E() << " and weight: " << nuray.fWeight
    //           << std::endl;

    f->Fill(nuray.fFourMom_lab.E(), nuray.fWeight_cm2);

    // if (i > 10) {
    //   break;
    // }
    // i++;
  }

  f->Scale(1.0 / total_pot, "WIDTH");
  f->GetYaxis()->SetTitle("#nus / cm^{2} / POT per GeV");
  f->GetXaxis()->SetTitle("#it{E}_{#nu} (GeV)");
  f->SetDirectory(fout);

  fout->Write();
  fout->Close();
}
