#pragma once

#include "exception/exception.hxx"

#include <cstdlib>
#include <string>

namespace nft {
namespace utils {

NEW_NFT_EXCEPT(nft_env_uninitialized);

inline std::string EnsureTrailingSlash(std::string str) {
  if (!str.size()) {
    return str;
  }
  if (str.back() != '/') {
    return str + '/';
  }
  return str;
}

inline std::string GetNFTDir() {
  const char *nftdir = getenv("NUFLUXTOOLSROOT");
  if (!nftdir) {
    throw nft_env_uninitialized()
        << "[ERROR]: Failed to read environment variable \"NUFLUXTOOLSROOT\". "
           "The NuFluxTools environment wasn't initialized.";
  }
  return EnsureTrailingSlash(nftdir);
}

} // namespace utils
} // namespace nft
