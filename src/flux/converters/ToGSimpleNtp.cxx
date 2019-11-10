#include "flux/converters/IFluxConverter.hxx"

#include "flux/DecayParent.hxx"

#include "utils/ROOTUtils.hxx"

#include "rng/RandomPositionDistributions.hxx"

#include "geom/FluxWindow.hxx"

#include "TRandom3.h"

#include "fhiclcpp/ParameterSet.h"

#ifdef IS_GENIE_V3
#else
#include "FluxDrivers/GSimpleNtpFlux.h"
#endif

#include <cmath>

class ToGSimpleNtp : public IFluxConverter {

  nft::utils::TreeFile fOutTree;

  genie::flux::GSimpleNtpEntry *fGSimple;

  nft::geom::FluxWindow fWindow;

  double fMaxWeight;
  double fMaxWeightSeen;
  double fMinWeightSeen;
  double fMaxEnergySeen;
  size_t fNCache;
  size_t fEntriesUsed;
  double fPOTUsed;
  bool IsWeighted;
  std::vector<std::pair<genie::flux::GSimpleNtpEntry, double>> fEntryCache;

public:
  ToGSimpleNtp()
      : fGSimple(nullptr), fMaxWeight(0), fMaxWeightSeen(0),
        fMinWeightSeen(std::numeric_limits<double>::max()), fMaxEnergySeen(0),
        fNCache(1E4), fEntriesUsed(0), fPOTUsed(0) {}
  bool Initialize(fhicl::ParameterSet const &ps) {

    fOutTree = nft::utils::MakeNewTTree(ps.get<std::string>("out.file"),
                                        "gsimpleTree", "RECREATE");

    fWindow = nft::geom::FluxWindow(ps.get<fhicl::ParameterSet>("window"));

    fNCache = ps.get<size_t>("NCache", fNCache);
    fMaxWeight = ps.get<double>("MaxWeight", fMaxWeight);
    IsWeighted = ps.get<bool>("IsWeighted", false);

    if (!fMaxWeight) {
      if (!fNCache) {
        fNCache = 1;
        std::cout
            << "[WARN]: MaxWeight not specified during gsimple creation. "
               "As no cache is requested will guess the maxweight based on the "
               "first event, this is unlikely to give good results.."
            << std::endl;
      } else {
        std::cout << "[WARN]: MaxWeight not specified during gsimple creation. "
                     "Will use one cache-worth ("
                  << fNCache << " decaying parents) to guess the maxweight."
                  << std::endl;
      }
    }

    fOutTree->Branch<genie::flux::GSimpleNtpEntry>("gsimple", &fGSimple);

    return true;
  }

  size_t GetEntriesUsed() { return fEntriesUsed; }

  double Convert(nft::flux::DecayParent const &par) {

    double POTWritten = 0;

    nft::geom::NuRay nr = par.Shoot(fWindow.GetRandomPosition());

    auto nrvect = nr.fThreePos_lab - par.fThreePos_lab;

    nr.fWeight_cm2 *= std::abs(nrvect.Unit().Dot(fWindow.fUnitNormal));

    double dist = std::sqrt(nrvect.Mag2());

    fMaxWeightSeen = std::max(fMaxWeightSeen, nr.fWeight_cm2);
    fMinWeightSeen = std::min(fMinWeightSeen, nr.fWeight_cm2);
    fMaxEnergySeen = std::max(fMaxEnergySeen, nr.fFourMom_lab.E());

    if (fEntryCache.size() < fNCache) {
      genie::flux::GSimpleNtpEntry gsmp;
      gsmp.wgt = nr.fWeight_cm2;
      gsmp.vtxx = nr.fThreePos_lab.X();
      gsmp.vtxy = nr.fThreePos_lab.Y();
      gsmp.vtxz = nr.fThreePos_lab.Z();
      gsmp.dist = dist;
      gsmp.px = nr.fFourMom_lab.X();
      gsmp.py = nr.fFourMom_lab.Y();
      gsmp.pz = nr.fFourMom_lab.Z();
      gsmp.E = nr.fFourMom_lab.E();
      gsmp.pdg = nr.fPDG;
      gsmp.metakey = 1;
      fEntryCache.emplace_back(gsmp, par.fPOT);
    }

    if (fEntryCache.size() == fNCache) {
      if (!fMaxWeight) {
        fMaxWeight = fMaxWeightSeen * 1.1;
        std::cout << "[INFO]: Chosen rej sampling ceiling = " << fMaxWeight
                  << std::endl;
      }

      // flush the cache
      // std::cout << "[INFO]: Flushing the cache of " << fEntryCache.size()
      //           << std::endl;
      size_t i = 0;
      for (auto &ent : fEntryCache) {
        (*fGSimple) = ent.first;
        if (fGSimple->wgt > fMaxWeight) {
          std::cout << "[WARN]: Saw nuray with an MC weight of "
                    << fGSimple->wgt
                    << " but the sampling ceiling is set to: " << fMaxWeight
                    << std::endl;
        }
        if (IsWeighted) {
          fOutTree->Fill();
          fEntriesUsed++;
          POTWritten += ent.second;
          fPOTUsed += ent.second;
        } else {
          double acc_p = fGSimple->wgt / fMaxWeight;
          if (gRandom->Uniform(1) < acc_p) {
            // They've been sampled, now equi-probable.
            POTWritten += ent.second / fGSimple->wgt;
            fPOTUsed += ent.second / fGSimple->wgt;
            fGSimple->wgt = 1;
            fOutTree->Fill();
            i++;
            fEntriesUsed++;
          }
        }
      }
      // std::cout << "[INFO]: Wrote " << i << " rays for a total of "
      //           << POTWritten << "POT/" << fPOTUsed << " POT." << std::endl;
      fEntryCache.clear();
    }
    return POTWritten;
  }

  ~ToGSimpleNtp() {
    auto metatree =
        nft::utils::AddNewTTreeToFile(fOutTree.file.get(), "gsimplemeta");

    genie::flux::GSimpleNtpMeta *meta = nullptr;
    metatree->Branch<genie::flux::GSimpleNtpMeta>("gsimplemeta", &meta);

    meta->maxEnergy = fMaxEnergySeen;
    meta->minWgt = IsWeighted ? fMaxWeightSeen : 1;
    meta->maxWgt = IsWeighted ? fMinWeightSeen : 1;
    meta->protons = fPOTUsed;
    metatree->Fill();
  }
};

DECLARE_PLUGIN(IFluxConverter, ToGSimpleNtp);
