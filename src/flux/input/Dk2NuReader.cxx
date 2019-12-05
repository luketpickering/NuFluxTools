#include "flux/IDecayParentReader.hxx"

#include "utils/PDGUtils.hxx"
#include "utils/ROOTUtils.hxx"

#include "dk2nu/tree/dk2nu.h"
#include "dk2nu/tree/dkmeta.h"

class Dk2NuReader : public IDecayParentReader {

  mutable bsim::Dk2Nu *dkReader;

  mutable nft::utils::TreeFile fDk2NuChain;

  double fTotalPOT;
  Long64_t fNEntries;

public:
  Dk2NuReader()
      : dkReader(nullptr), fDk2NuChain(), fTotalPOT(0), fNEntries(0) {}
  bool CanReadFile(std::string const &file) {

    try {
      (void)nft::utils::CheckOpenTChain(file, "dk2nuTree");
      (void)nft::utils::CheckOpenTChain(file, "dkmetaTree");
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
    int NFiles = 0;
    if (!fDk2NuChain) {
      fDk2NuChain = nft::utils::CheckOpenTChain(add, "dk2nuTree");
      NFiles = fDk2NuChain.chain()->GetListOfFiles()->GetEntries();

      fDk2NuChain->SetBranchAddress("dk2nu", &dkReader);
      fDk2NuChain->GetEntry(0);
    } else {
      NFiles = fDk2NuChain.chain()->Add(add.c_str());
    }
    fNEntries = fDk2NuChain->GetEntries();

    nft::utils::TreeFile DkMetaTree =
        nft::utils::CheckOpenTChain(add, "dkmetaTree");

    bsim::DkMeta *meta = nullptr;
    DkMetaTree->SetBranchAddress("dkmeta", &meta);

    double nPOT = 0;
    for (Long64_t i = 0; i < DkMetaTree->GetEntries(); ++i) {
      DkMetaTree->GetEntry(i);
      nPOT += meta->pots;
    }

    fTotalPOT += nPOT;

    std::cout << "[INFO]: Added " << NFiles << " files with " << nPOT
              << " POT (" << fTotalPOT << " Total POT) to input Dk2Nu TChain."
              << std::endl;

    return NFiles;
  }

  size_t GetN() const { return fNEntries; };

  nft::flux::DecayParent Get(size_t n) const {

    if (n >= GetN()) {
      throw IDecayParentReader_OOB() << "[ERROR]: Attempted to get Dk2Nu entry "
                                     << n << " but there are only: " << GetN();
    }

    nft::flux::DecayParent rdr;

    fDk2NuChain->GetEntry(n);

    rdr.fENu_com = dkReader->decay.necm;
    rdr.fDecayTo = dkReader->decay.ntype;
    rdr.fPOT = fTotalPOT / double(fNEntries);
    rdr.fWindowWeight_cm2 = 1E-4;
    rdr.fMCWeight = dkReader->decay.nimpwt / M_PI;

    rdr.fThreePos_lab = ROOT::Math::XYZPoint{
        dkReader->decay.vx, dkReader->decay.vy, dkReader->decay.vz};
    rdr.fFourMom_lab = ROOT::Math::PxPyPzMVector{
        dkReader->decay.pdpx, dkReader->decay.pdpy, dkReader->decay.pdpz,
        nft::utils::GetMass(dkReader->decay.ptype)};
    rdr.fPDG = dkReader->decay.ptype;

    rdr.fMomAtProduction = ROOT::Math::PxPyPzEVector{
        dkReader->decay.ppdxdz * dkReader->decay.pppz,
        dkReader->decay.ppdydz * dkReader->decay.pppz, dkReader->decay.pppz,
        dkReader->decay.ppenergy};

    rdr.fParentMomAtDecay = ROOT::Math::PxPyPzEVector{
        dkReader->decay.muparpx, dkReader->decay.muparpy,
        dkReader->decay.muparpz, dkReader->decay.mupare};

    return rdr;
  }

  ~Dk2NuReader() {}
};

DECLARE_PLUGIN(IDecayParentReader, Dk2NuReader);
