#include "flux/IDecayParentReader.hxx"

#include "utils/PDGUtils.hxx"
#include "utils/ROOTUtils.hxx"

#ifdef USE_DK2NU
#include "dk2nu/tree/dk2nu.h"
#include "dk2nu/tree/dkmeta.h"
#endif

class Dk2NuReader : public IDecayParentReader {

  mutable bsim::Dk2Nu *dkReader;

  mutable nft::utils::TFile_ptr fInputTree;
  mutable nft::utils::TreeFile fDk2NuTree;

  double fTotalPOT;
  Long64_t fNEntries;

public:
  Dk2NuReader()
      : dkReader(nullptr), fInputTree(nft::utils::make_unique_TFile(nullptr)) {}
  bool CanReadFile(std::string const &file) {
    nft::utils::TFile_ptr itree = nft::utils::CheckOpenTFile(file);

    try {
      (void)nft::utils::CheckGetTTree(itree.get(), "dk2nuTree");
      (void)nft::utils::CheckGetTTree(itree.get(), "dkmetaTree");
    } catch (nft::utils::failed_to_get_TTree) {
      return false;
    }
    return true;
  }

  bool Initialize(std::string const &file) {
    if (!CanReadFile(file)) {
      return false;
    }

    fInputTree = nft::utils::CheckOpenTFile(file);

    fDk2NuTree = nft::utils::CheckGetTTree(fInputTree.get(), "dk2nuTree");

    nft::utils::TreeFile DkMetaTree =
        nft::utils::CheckGetTTree(fInputTree.get(), "dkmetaTree");

    bsim::DkMeta *meta = nullptr;
    DkMetaTree->SetBranchAddress("dkmeta", &meta);

    fTotalPOT = 0;
    for (Long64_t i = 0; i < DkMetaTree->GetEntries(); ++i) {
      DkMetaTree->GetEntry(i);
      fTotalPOT += meta->pots;
    }

    fDk2NuTree->SetBranchAddress("dk2nu", &dkReader);
    fNEntries = fDk2NuTree->GetEntries();
    fDk2NuTree->GetEntry(0);

    return true;
  }

  size_t GetN() const { return fNEntries; };

  nft::flux::DecayParent Get(size_t n) const {

    if (n >= GetN()) {
      throw IDecayParentReader_OOB() << "[ERROR]: Attempted to get Dk2Nu entry "
                                     << n << " but there are only: " << GetN();
    }

    nft::flux::DecayParent rdr;

    fDk2NuTree->GetEntry(n);

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
