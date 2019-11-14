SET(flux_implementation_files
  DecayParent.cxx)

SET(flux_header_files
DecayParent.hxx
IDecayParentReader.hxx
INuRaySource.hxx
NuRayGun.hxx)

add_library(nft_flux SHARED ${flux_implementation_files})

install(TARGETS nft_flux DESTINATION lib)

install(FILES ${flux_header_files} DESTINATION include/flux)

add_subdirectory(input)
add_subdirectory(converters)
