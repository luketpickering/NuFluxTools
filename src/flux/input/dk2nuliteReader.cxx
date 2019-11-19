#include "flux/IDecayParentReader.hxx"

#include "utils/PDGUtils.hxx"
#include "utils/ROOTUtils.hxx"

class dk2nuliteReader : public IDecayParentReader {

  struct {
    // dk2nu
    Int_t decay_ntype;
    Double_t decay_vx;
    Double_t decay_vy;
    Double_t decay_vz;
    Double_t decay_pdpx;
    Double_t decay_pdpy;
    Double_t decay_pdpz;
    Double_t decay_ppdxdz;
    Double_t decay_ppdydz;
    Double_t decay_pppz;
    Double_t decay_ppenergy;
    Int_t decay_ptype;
    Double_t decay_muparpx;
    Double_t decay_muparpy;
    Double_t decay_muparpz;
    Double_t decay_mupare;
    Double_t decay_necm;
    Double_t decay_nimpwt;

    // dkmeta
    Double_t pots;
  } dk2nulite;

  mutable nft::utils::TreeFile fdk2nuliteChain;

  double fTotalPOT;
  Long64_t fNEntries;

public:
  dk2nuliteReader() : fdk2nuliteChain(), fTotalPOT(0), fNEntries(0) {}
  bool CanReadFile(std::string const &file) {

    try {
      (void)nft::utils::CheckOpenTChain(file, "dk2nuTree_lite");
      (void)nft::utils::CheckOpenTChain(file, "dkmetaTree_lite");
    } catch (nft::utils::failed_to_get_TTree) {
      return false;
    }
    return true;
  }

  bool Initialize(std::string const &file) {
    if (!CanReadFile(file)) {
      return false;
    }

    return AddFiles(file);
  }

  bool AddFiles(std::string const &add) {
    if (!fdk2nuliteChain) {
      fdk2nuliteChain = nft::utils::CheckOpenTChain(add, "dk2nuTree_lite");

      fdk2nuliteChain->SetBranchAddress("decay_ntype", &dk2nulite.decay_ntype);
      fdk2nuliteChain->SetBranchAddress("decay_vx", &dk2nulite.decay_vx);
      fdk2nuliteChain->SetBranchAddress("decay_vy", &dk2nulite.decay_vy);
      fdk2nuliteChain->SetBranchAddress("decay_vz", &dk2nulite.decay_vz);
      fdk2nuliteChain->SetBranchAddress("decay_pdpx", &dk2nulite.decay_pdpx);
      fdk2nuliteChain->SetBranchAddress("decay_pdpy", &dk2nulite.decay_pdpy);
      fdk2nuliteChain->SetBranchAddress("decay_pdpz", &dk2nulite.decay_pdpz);
      fdk2nuliteChain->SetBranchAddress("decay_ppdxdz",
                                        &dk2nulite.decay_ppdxdz);
      fdk2nuliteChain->SetBranchAddress("decay_ppdydz",
                                        &dk2nulite.decay_ppdydz);
      fdk2nuliteChain->SetBranchAddress("decay_pppz", &dk2nulite.decay_pppz);
      fdk2nuliteChain->SetBranchAddress("decay_ppenergy",
                                        &dk2nulite.decay_ppenergy);
      fdk2nuliteChain->SetBranchAddress("decay_ptype", &dk2nulite.decay_ptype);
      fdk2nuliteChain->SetBranchAddress("decay_muparpx",
                                        &dk2nulite.decay_muparpx);
      fdk2nuliteChain->SetBranchAddress("decay_muparpy",
                                        &dk2nulite.decay_muparpy);
      fdk2nuliteChain->SetBranchAddress("decay_muparpz",
                                        &dk2nulite.decay_muparpz);
      fdk2nuliteChain->SetBranchAddress("decay_mupare",
                                        &dk2nulite.decay_mupare);
      fdk2nuliteChain->SetBranchAddress("decay_necm", &dk2nulite.decay_necm);
      fdk2nuliteChain->SetBranchAddress("decay_nimpwt",
                                        &dk2nulite.decay_nimpwt);

      fdk2nuliteChain->GetEntry(0);
    }

    auto rtn = fdk2nuliteChain.chain()->Add(add.c_str());
    fNEntries = fdk2nuliteChain->GetEntries();

    nft::utils::TreeFile dkmetaliteChain =
        nft::utils::CheckOpenTChain(add, "dkmetaTree_lite");
    dkmetaliteChain->SetBranchAddress("pots", &dk2nulite.pots);

    for (Long64_t i = 0; i < dkmetaliteChain->GetEntries(); ++i) {
      dkmetaliteChain->GetEntry(i);
      fTotalPOT += dk2nulite.pots;
    }

    return rtn;
  }

  size_t GetN() const { return fNEntries; };

  nft::flux::DecayParent Get(size_t n) const {

    if (n >= GetN()) {
      throw IDecayParentReader_OOB()
          << "[ERROR]: Attempted to get dk2nulite entry " << n
          << " but there are only: " << GetN();
    }

    nft::flux::DecayParent rdr;

    fdk2nuliteChain->GetEntry(n);

    rdr.fENu_com = dk2nulite.decay_necm;
    rdr.fDecayTo = dk2nulite.decay_ntype;
    rdr.fPOT = fTotalPOT / double(fNEntries);
    rdr.fWindowWeight_cm2 = 1E-4;
    rdr.fMCWeight = dk2nulite.decay_nimpwt / M_PI;

    rdr.fThreePos_lab = ROOT::Math::XYZPoint{
        dk2nulite.decay_vx, dk2nulite.decay_vy, dk2nulite.decay_vz};
    rdr.fFourMom_lab = ROOT::Math::PxPyPzMVector{
        dk2nulite.decay_pdpx, dk2nulite.decay_pdpy, dk2nulite.decay_pdpz,
        nft::utils::GetMass(dk2nulite.decay_ptype)};
    rdr.fPDG = dk2nulite.decay_ptype;

    rdr.fMomAtProduction = ROOT::Math::PxPyPzEVector{
        dk2nulite.decay_ppdxdz * dk2nulite.decay_pppz,
        dk2nulite.decay_ppdydz * dk2nulite.decay_pppz, dk2nulite.decay_pppz,
        dk2nulite.decay_ppenergy};

    rdr.fParentMomAtDecay = ROOT::Math::PxPyPzEVector{
        dk2nulite.decay_muparpx, dk2nulite.decay_muparpy,
        dk2nulite.decay_muparpz, dk2nulite.decay_mupare};

    return rdr;
  }

  ~dk2nuliteReader() {}
};

DECLARE_PLUGIN(IDecayParentReader, dk2nuliteReader);
