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

#ifdef USE_DK2NU
bsim::Decay GetDecay(DecayParent const &par){
  bsim::Decay dk;

  dk.ntype = par.fDecayTo;
  dk.vx = par.fThreePos_lab.X();
  dk.vy = par.fThreePos_lab.Y();
  dk.vz = par.fThreePos_lab.Z();
  dk.pdpx = par.fFourMom_lab.X();
  dk.pdpy = par.fFourMom_lab.Y();
  dk.pdpz = par.fFourMom_lab.Z();
  dk.ppdxdz = par.fMomAtProduction.X() / par.fMomAtProduction.Z();
  dk.ppdydz = par.fMomAtProduction.Y() / par.fMomAtProduction.Z();
  dk.pppz = par.fMomAtProduction.Z();
  dk.ppenergy = par.fMomAtProduction.E();
  dk.ptype = par.fPDG;
  dk.muparpx = par.fParentMomAtDecay.X();
  dk.muparpy = par.fParentMomAtDecay.Y();
  dk.muparpz = par.fParentMomAtDecay.Z();
  dk.mupare = par.fParentMomAtDecay.E();
  dk.necm = par.fENu_com;
  dk.nimpwt = par.fMCWeight;

  return dk;
}
#endif

geom::NuRay DecayParent::Shoot(ROOT::Math::XYZPoint const &point) const {

#ifdef USE_DK2NU
  bsim::Decay dk = GetDecay(*this);

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
