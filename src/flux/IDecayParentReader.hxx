#pragma once

#include "plugins/traits.hxx"

#include "flux/DecayParent.hxx"

class IDecayParentReader {
public:
  virtual size_t GetN() const = 0;
  virtual nft::flux::DecayParent const &Get(size_t) const = 0;

  virtual ~IDecayParentReader() {}
};

DECLARE_PLUGIN_INTERFACE(IDecayParentReader);
