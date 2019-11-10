#pragma once

#include "plugins/traits.hxx"

namespace nft {
namespace flux {
class DecayParent;
}
} // namespace nft
namespace fhicl {
class ParameterSet;
}

class IFluxConverter {
public:
  virtual bool Initialize(fhicl::ParameterSet const &ps) = 0;
  virtual double Convert(nft::flux::DecayParent const &) = 0;
  virtual size_t GetEntriesUsed() = 0;

  virtual ~IFluxConverter() {}
};

DECLARE_PLUGIN_INTERFACE(IFluxConverter);
