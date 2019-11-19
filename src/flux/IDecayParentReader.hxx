#pragma once

#include "plugins/traits.hxx"

#include "Math/Transform3D.h"

#include "flux/DecayParent.hxx"

class IDecayParentReader {
public:
  NEW_NFT_EXCEPT(IDecayParentReader_OOB);
  NEW_NFT_EXCEPT(IDecayParentReader_Unimplemented);

  IDecayParentReader() : fBeamToDetectorTransformation() {}

  ROOT::Math::Transform3D fBeamToDetectorTransformation;

  virtual bool CanReadFile(std::string const &file) = 0;
  virtual bool Initialize(std::string const &file) = 0;
  virtual size_t GetN() const = 0;
  virtual nft::flux::DecayParent Get(size_t) const = 0;

  virtual bool AddFiles(std::string const &add) {
    throw IDecayParentReader_Unimplemented()
        << "[ERROR]: Subclass cannot add files to reader.";
  }

  void SetTransformation(ROOT::Math::Transform3D const &t) {
    fBeamToDetectorTransformation = t;
  }

  nft::flux::DecayParent Get_ToDetectorCoords(size_t it) const {
    return Get(it).CoordTransform(fBeamToDetectorTransformation);
  }

  virtual ~IDecayParentReader() {}

  struct DecayParent_const_iterator
      : public std::iterator<
            std::input_iterator_tag, nft::flux::DecayParent const, size_t,
            nft::flux::DecayParent const *, nft::flux::DecayParent const &> {

    DecayParent_const_iterator(size_t _idx, IDecayParentReader const *_idpr) {
      idx = _idx;
      idpr = _idpr;
      if (idx < idpr->GetN()) {
        fDPCache = idpr->Get_ToDetectorCoords(idx);
      }
    }
    DecayParent_const_iterator(DecayParent_const_iterator const &other) {
      idx = other.idx;
      idpr = other.idpr;
      fDPCache = other.fDPCache;
    }
    DecayParent_const_iterator
    operator=(DecayParent_const_iterator const &other) {
      idx = other.idx;
      idpr = other.idpr;
      fDPCache = other.fDPCache;
      return (*this);
    }

    bool operator==(DecayParent_const_iterator const &other) {
      return (idpr == other.idpr) && (idx == other.idx);
    }
    bool operator!=(DecayParent_const_iterator const &other) {
      return !(*this == other);
    }
    nft::flux::DecayParent const &operator*() { return fDPCache; }
    nft::flux::DecayParent const *operator->() { return &fDPCache; }

    DecayParent_const_iterator operator++() {
      idx++;
      if (idx < idpr->GetN()) {
        fDPCache = idpr->Get_ToDetectorCoords(idx);
      }
      return *this;
    }
    DecayParent_const_iterator operator++(int) {
      DecayParent_const_iterator tmp(*this);
      idx++;
      if (idx < idpr->GetN()) {
        fDPCache = idpr->Get_ToDetectorCoords(idx);
      }
      return tmp;
    }

  private:
    size_t idx;
    IDecayParentReader const *idpr;
    nft::flux::DecayParent fDPCache;
  };

  DecayParent_const_iterator begin() const {
    return DecayParent_const_iterator(0, this);
  }
  DecayParent_const_iterator end() const {
    return DecayParent_const_iterator(GetN(), this);
  }
};

DECLARE_PLUGIN_INTERFACE(IDecayParentReader);
