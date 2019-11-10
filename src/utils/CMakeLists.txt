SET(utils_implementation_files
FileSystemUtils.cxx
StringUtils.cxx)

SET(utils_header_files
Particle.hxx
PDGUtils.hxx
FileSystemUtils.hxx
ROOTUtils.hxx
T2Str.hxx
StringUtils.hxx
TerminalUtils.hxx)

add_library(nft_utils SHARED ${utils_implementation_files})

install(TARGETS nft_utils DESTINATION lib)

install(FILES ${utils_header_files} DESTINATION include/utils)
