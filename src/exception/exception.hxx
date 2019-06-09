#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

namespace nft {
struct nft_except : public std::exception {
  std::stringstream msgstrm;
  std::string msg;
  nft_except() : msgstrm(), msg() {}
  nft_except(nft_except const &other) : msgstrm(), msg() {
    msgstrm << other.msg;
    msg = other.msg;
  }
  const char *what() const noexcept { return msg.c_str(); }

  template <typename T> nft_except &operator<<(T const &obj) {
    msgstrm << obj;
    msg = msgstrm.str();
    return (*this);
  }
};

} // namespace nft

#define NEW_NFT_EXCEPT(EXCEPT_NAME)                                            \
  struct EXCEPT_NAME : public nft::nft_except {                                \
    EXCEPT_NAME() : nft::nft_except() {}                                       \
    EXCEPT_NAME(EXCEPT_NAME const &other) : nft::nft_except(other) {}          \
    template <typename T> EXCEPT_NAME &operator<<(T const &obj) {              \
      msgstrm << obj;                                                          \
      msg = msgstrm.str();                                                     \
      return (*this);                                                          \
    }                                                                          \
  }

namespace nft {
NEW_NFT_EXCEPT(internal_error);
}
