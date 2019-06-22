#include "flux/IDecayParentReader.hxx"

class dk2nuLiteReader : public IDecayParentReader {

  nft::flux::DecayParent fReader;

public:
  size_t GetN() const { return 0; };

  nft::flux::DecayParent const &Get(size_t) const { return fReader; }

  ~dk2nuLiteReader() {}
};

DECLARE_PLUGIN(IDecayParentReader, dk2nuLiteReader);
