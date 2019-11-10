#include "flux/DecayParent.hxx"

#include "exception/exception.hxx"

#ifdef USE_DK2NU
#include "dk2nu/tree/calcLocationWeights.h"
#include "dk2nu/tree/dk2nu.h"
#endif

namespace nft {
namespace flux {

NEW_NFT_EXCEPT(no_dk2nu_support);
NEW_NFT_EXCEPT(calcLocationWeight_error);

geom::NuRay DecayParent::Shoot(ROOT::Math::XYZPoint const &point) const {

#ifdef USE_DK2NU
  bsim::Decay dk;

  dk.ntype = fDecayTo;
  dk.vx = fThreePos_lab.X();
  dk.vy = fThreePos_lab.Y();
  dk.vz = fThreePos_lab.Z();
  dk.pdpx = fFourMom_lab.X();
  dk.pdpy = fFourMom_lab.Y();
  dk.pdpz = fFourMom_lab.Z();
  dk.ppdxdz = fMomAtProduction.X() / fMomAtProduction.Z();
  dk.ppdydz = fMomAtProduction.Y() / fMomAtProduction.Z();
  dk.pppz = fMomAtProduction.Z();
  dk.ppenergy = fMomAtProduction.E();
  dk.ptype = fPDG;
  dk.muparpx = fParentMomAtDecay.X();
  dk.muparpy = fParentMomAtDecay.Y();
  dk.muparpz = fParentMomAtDecay.Z();
  dk.mupare = fParentMomAtDecay.E();
  dk.necm = fENu_com;
  dk.nimpwt = fMCWeight;

  double nu_energy, nu_wght;
  if (bsim::calcEnuWgt(dk, TVector3(point.X(), point.Y(), point.Z()), nu_energy,
                       nu_wght)) {
    std::cout << "[ERROR]: bsim::calcLocationWeight failed." << std::endl;
    throw calcLocationWeight_error();
  }

  ROOT::Math::XYZVector pnu = (point - fThreePos_lab).Unit() * nu_energy;

  return geom::NuRay{utils::Particle{point,
                                     ROOT::Math::PxPyPzEVector{
                                         pnu.X(), pnu.Y(), pnu.Z(), nu_energy},
                                     fDecayTo},
                     nu_wght * fMCWeight * fWindowWeight_cm2};
#else
  throw no_dk2nu_support();
#endif
}

} // namespace flux
} // namespace nft
