#pragma once

#include <sys/ioctl.h>

#include <unistd.h>

namespace nft {
namespace utils {
inline size_t GetWindowWidth() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_col;
}
} // namespace utility
} // namespace nuis
