#include "flux/converters/IFluxConverter.hxx"

#include "flux/DecayParent.hxx"

#include "utils/ROOTUtils.hxx"

#include "dk2nu/tree/dk2nu.h"
#include "dk2nu/tree/dkmeta.h"

#include "fhiclcpp/ParameterSet.h"

#include <cmath>

class ToDk2Nu : public IFluxConverter {

  nft::utils::TreeFile fDkOutTree;

  bsim::Dk2Nu *fDk;

  double fPOTUsed;
  double fMaxPOT;
  size_t fEntriesUsed;

public:
  ToDk2Nu() : fDk(nullptr), fPOTUsed(0), fMaxPOT(-1), fEntriesUsed(0) {}
  bool Initialize(fhicl::ParameterSet const &ps) {

    fDkOutTree = nft::utils::MakeNewTTree(ps.get<std::string>("out.file"),
                                          "dk2nuTree", "RECREATE");

    fDkOutTree->Branch<bsim::Dk2Nu>("dk2nu", &fDk);

    fDk->job = ps.get<int>("job", 0);
    fDk->potnum = ps.get<int>("potnum", 0);
    fDk->jobindx = ps.get<int>("jobindx", 0);
    fMaxPOT = ps.get<double>("MaxPOT", fMaxPOT);

    return true;
  }

  size_t GetEntriesUsed() { return fEntriesUsed; }

  double Convert(nft::flux::DecayParent const &par) {

    if (fMaxPOT > 0 && fPOTUsed > fMaxPOT) {
      fDone = true;
      return 0;
    }

    fPOTUsed += par.fPOT;
    fEntriesUsed++;

    fDk->decay = GetDecay(par);
    fDk->nuray.clear();
    fDk->ancestor.clear();
    fDk->ppvx = 0;
    fDk->ppvy = 0;
    fDk->ppvz = 0;
    fDk->tgtexit = bsim::TgtExit();
    fDk->traj.clear();
    fDk->flagbits = 0;
    fDk->vint.clear();
    fDk->vdbl.clear();

    fDkOutTree->Fill();
    fDk->potnum++;

    return par.fPOT;
  }

  ~ToDk2Nu() {
    auto metatree =
        nft::utils::AddNewTTreeToFile(fDkOutTree.file.get(), "dkmetaTree");

    bsim::DkMeta *meta = nullptr;
    metatree->Branch<bsim::DkMeta>("dkmeta", &meta);

    meta->pots = fPOTUsed;
    meta->job = fDk->job;

    metatree->Fill();
  }
};

DECLARE_PLUGIN(IFluxConverter, ToDk2Nu);
