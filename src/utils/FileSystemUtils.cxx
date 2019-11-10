#include "utils/FileSystemUtils.hxx"
#include "utils/StringUtils.hxx"

#include "exception/exception.hxx"

#include <dirent.h>

#include <iostream>

// #define DEBUG_GETMATCHINGFILES

namespace nft {
namespace utils {

NEW_NFT_EXCEPT(nft_env_uninitialized);

std::vector<std::string> GetMatchingFiles(std::string directory,
                                          std::string pattern) {

#ifdef DEBUG_GETMATCHINGFILES
  std::cout << "[INFO]: Looking for files matching: \"" << pattern
            << "\" in directory: " << directory << std::endl;
#endif

  directory = EnsureTrailingSlash(directory);
  pattern = DeGlobPattern(pattern);

  std::regex rpattern(pattern);

  std::vector<std::string> matches;

  DIR *dir;
  struct dirent *ent;
  dir = opendir(directory.c_str());
  if (dir != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      if (std::regex_match(ent->d_name, rpattern)) {
        matches.push_back(ent->d_name);
      }
    }
  }
  return matches;
}

std::string GetNFTDir() {
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
