#pragma once

#include "flux/DecayParent.hxx"

namespace nft {
namespace flux{
  class IDecayParentReader {
    virtual size_t GetN() const = 0;
    virtual DecayParent const &Get(size_t) const = 0;

      virtual ~IDecayParentReader(){}

  };
}
}
